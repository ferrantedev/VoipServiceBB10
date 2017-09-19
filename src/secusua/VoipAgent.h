//voipagent.h

#ifndef VOIPAGENT_H
#define VOIPAGENT_H

#include <QObject>
#include "customsua.hpp"
#include "voipaccount.h"
#include "voipcall.h"
#include "endpoint.hpp"
#include "account.hpp"
#include <iostream>
#include <memory>

using namespace pj;

class VoipAgent : QObject
{
    Q_OBJECT
public:
    VoipAgent(QObject * parent = 0);
    void connectAccount(const std::string &username, const std::string &registerDomain, const std::string &password);
    void shutdown();
    void testCall();
    void setStatusOnline();
private:
    VoipAccount * _acc;
    Endpoint * _ep;
};

#endif // VOIPAGENT_H
