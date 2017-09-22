#include "voipagent.h"
#include <qDebug>

VoipAgent::VoipAgent(QObject * parent) : QObject(parent), _acc(new VoipAccount(this)), _ep(new Endpoint)
{
    _ep->libCreate();
    // Initialize endpoint
    EpConfig ep_cfg;
    _ep->libInit( ep_cfg );

    // Create SIP transport. Error handling sample is shown
    TransportConfig tcfg;
    tcfg.port = 5060;
    try {
        _ep->transportCreate(PJSIP_TRANSPORT_UDP, tcfg);
        // Start the library (worker threads etc)
        _ep->libStart();
        qDebug() << "*** PJSUA2 STARTED ***";
    } catch (Error &err) {
        qDebug() << QString::fromStdString(err.info());
    }
}

void VoipAgent::connectAccount(const std::string &username, const std::string &registerDomain, const std::string &password) {

    // Configure an AccountConfig
    AccountConfig acfg;
    acfg.idUri = "sip:" + username + "@" + registerDomain;
    acfg.regConfig.registrarUri = "sip:" + registerDomain;
    AuthCredInfo cred("digest", "*", username, 0, password);
    acfg.sipConfig.authCreds.push_back( cred );
    // Create the account
    _acc->create(acfg);

    bool ok = QObject::connect(_acc, SIGNAL(s_onIncomingCall()), this, SLOT(onIncomingCall()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}

void VoipAgent::onIncomingCall() {
    emit s_onIncomingCall();
}

void VoipAgent::answerCall() {
    _acc->answerCall();
}

void VoipAgent::hangupCall() {
    _acc->hangupCall();
}

void VoipAgent::setStatusOnline() {
    try {
        PresenceStatus ps;
        ps.status = PJSUA_BUDDY_STATUS_ONLINE;
        // Optional, set the activity and some note
        //ps.activity = PJRPID_ACTIVITY_AWAY;
        ps.note = "Available";
        _acc->setOnlineStatus(ps);
    } catch(Error& err) {
        qDebug() << QString::fromStdString(err.info());
    }
}

void VoipAgent::testCall() {
    const string call_uri = "sip:john27@opensips.org";
    Call *call = new VoipCall(*_acc);
    CallOpParam prm(true); // Use default call settings
    try {
        call->makeCall(call_uri, prm);
    } catch(Error& err) {
        qDebug() << QString::fromStdString(err.info());
        shutdown();
    }
}

void VoipAgent::shutdown() {
    if(_acc != NULL) {
        delete _acc;
        _acc = NULL;
    } else {
        qDebug() << "INFO: Cannot delete account, Account is not initialized";
    }

    if(_ep != NULL) {
        try{
            _ep->libDestroy();
            delete _ep;
            _ep = NULL;
        } catch(Error& err) {
            qDebug() << "Startup error: " + QString::fromStdString(err.info());
        }
    } else {
        qDebug() << "INFO: Cannot delete endpoint, Endpoint is not initialized";
    }
}


