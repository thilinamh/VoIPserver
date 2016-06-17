//
// Created by user on 12/18/15.
//
#define REG_OK "rgk\r\n"
#define REG_ERROR "rge\r\n"

#include "VerificationState.h"
#include "../Behaviours/VerificationBehaviour.h"
#include "../../Helper/Crypto.h"
#include "../../Helper/StringHelper.h"
#include "../../UserStructure/User.hxx"
#include "../RequestState/InitalState.h"

void VerificationState::processRequest(const std::string &data, User &context) {

    std::cout << context.getUid()<<" in Verification " << data << std::endl;

    if (data.compare(0, 3, "rfy") == 0) {
        string encrypted = data.substr(4); //take data part
        unique_ptr<string> decodedStr = Crypto::decryptRequest(&encrypted[0]);
        auto elements = StringHelper::split(*decodedStr, ',');

        for (string x:*elements) {
            cout << x << endl;
        }
        string uid = elements->at(0);
        string verif_num = elements->at(1);
        {// extended func due to connectipon close

            context.setUid(uid);

            if(context.load_by_uid()){
                cout<<"user reloaded UID : "<<context.getUid()<<"UUID : "<< context.getUuid()<<endl;
            } else{
                cout<<"error reloading"<<endl;

                if(context.load_by_uuid("92a7d0db-c962-4bde-8634-9dc507f410c0")){
                    cout<<"user loaded "<<context.getUuid()<<endl;

                }
                context.writeToClient(REG_ERROR, 0);
                return;
            }

        }

        if (context.getUid().compare(uid) == 0 /*&& context.verifyRegistration(verif_num)*/) {
            cout << "verification success" << endl;

            if(context.save()){
                context.writeToClient(REG_OK, 0);

            }else if(context.update_db()){
                context.writeToClient(REG_OK, 0);
            }else{
                context.writeToClient(REG_ERROR, 0);
            }
            context.setCurrent_state(InitalState::getInstance());

        } else {
            cout << "verification error" << endl;
            context.writeToClient(REG_ERROR, 0);
        }
    } else if (data.compare(0, 3, "sht") == 0) {
        cout << "Shutdown req rejected in verif" << endl;
    } else {
        context.setCurrent_state(InitalState::getInstance());
        throw std::invalid_argument("forwaded to initial state from verification");
    }
}
