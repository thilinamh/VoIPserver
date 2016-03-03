//
// Created by tm on 07/01/16.
//

#ifndef POCOSERVER_BEHAVIOURCONTAINERCOMM_H
#define POCOSERVER_BEHAVIOURCONTAINERCOMM_H


#include "../BehaviourContainer.h"
#include "RegistrationBehaviourCOMM.h"
#include "VerificationBehaviourCOMM.h"

class BehaviourContainerCOMM:public BehaviourContainer {

public:
    RegistrationBehavior &getRegistrationBehaviour()  override final;

    virtual CallInItializerBehaviour &getCallInitializerBehaviour() override ;

    virtual VerificationBehaviour &getVerificationBehaviour() override final;

    void pass();






};


#endif //POCOSERVER_BEHAVIOURCONTAINERCOMM_H
