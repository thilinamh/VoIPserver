//
// Created by user on 2/10/16.
//
#define REMOTE_REJECT "rrj\r\n"
#include "InCallState.h"
#include "../../UserStructure/User.hxx"
#include "LoginState.h"
#include "../../Utils.h"
#include "../../UserStructure/LoggedUsersMap.h"
#include "ConferenceSetupState.h"
#include <algorithm>
void InCallState::processRequest(const std::string &data, User &context) {
    std::cout << context.getUid()<<" in InCallState " << data<< std::endl;

    if (data.compare(0, 3, "rej") == 0) {
        if(context.Session().get()) {
            context.Session()->unregisterObserver(context.shared_from_this());
            context.Session().reset();
        }
        cout<<"state changed to Login"<<endl;
        context.setCurrent_state(LoginState::getInstance());
    }else if(data.c_str()[3]==0x05 && data.c_str()[2]==0x01 && data.c_str()[0]==0x019){
        cout<<"rej1"<<endl;
        if(context.Session()) {
            context.Session()->unregisterObserver(context.shared_from_this());
            context.Session().reset();
        }
    }else if (data.compare(0, 3, "usa") == 0){
        checkUserAvailability(data, context);

    }else if (data.compare(0, 3, "ccr") == 0){ //conference call req
        string rem_number(data.substr(4));
        rem_number.erase(remove_if(rem_number.begin(), rem_number.end(), [](char x){return std::isspace(x)||std::isblank(x);}),
                         rem_number.end());
        string b64ededKey;
        string cmd;
        unsigned char key[] = "1234567895";
        char *b64k;
        cout << "Voice Client : " << context.getUid() << " Remote Number is " << rem_number << endl;
        Base64Encode((const char *)key,&b64k,10);
        b64ededKey = (string)b64k;
        cmd = "ccy " + context.getUid() + "," + b64ededKey + "\r\n";
        free(b64k);
        try {
            auto rem_user = LoggedUsersMap::getInstance().findUser(rem_number);

            rem_user->setSession(context.Session());
            rem_user->setCurrent_state(ConferenceSetupState::getInstance());
            rem_user->writeToClient(cmd);
            context.Session()->registerAsCaller(context);

            cout<<"conf req sent to "<<rem_user->getUid()<<endl;

        }catch(out_of_range& e){
            context.writeToClient("uof\r\n");
            cout<<"user not available"<<endl;
        }

    }
}

void InCallState::checkUserAvailability(const string &data, User &context) const {
    string rem_number=data.substr(4);
    rem_number.erase(remove_if(rem_number.begin(), rem_number.end(), [](char x){return isspace(x) || isblank(x);}),
                         rem_number.end());
    try {
            cout<<"lookong for user "<<rem_number<<" "<<rem_number.size();
            auto rem_user = LoggedUsersMap::getInstance().findUser(rem_number);
            if(rem_user->getCurrentState()==&LoginState::getInstance()){ //if not busy
                context.writeToClient("uok\r\n", 0);// user ok 67368643 716154607
            }else{
                rem_user->writeToClient("png " + context.getUid(), 0);
            }

        }catch(out_of_range& e){
            context.writeToClient("uof\r\n", 0);
            cout<<"user not available"<<endl;
        }
}
