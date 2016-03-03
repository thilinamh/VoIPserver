//
// Created by tm on 07/01/16.
//

#ifndef POCOSERVER_BEHAVIOURCONTAINERENT_H
#define POCOSERVER_BEHAVIOURCONTAINERENT_H


#include "../BehaviourContainer.h"
#include "RegistrationBehaviourENT.h"
#include "ConferenceBehaviourENT.h"

class BehaviourContainerENT:public BehaviourContainer {

public:
    virtual VerificationBehaviour &getVerificationBehaviour();

    RegistrationBehavior &getRegistrationBehaviour() override final;


    virtual CallInItializerBehaviour &getCallInitializerBehaviour();
};


#endif //POCOSERVER_BEHAVIOURCONTAINERENT_H
