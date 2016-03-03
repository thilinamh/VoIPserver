//
// Created by user on 2/15/16.
//

#ifndef POCOSERVER_CONFERENCESETUPSTATE_H
#define POCOSERVER_CONFERENCESETUPSTATE_H


#include "State.h"
#include "../../Helper/Singleton.h"

class ConferenceSetupState :public Singleton<ConferenceSetupState>, public State{

public:
    virtual void processRequest(const std::string &data, User &context);

private:
    friend class Singleton; // because we are invoking private ctor from template
    ConferenceSetupState(){cout<<"ConferenceSetupState created "<<endl;}
    ConferenceSetupState(const ConferenceSetupState& src);
    ConferenceSetupState& operator=(const ConferenceSetupState& rhs);
};


#endif //POCOSERVER_CONFERENCESETUPSTATE_H
