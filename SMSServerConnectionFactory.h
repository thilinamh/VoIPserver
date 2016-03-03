//
// Created by user on 1/28/16.
//

#ifndef POCOSERVER_SMSSERVERCONNECTIONFACTORY_H
#define POCOSERVER_SMSSERVERCONNECTIONFACTORY_H

#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/StreamSocket.h"


using Poco::Net::TCPServerConnectionFactory;
using Poco::Net::TCPServerConnection;
using Poco::Net::StreamSocket;

class SMSServerConnectionFactory: public TCPServerConnectionFactory  {

public:
    TCPServerConnection *createConnection(const StreamSocket &socket) override ;

    SMSServerConnectionFactory() = default;
    SMSServerConnectionFactory(const SMSServerConnectionFactory & orig)= default;
    ~SMSServerConnectionFactory()= default;
};


#endif //POCOSERVER_SMSSERVERCONNECTIONFACTORY_H
