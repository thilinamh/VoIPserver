//
// Created by user on 2/8/16.
//

#include "State.h"
#include "../../Helper/Singleton.h"

#ifndef POCOSERVER_RINGINGSTATE_H
#define POCOSERVER_RINGINGSTATE_H


class RingingState :public State,public Singleton<RingingState>{


public:
    virtual void processRequest(const std::string &data, User &context);

private:
    friend class Singleton; // because we are invoking private ctor from template
    RingingState(){cout<<"RingingState created "<<endl;};
    RingingState(const RingingState& src);
    RingingState& operator=(const RingingState& rhs);
};


#endif //POCOSERVER_RINGINGSTATE_H
