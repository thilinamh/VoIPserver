//
// Created by user on 12/18/15.
//


#include "InitalState.h"
#include "../../UserStructure/User.hxx"
#include ".././../UserStructure/LoggedUsersMap.h"
#include "RegistrationState.h"
#include "VerificationState.h"
#include "LoginState.h"
#define REG_PENDING "rpd\r\n"
__USING_NAMESPACE_STD()
void InitalState::processRequest(const std::string &data, User &context) {
    std::cout << context.getUid()<<" in InitState " << std::endl;


    if (data.compare(0,3,"rrq")==0) {
        //context.writeToClient(REG_PENDING);
        cout << "reg req" << endl;
        context.setCurrent_state(RegistrationState::getInstance());
        throw std::invalid_argument("forwaded to registration");

    } else if(data.compare(0,3,"uid")==0){


        /*{
            context.setUid(data.substr(4));
            LoggedUsersMap::getInstance().addUser(context);
        }*/
        cout<<"Login req"<<endl;
        context.setCurrent_state(LoginState::getInstance());
        throw std::invalid_argument("forwaded to login");

    }else if(data.compare(0,3,"sht")==0){
        cout<<"Shutdown req rejected"<<endl;
        //context.closeSockets();
    }else if(data.compare(0,3,"rfy")==0) {
        cout << "verify req" << endl;
        context.setCurrent_state(VerificationState::getInstance());
        throw std::invalid_argument("forwaded to Verification");
    }else{
        cerr<<"invalid request"<<endl;
    }

}


