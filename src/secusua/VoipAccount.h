// Voipaccount.h

#ifndef VOIPACCOUNT_H
#define VOIPACCOUNT_H
#include "customsua.hpp"
#include <QObject>
#include <QDebug>
#include <iostream>
#include "call.hpp"
#include <bb/platform/NotificationDialog>
#include <bb/system/InvokeManager>
#include <bb/system/SystemUiButton>
#include <bb/platform/NotificationResult>

using namespace pj;
using namespace bb::system;

// Subclass to extend the Account and get notifications etc.
class VoipAccount : public QObject, public Account {
    Q_OBJECT
public:
    VoipAccount(QObject * parent = 0);

    virtual void onRegState(OnRegStateParam &prm) {
        AccountInfo ai = getInfo();
        //TODO: Change this, detect status from enum below and act accordingly
        QString statusCode = 0;
        /*
        switch(prm.code){
            PJSIP_SC_TRYING = 100,
            PJSIP_SC_RINGING = 180,
            PJSIP_SC_CALL_BEING_FORWARDED = 181,
            PJSIP_SC_QUEUED = 182,
            PJSIP_SC_PROGRESS = 183,

            PJSIP_SC_OK = 200,
            PJSIP_SC_ACCEPTED = 202,

            PJSIP_SC_MULTIPLE_CHOICES = 300,
            PJSIP_SC_MOVED_PERMANENTLY = 301,
            PJSIP_SC_MOVED_TEMPORARILY = 302,
            PJSIP_SC_USE_PROXY = 305,
            PJSIP_SC_ALTERNATIVE_SERVICE = 380,

            PJSIP_SC_BAD_REQUEST = 400,
            PJSIP_SC_UNAUTHORIZED = 401,
            PJSIP_SC_PAYMENT_REQUIRED = 402,
            PJSIP_SC_FORBIDDEN = 403,
            PJSIP_SC_NOT_FOUND = 404,
            PJSIP_SC_METHOD_NOT_ALLOWED = 405,
            PJSIP_SC_NOT_ACCEPTABLE = 406,
            PJSIP_SC_PROXY_AUTHENTICATION_REQUIRED = 407,
            PJSIP_SC_REQUEST_TIMEOUT = 408,
            PJSIP_SC_GONE = 410,
            PJSIP_SC_REQUEST_ENTITY_TOO_LARGE = 413,
            PJSIP_SC_REQUEST_URI_TOO_LONG = 414,
            PJSIP_SC_UNSUPPORTED_MEDIA_TYPE = 415,
            PJSIP_SC_UNSUPPORTED_URI_SCHEME = 416,
            PJSIP_SC_BAD_EXTENSION = 420,
            PJSIP_SC_EXTENSION_REQUIRED = 421,
            PJSIP_SC_SESSION_TIMER_TOO_SMALL = 422,
            PJSIP_SC_INTERVAL_TOO_BRIEF = 423,
            PJSIP_SC_TEMPORARILY_UNAVAILABLE = 480,
            PJSIP_SC_CALL_TSX_DOES_NOT_EXIST = 481,
            PJSIP_SC_LOOP_DETECTED = 482,
            PJSIP_SC_TOO_MANY_HOPS = 483,
            PJSIP_SC_ADDRESS_INCOMPLETE = 484,
            PJSIP_AC_AMBIGUOUS = 485,
            PJSIP_SC_BUSY_HERE = 486,
            PJSIP_SC_REQUEST_TERMINATED = 487,
            PJSIP_SC_NOT_ACCEPTABLE_HERE = 488,
            PJSIP_SC_BAD_EVENT = 489,
            PJSIP_SC_REQUEST_UPDATED = 490,
            PJSIP_SC_REQUEST_PENDING = 491,
            PJSIP_SC_UNDECIPHERABLE = 493,

            PJSIP_SC_INTERNAL_SERVER_ERROR = 500,
            PJSIP_SC_NOT_IMPLEMENTED = 501,
            PJSIP_SC_BAD_GATEWAY = 502,
            PJSIP_SC_SERVICE_UNAVAILABLE = 503,
            PJSIP_SC_SERVER_TIMEOUT = 504,
            PJSIP_SC_VERSION_NOT_SUPPORTED = 505,
            PJSIP_SC_MESSAGE_TOO_LARGE = 513,
            PJSIP_SC_PRECONDITION_FAILURE = 580,

            PJSIP_SC_BUSY_EVERYWHERE = 600,
            PJSIP_SC_DECLINE = 603,
            PJSIP_SC_DOES_NOT_EXIST_ANYWHERE = 604,
            PJSIP_SC_NOT_ACCEPTABLE_ANYWHERE = 606,
            PJSIP_SC_TSX_TIMEOUT = PJSIP_SC_REQUEST_TIMEOUT,
            PJSIP_SC_TSX_TRANSPORT_ERROR = PJSIP_SC_SERVICE_UNAVAILABLE,
        }*/

        qDebug() << QString::fromStdString(string(ai.regIsActive? "*** Register:" : "*** Unregister:"))
                  << " code=" << statusCode;
    }

    virtual void onIncomingCall(OnIncomingCallParam &iprm)
    {
        qDebug() << "VOIPSERVICE: Incoming call";
        notifyIncomingCall();
        /*
        Call *call = new Call(*this, iprm.callId);
        // Just hangup for now
        CallOpParam op;
        op.statusCode = PJSIP_SC_DECLINE;
        call->hangup(op);

        // And delete the call
        delete call;
        */
    }

    void notifyIncomingCall();

    private slots:
        void incomingCallSelection(bb::platform::NotificationResult::Type result);

    private:
        bb::platform::NotificationDialog * m_callDialog;
        bb::system::InvokeManager * m_invokeManager;

};

#endif // VOIPACCOUNT_H
