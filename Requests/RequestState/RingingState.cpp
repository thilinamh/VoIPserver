//
// Created by user on 2/8/16.
//
/*this state is valid only for callee*/
#include "RingingState.h"
#include "../../UserStructure/User.hxx"
#include "../../UserStructure/LoggedUsersMap.h"
#include "LoginState.h"
#include "InCallState.h"

#define REMOTE_REJECT "rrj\r\n"
#define CONNECT "con "
void RingingState::processRequest(const std::string &data, User &context) {
    std::cout << context.getUid()<<" in RingingState " << data << std::endl;

    if (data.compare(0, 3, "ans") == 0) {// call receiver sends this
        if (context.Session()) {
            try {
                context.Session()->startUDP();
                //context.Session()->getCaller().writeToClient(CONNECT+context.Session()->getPortOf());
            }catch (const Poco::NotFoundException& e){ // if the observer list is zero
                cerr<<e.what()<<endl;
            }
        //context.setCurrent_state(InCallState::getInstance());

        }
    }else if(data.compare(0, 3, "rej") == 0) {
        if (context.Session().get()) {
            context.Session()->unregisterObserver(context.shared_from_this());
            context.Session()= nullptr;

        }else{
            cerr<<"err session"<<endl;
        }
        context.setCurrent_state(LoginState::getInstance());

    }else{
        context.setCurrent_state(LoginState::getInstance());
        throw std::invalid_argument("Thrown to login State");
    }
}
