//
// Created by user on 2/15/16.
//

#include "ConferenceSetupState.h"
#include "../../UserStructure/User.hxx"

void ConferenceSetupState::processRequest(const std::string &data, User &context) {
    std::cout << context.getUid()<<" in ConferenceSetupState " << data<< std::endl;

    if (data.compare(0, 3, "can") == 0) {
        if(context.Session()){
            context.Session()->registerAsReceiver(context.shared_from_this());
            context.Session()->getCaller().writeToClient("ccn\r\n");
            context.Session()->startUDP();
        }

    }else  if (data.compare(0, 3, "rej") == 0) {
        context.Session()->getCaller().writeToClient("rrj\r\n");
    }
}
