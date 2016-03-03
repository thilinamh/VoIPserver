//
// Created by tm on 07/01/16.
//

#include "BehaviourContainerENT.h"
#include "VerificationBehaviourENT.h"

RegistrationBehavior &BehaviourContainerENT::getRegistrationBehaviour() {
 if(regBehaviour== nullptr){
  regBehaviour=&RegistrationBehaviourENT::getInstance();
 }
 return *regBehaviour;
}

VerificationBehaviour &BehaviourContainerENT::getVerificationBehaviour() {
 if(BehaviourContainer::verificationBehaviour== nullptr){
  BehaviourContainer::verificationBehaviour=&VerificationBehaviourENT::getInstance();
 }
 return *BehaviourContainer::verificationBehaviour;
}

CallInItializerBehaviour &BehaviourContainerENT::getCallInitializerBehaviour() {

 if(BehaviourContainer::callIntitBehaviour== nullptr){
  BehaviourContainer::callIntitBehaviour=&CallInItializerBehaviour::getInstance();
 }
 return *BehaviourContainer::callIntitBehaviour;
}
