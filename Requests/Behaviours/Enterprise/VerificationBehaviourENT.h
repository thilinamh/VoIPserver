//
// Created by tm on 19/01/16.
//

#ifndef POCOSERVER_VERIFICATIONBEHAVIOURENT_H
#define POCOSERVER_VERIFICATIONBEHAVIOURENT_H


#include "../VerificationBehaviour.h"
#include "../../../Helper/Singleton.h"

class VerificationBehaviourENT :public VerificationBehaviour, public Singleton<VerificationBehaviourENT>{

public:
    bool verify(const std::string &verification_num, const std::string &uuid) override final;
private:
    friend class Singleton;
    VerificationBehaviourENT() : Singleton() { cout<<"VerifBehaviourCOMM obj created"<<endl;}
};


#endif //POCOSERVER_VERIFICATIONBEHAVIOURENT_H
