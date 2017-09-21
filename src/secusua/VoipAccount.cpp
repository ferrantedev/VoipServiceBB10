/*
 * VoipAccount.cpp
 *
 *  Created on: Sep 15, 2017
 *      Author: sigterm
 */
#include "VoipAccount.h"


VoipAccount::VoipAccount(QObject * parent) : QObject(parent) {
}

void VoipAccount::onFinished() {
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

void VoipAccount::answerCall() {
    qDebug() << "VOIPSERVICE: Answering call";
    CallOpParam op;
    op.statusCode = PJSIP_SC_OK;
    _currentCall->answer(PJSIP_SC_OK);
}

/*
void VoipAccount::incomingCallSelection(bb::platform::NotificationResult::Type result) {
    switch(result) {
        case bb::platform::NotificationResult::ButtonSelection:
            //Button was selected now what?;
            qDebug() << "VOIPSERVICE: Notification dialog button was selected, but which one?";
            if(m_callDialog->buttonSelection()->label().compare("Answer")) {
                qDebug() << "VOIPSERVICE: Notification dialog button ANSWER selected";

                bb::system::InvokeRequest request;
                request.setTarget("com.secucom.SecuVoip");
                request.setAction("bb.action.CALL");
                m_invokeManager->invoke(request);

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
}*/
