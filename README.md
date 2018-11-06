# VoipServiceBB10
Voip client implementation using PJSIP for Blackberry 10 written in C++ 11 and Qt 4.8

This implementation runs as a background service, IPC communication is handled via QSocketConnection

The project should compile, just make sure to modify Blackberry10's SDK file path in the configuration file named, SecuVoipService.pro.


Also remember to modify your sip registrar url, user account and password in VoipService::registerSipUA()
