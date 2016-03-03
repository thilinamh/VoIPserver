//
// Created by user on 12/21/15.
//

#ifndef POCOSERVER_VERIFICATIONBEHAVIOURCOMMUNITY_H
#define POCOSERVER_VERIFICATIONBEHAVIOURCOMMUNITY_H


#include "../../../Helper/Singleton.h"
#include "../VerificationBehaviour.h"

class VerificationBehaviourCOMM :public VerificationBehaviour, public Singleton<VerificationBehaviourCOMM> {
public:
    bool verify(const std::string &verification_num, const std::string &uuid) override final;
private:
    friend class Singleton;
    VerificationBehaviourCOMM() : Singleton() { cout<<"VerifBehaviourCOMM obj created"<<endl;}
};


#endif //POCOSERVER_VERIFICATIONBEHAVIOURCOMMUNITY_H
