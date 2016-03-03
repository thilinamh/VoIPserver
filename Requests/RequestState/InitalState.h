//
// Created by user on 12/18/15.
//

#ifndef POCOSERVER_INITALSTATE_H
#define POCOSERVER_INITALSTATE_H

#include "State.h"
#include "../../Helper/Singleton.h"
//#include "/Helper/Singleton.h"

class InitalState: public Singleton<InitalState>, public State {

public:
    void processRequest(const std::string &data, User &context) override;

private:
    friend class Singleton;

    InitalState() : Singleton(), State() {
        std::cout<<"InitialState created"<<std::endl;
    }
    InitalState(const InitalState& src);
    InitalState& operator=(const InitalState& rhs);

};


#endif //POCOSERVER_INITALSTATE_H
