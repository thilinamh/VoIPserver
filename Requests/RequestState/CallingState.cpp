//
// Created by user on 2/10/16.
//

#include "CallingState.h"
#include "../../UserStructure/User.hxx"
#include "LoginState.h"
void CallingState::processRequest(const std::string &data, User &context) {
    std::cout << context.getUid()<<" in CallingState " << data << std::endl;

    if(data.compare(0, 3, "rej") == 0) {
        if (context.Session().get()) {
            //context.Session()->interrupt_caller();//interrupt the waiting thread implmntd in CallSetupState
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
