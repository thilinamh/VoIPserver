//
// Created by user on 12/18/15.
//
#define REG_ERROR "rge\r\n"
#define REG_PENDING "rpd\r\n"

#include "RegistrationState.h"
#include "../../UserStructure/User.hxx"
#include "../RequestState/InitalState.h"
#include "../../Helper/Crypto.h"
#include "../../Helper/StringHelper.h"
#include "VerificationState.h"

void RegistrationState::processRequest(const std::string &data, User &context) {
    std::cout << context.getUid()<<" in Registration " << data << std::endl;

    if (data.compare(0, 3, "rrq") == 0) {

        string encrypted = data.substr(4); //take data part from 4th onwards

        try {

            unique_ptr<string> decodedStr = Crypto::decryptRequest(&encrypted[0]);


            auto elements = StringHelper::split(*decodedStr, ',');

            for (string& x:*elements) {
                cout << x << endl;
            }
            if(elements->size()<3){
                throw std::invalid_argument("missing params");
            }
            context.setUid(elements->at(0));
            context.setUuid(elements->at(2));

            if (/*context.registerUser(elements->at(1))*/true) {
                cout<<"verify message sent to "<<context.getUid()<<endl;
                context.writeToClient(REG_PENDING, 0);
                context.setCurrent_state(VerificationState::getInstance());

            } else {
                cout<<"reg failed"<<endl;
                context.setCurrent_state(InitalState::getInstance());
                throw std::invalid_argument("OTP failed");
            }

            //context.save();
        } catch (std::bad_exception &e) {
            context.writeToClient(REG_ERROR, 0);
            context.setCurrent_state(InitalState::getInstance());
            cout << e.what() << endl;
        } catch (std::invalid_argument &e) {
            context.writeToClient(REG_ERROR, 0);
            context.setCurrent_state(InitalState::getInstance());
            cout << e.what() << endl;
        } catch (...) {
            context.setCurrent_state(InitalState::getInstance());
            cout << "error Registering" <<endl;
        }
    } else {
        context.setCurrent_state(InitalState::getInstance());
        throw std::invalid_argument("forwaded to initial from registration");
    }
}

