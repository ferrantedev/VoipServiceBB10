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
    };

    // Notification when call's media state has changed.
    virtual void onCallMediaState(OnCallMediaStateParam &prm){

        CallInfo ci = getInfo();
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
        /*
        std::cout << "Call media state changed" << std::endl;
        CallInfo ci = this->getInfo();
        AudioMedia *aud_med = NULL;

        AudioMedia cap_med = Endpoint::instance().audDevManager().getCaptureDevMedia();
        AudioMedia play_med = Endpoint::instance().audDevManager().getPlaybackDevMedia();

        // Find out which media index is the audio
        for (unsigned i=0; i<ci.media.size(); ++i) {
            if (ci.media[i].type == PJMEDIA_TYPE_AUDIO) {
                aud_med = (AudioMedia *)this->getMedia(i);
                break;
            }
        }
        if (aud_med) {
            // This will connect the sound device/mic to the call audio media
            cap_med.startTransmit(*aud_med);

            // And this will connect the call audio media to the sound device/speaker
            aud_med->startTransmit(play_med);
        }
        */
    };
};


#endif // VOIPCALL_H
