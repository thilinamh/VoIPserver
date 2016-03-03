//
// Created by user on 2/10/16.
//

#ifndef POCOSERVER_CALLINGSTATE_H
#define POCOSERVER_CALLINGSTATE_H


#include "../../Helper/Singleton.h"

class CallingState:public State, public Singleton<CallingState> {

public:
    virtual void processRequest(const std::string &data, User &context);

private:
    friend class Singleton; // because we are invoking private ctor from template
    CallingState(){cout<<"CallingState created "<<endl;};
    CallingState(const CallingState& src);
    CallingState& operator=(const CallingState& rhs);
};


#endif //POCOSERVER_CALLINGSTATE_H
