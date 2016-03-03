//
// Created by tm on 11/01/16.
//

#ifndef POCOSERVER_LOGINSTATE_H
#define POCOSERVER_LOGINSTATE_H

#include "../../Helper/Singleton.h"

class LoginState :public State, public Singleton<LoginState>{

public:
     void processRequest(const std::string &data, User &context) override final;


private:
    friend class Singleton; // because we are invoking private ctor from template
    LoginState(){cout<<"LoginState created "<<endl;};
    LoginState(const LoginState& src);
    LoginState& operator=(const LoginState& rhs);
};


#endif //POCOSERVER_LOGINSTATE_H
