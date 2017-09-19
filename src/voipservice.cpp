/*
 * Copyright (c) 2013-2015 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "voipservice.hpp"

#include <bb/platform/Notification>
#include <bb/platform/NotificationDefaultApplicationSettings>
#include <bb/system/InvokeManager>
#include <bb/system/SystemUiButton>
#include <QTimer>
#include <QDebug>

using namespace bb::platform;
using namespace bb::system;
using namespace pj;

VoipService::VoipService(int &argc, char **argv) :
                                Application(argc, argv),
                                m_server(new QTcpServer(this)),
                                m_port(7777)
{
    /*
    bool ok =  m_invokeManager->connect(m_invokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)),
            this, SLOT(handleInvoke(const bb::system::InvokeRequest&)));
    Q_ASSERT(ok);
    */
    bool ok = connect(m_server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
    listen();
}

VoipService::~VoipService()
{
    if (m_socket) {
        disconnected();
        m_socket->deleteLater();
    }
    m_server->close();
    m_server->deleteLater();
}

void VoipService::listen()
{
    qDebug() << "VOIPSERVICE: listening for connections";
    m_server->listen(QHostAddress::LocalHost, m_port);
}

void VoipService::newConnection()
{
    m_socket = m_server->nextPendingConnection();
    if (m_socket->state() == QTcpSocket::ConnectedState) {
        qDebug() << "VOIPSERVICE: new connection established.";
    }
    // Make connections for reveiving disconnect and read ready signals for the
    // new connection socket
    bool ok = connect(m_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    Q_ASSERT(ok);
    ok = connect(m_socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}

void VoipService::readyRead()
{
    qDebug() << "VOIPSERVICE: server data received";
    QByteArray ba = m_socket->read(20);
    // TODO: Emit when finished reading in the data from the droid client socket
    //Q_EMIT dataRead(QString(ba));

    int data = ba.toInt();
    qDebug() << "VOIPSERVICE: received data: " << data;

    if( data == 100) {
        // good
        qDebug() << "VOIPSERVICE: Registering to SIP server";
        registerSipUA();
    } else if( data == 300) {
        // good
        qDebug() << "VOIPSERVICE: Performing voip test call";
        testCall();
    } else {
        //Something else was received
    }
}

void VoipService::readyWrite(const int code)
{
    if (m_socket && m_socket->state() == QTcpSocket::ConnectedState) {
        m_socket->write(QByteArray::number(code));
        m_socket->flush();
    }
}

void VoipService::disconnected()
{
    qDebug() << "VOIPSERVICE: Socket disconnected";
    disconnect(m_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    disconnect(m_socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    m_socket->close();
}

void VoipService::registerSipUA() {
    _agent = new VoipAgent(this);
    std::string username = "jenny27";
    std::string registerDomain = "opensips.org";
    std::string password = "serena";
    _agent->connectAccount(username, registerDomain, password);
    _agent->setStatusOnline();
    bool ok = QObject::connect(_agent, SIGNAL(s_onIncomingCall()), this, SLOT(onIncomingCall()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
    readyWrite(200);

}

void VoipService::testCall() {
    _agent->testCall();
}

void VoipService::handleInvoke(const bb::system::InvokeRequest & request)
{
    if (request.action().compare("com.secucom.SecuVoipService.START") == 0) {
        qDebug() << "INFO: VoipService: Invoked with START command";
        triggerNotification();
    }
}

void VoipService::triggerNotification()
{
    // Timeout is to give time for UI to minimize
    QTimer::singleShot(2000, this, SLOT(onTimeout()));
}

void VoipService::onTimeout()
{
    Notification::clearEffectsForAll();
    Notification::deleteAllFromInbox();
    //m_notify->notify();
}


bool VoipService::notify(QObject* receiver, QEvent* event)
{
    try {
        return Application::notify(receiver, event);
    } catch (std::exception &e) {
        qDebug() << "Caught an exception" << endl;
        qFatal("Error %s", e.what(), typeid(*event).name(), qPrintable(receiver->objectName()),
                typeid(*receiver).name());
    }   catch (...) {
        qFatal("Error <unknown> sending event %s to object %s (%s)",
            typeid(*event).name(), qPrintable(receiver->objectName()),
            typeid(*receiver).name());
    }

    return false;
}


void VoipService::onIncomingCall() {
    /*
        <invoke-target id="com.secucom.SecuVoipApp">
        <invoke-target-name>SecuVoip Phone</invoke-target-name>
        <icon><image>icon.png</image></icon>
        <type>card.voiphone</type>
        <filter>
            <action>com.secucom.VOIPHONE</action>
            <mime-type>phone/voip</mime-type>
        </filter>
     */
    InvokeRequest request;
    request.setTarget("com.secucom.SecuVoipApp");
    request.setAction("com.secucom.VOIPHONE");
    request.setMimeType("voip/phone");
    request.setTargetTypes(InvokeTarget::Card);

    InvokeManager invokeManager;
    InvokeTargetReply * repl = invokeManager.invoke(request);
    repl->setParent(this);
    //const InvokeTargetReply *reply = m_invokeManager->invoke(request);
    if (repl) {
        // Ensure that processInvokeReply() is called when the invocation has finished
        bool ok = QObject::connect(repl, SIGNAL(finished()), this, SLOT(onFinished()));
        Q_ASSERT(ok);
        Q_UNUSED(ok);
        //m_invokeTargetReply = repl;
    } else {
        qDebug() << tr("Invoke Failed! Reply object is empty.");
        return;
    }
}

void VoipService::onFinished() {
    InvokeTargetReply * reply = qobject_cast<bb::system::InvokeTargetReply*>(sender());
    switch(reply->error()){
        case bb::system::InvokeReplyError::NoTarget:
            qDebug() << "VOIPSERVICE: Failed invoking VOIPAPP, error, NOTARGET";
            break;
        case bb::system::InvokeReplyError::BadRequest:
            qDebug() << "VOIPSERVICE: Failed invoking VOIPAPP, error, BADREQUEST";
            break;
        case bb::system::InvokeReplyError::Internal:
            qDebug() << "VOIPSERVICE: Failed invoking VOIPAPP, error, INTERNAL";
            break;
        default:
            qDebug() << "VOIPSERVICE: Invoke VOIPAPP succeeded";
            break;
    }
    reply->deleteLater();
}
