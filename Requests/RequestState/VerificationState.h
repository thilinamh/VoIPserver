//
// Created by user on 12/18/15.
//

#ifndef POCOSERVER_VERIFICATIONSTATE_H
#define POCOSERVER_VERIFICATIONSTATE_H


#include "RegistrationState.h"

class VerificationState:public State, public Singleton<VerificationState> {

public:
     void processRequest(const std::string &data, User &context) override final ;

private:
    friend class Singleton; // because we are invoking private ctor from template
    VerificationState(){cout<<"VerificationState created "<<endl;};
    VerificationState(const VerificationState& src);
    VerificationState& operator=(const VerificationState& rhs);
};


#endif //POCOSERVER_VERIFICATIONSTATE_H
