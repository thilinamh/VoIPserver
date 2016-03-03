/* 
 * File:   TCPClientConnectionFactory.h
 * Author: user
 *
 * Created on December 11, 2015, 5:22 PM
 */

#ifndef TCPCLIENTCONNECTIONFACTORY_H
#define	TCPCLIENTCONNECTIONFACTORY_H

#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/StreamSocket.h"


using Poco::Net::TCPServerConnectionFactory;
using Poco::Net::TCPServerConnection;
using Poco::Net::StreamSocket;

class ServerConnectionFactory : public TCPServerConnectionFactory {
public:
    ServerConnectionFactory();
    ServerConnectionFactory(const ServerConnectionFactory & orig);
    virtual ~ServerConnectionFactory();
    TCPServerConnection* createConnection(const StreamSocket& socket);

private:

};

#endif	/* TCPCLIENTCONNECTIONFACTORY_H */

