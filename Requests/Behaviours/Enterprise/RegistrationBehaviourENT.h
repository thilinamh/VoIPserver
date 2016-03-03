//
// Created by user on 12/21/15.
//

#ifndef POCOSERVER_REGISTRATIONBEHAVIOURENTERPRISE_H
#define POCOSERVER_REGISTRATIONBEHAVIOURENTERPRISE_H

#include "../RegistrationBehavior.h"
#include "../../../Helper/Singleton.h"

class RegistrationBehaviourENT :public Singleton<RegistrationBehaviourENT>, public RegistrationBehavior {

public:
    virtual bool registerUser(const string &uid, const string &uuid, const string &extra) override ;

    //virtual bool registerUser(const string &uid, const string &uuid) override;
};


#endif //POCOSERVER_REGISTRATIONBEHAVIOURENTERPRISE_H
