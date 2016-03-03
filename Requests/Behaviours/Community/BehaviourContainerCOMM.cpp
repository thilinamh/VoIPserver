//
// Created by tm on 07/01/16.
//

#include "BehaviourContainerCOMM.h"

RegistrationBehavior &BehaviourContainerCOMM::getRegistrationBehaviour()  {
    if(BehaviourContainer::regBehaviour== nullptr){
        BehaviourContainer::regBehaviour=&RegistrationBehaviourCOMM::getInstance();
    }
    return *BehaviourContainer::regBehaviour;
}

void BehaviourContainerCOMM::pass() {

}

VerificationBehaviour &BehaviourContainerCOMM::getVerificationBehaviour() {
    if(BehaviourContainer::verificationBehaviour== nullptr){
        BehaviourContainer::verificationBehaviour=&VerificationBehaviourCOMM::getInstance();
    }
    return *BehaviourContainer::verificationBehaviour;
}

CallInItializerBehaviour &BehaviourContainerCOMM::getCallInitializerBehaviour() {
    if(BehaviourContainer::callIntitBehaviour== nullptr){
        BehaviourContainer::callIntitBehaviour=&CallInItializerBehaviour::getInstance();
    }
    return *BehaviourContainer::callIntitBehaviour;
}
