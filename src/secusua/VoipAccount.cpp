/*
 * VoipAccount.cpp
 *
 *  Created on: Sep 15, 2017
 *      Author: sigterm
 */
#include "VoipAccount.h"


VoipAccount::VoipAccount(QObject * parent) : QObject(parent),
                                            m_invokeManager(new InvokeManager(this))

{
}

void VoipAccount::notifyIncomingCall() {
    qDebug() << "VOIPSERVICE: Incoming call, displaying alert";
    m_callDialog = new bb::platform::NotificationDialog(this);

    m_callDialog->setTitle("Incoming call");
    m_callDialog->setBody("Call from: HQ");

    m_callDialog->appendButton(new bb::system::SystemUiButton("Answer"));
    m_callDialog->appendButton(new bb::system::SystemUiButton("Hangup"));
   bool success = QObject::connect(m_callDialog,
         SIGNAL(finished(bb::platform::NotificationResult::Type)),
         this,
         SLOT(incomingCallSelection(bb::platform::NotificationResult::Type)));
   if(success) {
       m_callDialog->show();
       m_callDialog->deleteLater();

   } else
   {
       qDebug() << "VOIPSERVICE: Failed to connect phone call notification dialog";
   }
    Q_UNUSED(success);
}


void VoipAccount::incomingCallSelection(bb::platform::NotificationResult::Type result) {
    switch(result) {
        case bb::platform::NotificationResult::ButtonSelection:
            //Button was selected now what?;
            qDebug() << "VOIPSERVICE: Notification dialog button was selected, but which one?";
            if(m_callDialog->buttonSelection()->label().compare("Answer")) {
                qDebug() << "VOIPSERVICE: Notification dialog button ANSWER selected";
            }
            else if(m_callDialog->buttonSelection()->label().compare("Hangup")) {
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
