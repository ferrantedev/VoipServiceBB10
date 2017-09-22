//voipcall.h
#ifndef VOIPCALL_H
#define VOIPCALL_H
#include "customsua.hpp"
#include <iostream>
#include "media.hpp"
#include <QDebug>

using namespace pj;

class VoipCall : public Call
{
public:
    VoipCall(Account &acc, int call_id = PJSUA_INVALID_ID)
    : Call(acc, call_id)
    { }

    ~VoipCall()
    { }

    // Notification when call's state has changed.
    virtual void onCallState(OnCallStateParam &prm) {
        qDebug() << "Call state changed";
            CallInfo ci = getInfo();
            if (ci.state == PJSIP_INV_STATE_DISCONNECTED) {
                /* Delete the call */
                delete this;
            }
    };

    // Notification when call's media state has changed.
    virtual void onCallMediaState(OnCallMediaStateParam &prm){
        CallInfo ci = getInfo();

        if(ci.lastStatusCode == PJSIP_SC_RINGING ) {
            qDebug() << "SERVICE: Call accepted, connecting audio";
            try {
               // Iterate all the call medias
               for (unsigned i = 0; i < ci.media.size(); i++) {
                   if (ci.media[i].type==PJMEDIA_TYPE_AUDIO && getMedia(i)) {
                       AudioMedia *aud_med = (AudioMedia *)getMedia(i);

                       // Connect the call audio media to sound device
                       AudDevManager& mgr = Endpoint::instance().audDevManager();
                       aud_med->startTransmit(mgr.getPlaybackDevMedia());
                       mgr.getCaptureDevMedia().startTransmit(*aud_med);
                   }
               }
           } catch(Error& err) {
               qDebug() << QString::fromStdString(err.info());
           }
        }
    };
};


#endif // VOIPCALL_H
