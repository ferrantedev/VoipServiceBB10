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

#ifndef SERVICE_H_
#define SERVICE_H_

#include <bb/Application>
#include <QObject>
#include "secusua/VoipAgent.h"
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <bb/system/InvokeManager>
#include <bb/platform/Notification>
#include <bb/platform/NotificationResult>


namespace bb {
    class Application;
    namespace platform {
        class Notification;
    }
    namespace system {
        class InvokeManager;
        class InvokeRequest;
    }
}

using namespace pj;
using namespace bb;
using namespace bb::system;

class VoipService : public Application
{
    Q_OBJECT
public:
    VoipService(int &argc, char **argv);
    ~VoipService();
    bool notify(QObject* receiver, QEvent* event);

    /**
     * Method to start listening on the server socket
     * for new connections.
     */
    void listen();

    void notifyIncomingCall();

public Q_SLOTS:
    /**
     * This method is invoked through the signal/slot
     * mechanism when a new connection occurs on the server socket.
     */
    void newConnection();

    /**
     * This method is invoked through the signal/slot mechanism
     * as well, when the socket is ready to read data.
     */
    void readyRead();

    /**
     * This method is used in order to write data out
     * to the socket.
     */
    void readyWrite(const int code);

    /**
     * This method is invoked when the socket connection disconnects.
     */
    void disconnected();


private slots:
    void handleInvoke(const bb::system::InvokeRequest &);
    void onTimeout();
    void onIncomingCall();
    void onFinished();
    void onSelected(bb::platform::NotificationResult::Type);

private:
    void registerSipUA();
    void testCall();
    void triggerNotification();

    bb::platform::NotificationDialog * m_callDialog;
    bb::system::InvokeManager * m_invokeManager;
    bb::platform::Notification * m_notify;
    VoipAgent * _agent;

    // The port used for the server socket
    int m_port;
    //The server socket variable
    QTcpServer *m_server;
    //The socket that is created when a connection with the client is made
    QTcpSocket *m_socket;
};

#endif /* SERVICE_H_ */
