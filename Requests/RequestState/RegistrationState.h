//
// Created by user on 12/18/15.
//

#ifndef POCOSERVER_REGISTRATIONSTATE_H
#define POCOSERVER_REGISTRATIONSTATE_H

#include "State.h"
//#include "../Behaviours/RegistrationBehavior.h"
#include "../../Helper/Singleton.h"

class RegistrationState :public State, public Singleton<RegistrationState>{

public:

    void processRequest(const std::string &data, User &context) override final;


private:
    friend class Singleton; // because we are invoking private ctor from template
    RegistrationState(){cout<<"RegistrationState created "<<endl;};
    RegistrationState(const RegistrationState& src)= delete;
    RegistrationState& operator=(const RegistrationState& rhs)= delete;


};


#endif //POCOSERVER_REGISTRATIONSTATE_H
