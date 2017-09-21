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
#include <bb/platform/NotificationDialog>
#include <bb/platform/NotificationDefaultApplicationSettings>
#include <bb/system/InvokeManager>
#include <bb/system/SystemUiButton>
#include <bb/system/InvokeTarget>
#include <QTimer>
#include <QtCore/QUrl>
#include <QDebug>

using namespace bb::platform;
using namespace bb::system;
using namespace pj;

VoipService::VoipService(int &argc, char **argv) :
                                Application(argc, argv),
                                m_server(new QTcpServer(this)),
                                m_port(7777),
                                m_invokeManager(new InvokeManager(this)),
                                m_notify(new Notification(this))
{
    m_invokeManager->connect(m_invokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)),
            this, SLOT(handleInvoke(const bb::system::InvokeRequest&)));
    /*
     bool ok =  m_invokeManager->connect(m_invokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)),
     this, SLOT(handleInvoke(const bb::system::InvokeRequest&)));
     Q_ASSERT(ok);
     */

    NotificationDefaultApplicationSettings settings;
    settings.setPreview(NotificationPriorityPolicy::Allow);
    settings.apply();

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

    QString data = ba;
    qDebug() << "VOIPSERVICE: received data: " << data;
    if(data == "REGISTER") {
            // good
            qDebug() << "VOIPSERVICE: Registering to SIP server";
            registerSipUA();
    }
    else if (data == "CALL"){
            // good
            qDebug() << "VOIPSERVICE: Performing voip test call";
            testCall();
    }
    else if (data == "STATUS"){
            qDebug() << "VOIPSERVICE: STATUS queried";
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

void VoipService::onIncomingCall()
{
    qDebug() << "Invoking SecuVoip Ui";
    bb::system::InvokeRequest request;
    request.setTarget("com.secucom.SecuVoip");
    request.setAction("bb.action.START");
    NotificationDialog * m_callDialog = new NotificationDialog(this);
    m_callDialog->setTitle("Incoming Call");
    m_callDialog->setBody("Tap to answer");
    //m_callDialog->setInvokeRequestAt(request);
    m_callDialog->setRepeat(true);
    m_callDialog->setSoundUrl(QUrl("shared/documents/rsound.m4a"));

    /*
     m_notify->setTitle("AN alert");
     m_notify->setBody("Open Secuvoip");
     m_notify->setInvokeRequest(request);
     */
    Notification::clearEffectsForAll();
    Notification::deleteAllFromInbox();

    m_callDialog->appendButton(new bb::system::SystemUiButton("Answer"));
    m_callDialog->appendButton(new bb::system::SystemUiButton("Hangup"));
    bool success = QObject::connect(m_callDialog, SIGNAL(finished(bb::platform::NotificationResult::Type)),
                                    this, SLOT(onSelected(bb::platform::NotificationResult::Type)));
    m_callDialog->setInvokeRequestAt(0, request);
    if (success) {
        m_callDialog->show();
    } else {

    }
}
void VoipService::onSelected(bb::platform::NotificationResult::Type sel) {
    qDebug() << "VOIPSERVICE: Notification button selected";
    NotificationDialog * dialog = qobject_cast<bb::platform::NotificationDialog*>(sender());
    switch(sel) {
            case bb::platform::NotificationResult::ButtonSelection:
                //Button was selected now what?;
                if(dialog->buttonSelection()->label() == "Answer") {
                    qDebug() << "VOIPSERVICE: Notification dialog button ANSWER selected";
                    _agent->answerCall();
                }
                else if(dialog->buttonSelection()->label() == "Hangup") {
                    qDebug() << "VOIPSERVICE: Notification dialog button HANGUP selected";
                } else {
                    qDebug() << "VOIPSERVICE: Notification dialog button UKNOWN selected";
                }
                break;
            case bb::platform::NotificationResult::None:
                qDebug() << "VOIPSERVICE: Notification's selection SKIPPED button selcted";
                break;
            case bb::platform::NotificationResult::Error:
                qDebug() << "VOIPSERVICE: Something wrong happened with the notification's button selection";
                break;
            default:
                qDebug() << "VOIPSERVICE: Something wrong happened with the notification's button selection";
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
