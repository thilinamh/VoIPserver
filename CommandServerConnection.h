//
// Created by user on 2/19/16.
//

#ifndef POCOSERVER_COMMANDSERVERCONNECTION_H
#define POCOSERVER_COMMANDSERVERCONNECTION_H


#include "ServerConnection.h"

class CommandServerConnection : public ServerConnection {

public:
    virtual void run() override;
    CommandServerConnection(const StreamSocket &client);

    virtual ~CommandServerConnection() ;
};


#endif //POCOSERVER_COMMANDSERVERCONNECTION_H
