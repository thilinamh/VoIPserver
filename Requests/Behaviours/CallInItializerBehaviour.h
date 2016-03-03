//
// Created by tm on 26/01/16.
//

#ifndef POCOSERVER_CALLINITIALIZERBEHAVIOUR_H
#define POCOSERVER_CALLINITIALIZERBEHAVIOUR_H

#include <iostream>
#include "../../Helper/Singleton.h"
class User;
using namespace std;
class CallInItializerBehaviour:public Singleton<CallInItializerBehaviour>{
public:
    void sendSessionKey(string sKey, User &user, bool isReciever);

private:
    friend class Singleton; // because we are invoking private ctor from template
    CallInItializerBehaviour(){cout<<"CallInItializerBehaviour created "<<endl;};
    CallInItializerBehaviour(const CallInItializerBehaviour& src);
    CallInItializerBehaviour& operator=(const CallInItializerBehaviour& rhs);
};


#endif //POCOSERVER_CALLINITIALIZERBEHAVIOUR_H
