APP_NAME = SecuVoipService

CONFIG += qt warn_on

include(config.pri)

LIBS += -lbb -lbbsystem -lbbplatform
	
DEFINES += PJ_AUTOCONF
	
PJ_INCLUDEPATH += $$quote(third_party/pjsip/include)
	
PJ_LIBPATH += $$quote(-Lthird_party/pjsip/lib)

PJ_LIBPATH += $$quote(-L/Applications/Momentics.app/target_10_3_1_995/qnx6/armle-v7/usr/lib)
PJ_LIBPATH += $$quote(-L/Applications/Momentics.app/target_10_3_1_995/qnx6/armle-v7/lib)
PJ_LIBPATH += $$quote(-L/Applications/Momentics.app/host_10_3_1_12/darwin/x86/usr/lib/gcc/arm-unknown-nto-qnx8.0.0eabi/4.6.3)
	
PJ_LIBS += -lpjsua-arm-unknown-nto-qnx8.0.0eabi
PJ_LIBS += -lpjsip-ua-arm-unknown-nto-qnx8.0.0eabi
PJ_LIBS += -lpjsip-simple-arm-unknown-nto-qnx8.0.0eabi
PJ_LIBS += -lpjsip-arm-unknown-nto-qnx8.0.0eabi
PJ_LIBS += -lpjmedia-codec-arm-unknown-nto-qnx8.0.0eabi
PJ_LIBS += -lpjmedia-arm-unknown-nto-qnx8.0.0eabi
PJ_LIBS += -lpjmedia-videodev-arm-unknown-nto-qnx8.0.0eabi
PJ_LIBS += -lpjmedia-audiodev-arm-unknown-nto-qnx8.0.0eabi
PJ_LIBS += -lpjmedia-arm-unknown-nto-qnx8.0.0eabi
PJ_LIBS += -lpjnath-arm-unknown-nto-qnx8.0.0eabi
PJ_LIBS += -lpjlib-util-arm-unknown-nto-qnx8.0.0eabi
PJ_LIBS += -lsrtp-arm-unknown-nto-qnx8.0.0eabi
PJ_LIBS += -lresample-arm-unknown-nto-qnx8.0.0eabi
PJ_LIBS += -lgsmcodec-arm-unknown-nto-qnx8.0.0eabi
PJ_LIBS += -lspeex-arm-unknown-nto-qnx8.0.0eabi
PJ_LIBS += -lilbccodec-arm-unknown-nto-qnx8.0.0eabi
PJ_LIBS += -lg7221codec-arm-unknown-nto-qnx8.0.0eabi
PJ_LIBS += -lwebrtc-arm-unknown-nto-qnx8.0.0eabi
PJ_LIBS += -lpj-arm-unknown-nto-qnx8.0.0eabi
PJ_LIBS += -lasound
	
INCLUDEPATH += $$PJ_INCLUDEPATH
LIBS += $$PJ_LIBPATH
LIBS += $$PJ_LIBS
LIBS += -lOpenAL -lalut -laudio_manager -lsocket -lasound -lbbsystem -lm