//
// Created by tm on 26/01/16.
//

#include "State.h"
#include "../../Helper/Singleton.h"

#ifndef POCOSERVER_CALLREQUEST_H
#define POCOSERVER_CALLREQUEST_H


class CallSetuptState :public State,public Singleton<CallSetuptState> {


public:
    void processRequest(const std::string &data, User &context);

private:
    friend class Singleton;
    CallSetuptState() {cout<<"CallSetupState created"<<endl; };
    CallSetuptState(const CallSetuptState & src);
    CallSetuptState & operator=(const CallSetuptState & rhs);
};


#endif //POCOSERVER_CALLREQUEST_H
