/* 
 * File:   TCPClientConnectionFactory.cpp
 * Author: user
 * 
 * Created on December 11, 2015, 5:22 PM
 */
#include <tr1/memory>
#include <thread>
#include "ServerConnectionFactory.h"
#include "UserStructure/User.hxx"
#include "UserStructure/CommunityUser.h"

//#include "ServerConnection.h"
ServerConnectionFactory::ServerConnectionFactory() {
}

ServerConnectionFactory::ServerConnectionFactory(const ServerConnectionFactory & orig) {
}

ServerConnectionFactory::~ServerConnectionFactory() {
}

TCPServerConnection*ServerConnectionFactory::createConnection(const StreamSocket& socket){
   shared_ptr<CommunityUser> usr (new CommunityUser(socket));
   usr->bindWithServer();


   return dynamic_cast<TCPServerConnection*>(&(usr->getServerConnection()));
}

