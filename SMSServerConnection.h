//
// Created by user on 1/28/16.
//

#ifndef POCOSERVER_SMSSERVERCONNECTION_H
#define POCOSERVER_SMSSERVERCONNECTION_H


#include "ServerConnection.h"
using Poco::Net::TCPServerConnection;
using Poco::Net::StreamSocket;

class SMSServerConnection: public ServerConnection {


public:
    void run() override final;
    ~SMSServerConnection() ;

    SMSServerConnection(const StreamSocket &client) ;


};


#endif //POCOSERVER_SMSSERVERCONNECTION_H
