//
// Created by user on 2/10/16.
//

#ifndef POCOSERVER_INCALLSTATE_H
#define POCOSERVER_INCALLSTATE_H


#include "../../Helper/Singleton.h"

class InCallState: public Singleton<InCallState>, public State {

public:
    virtual void processRequest(const std::string &data, User &context);

private:
    friend class Singleton; // because we are invoking private ctor from template
    InCallState(){cout<<"InCallState created "<<endl;}
    InCallState(const InCallState& src);
    InCallState& operator=(const InCallState& rhs);

    void checkUserAvailability(const string &data, User &context) const;
};


#endif //POCOSERVER_INCALLSTATE_H
