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
#define CERT_REQUEST "krq %s\r\n"
#define REMOTE_USER_NOT_AVAILABLE "una\r\n"
#include "CallSetuptState.h"
void InCallState::processRequest(const std::string &data, User &context) {
    std::cout << context.getUid() << " in InCallState " << data << std::endl;

    if (data.compare(0, 3, "rej") == 0) {
        if (context.Session().get()) {
            context.Session()->unregisterObserver(context.shared_from_this());
            context.Session().reset();
        }
        cout << "state changed to Login" << endl;
        context.setCurrent_state(LoginState::getInstance());
    } else if (data.c_str()[3] == 0x05 && data.c_str()[2] == 0x01 && data.c_str()[0] == 0x019) {
        cout << "rej1" << endl;
        if (context.Session()) {
            context.Session()->unregisterObserver(context.shared_from_this());
            context.Session().reset();
        }
    } else if (data.compare(0, 3, "usa") == 0) {
        checkUserAvailability(data, context);

    } else if (data.compare(0, 3, "ccr") == 0) { //conference call req
        string rem_number(data.substr(4));
        //trim string
        rem_number.erase(remove_if(rem_number.begin(), rem_number.end(),
                                   [](char x) { return std::isspace(x) || std::isblank(x); }),
                         rem_number.end());
        process_call_req(context, rem_number);

    }
}

void InCallState::process_call_req(User &context, const string &dest_uid) {
    try {
        if (context.getUid() == dest_uid) {
            throw Poco::InvalidAccessException("You cannot call yourself ");
        }
        auto receiver = LoggedUsersMap::getInstance().findUser(dest_uid); //throws out_of_range exception
        if (receiver != nullptr) { //this is not necessary. But checking it won't kill you
            receiver->setSession(context.Session()); // receiver will register itself as a observer on 'kcs' event

            int bytes = receiver->writeToClientBlocking(CERT_REQUEST, 5000);
            if (bytes < 0) {
                throw IOException("write failed");
            }
            context.Session()->registerAsCaller(context.shared_from_this());
            receiver->setCurrent_state(CallSetuptState::getInstance());
        }




    } catch (Poco::InvalidAccessException &e) {
        context.writeToClient(REMOTE_USER_NOT_AVAILABLE, 0);
        cerr << e.message() << endl;

    }catch (Poco::IOException &e) {
        context.writeToClient(REMOTE_USER_NOT_AVAILABLE, 0);
        cerr << e.message() << endl;

    }
}

void InCallState::process_conf_req(User &context, const string &rem_number) const {
    string b64ededKey;
    string cmd;
    unsigned char key[] = "1234567895";
    char *b64k;
    cout << "Voice Client : " << context.getUid() << " Remote Number is " << rem_number << endl;
    Base64Encode((const char *) key, &b64k, 10);
    b64ededKey = (string) b64k;
    cmd = "ccy " + context.getUid() + "," + b64ededKey + "\r\n";
    free(b64k);
    try {
        auto rem_user = LoggedUsersMap::getInstance().findUser(rem_number);

        rem_user->setSession(context.Session());
        rem_user->setCurrent_state(ConferenceSetupState::getInstance());
        rem_user->writeToClient(cmd);
        context.Session()->registerAsCaller(context.shared_from_this());

        cout << "conf req sent to " << rem_user->getUid() << endl;

    } catch (out_of_range &e) {
        context.writeToClient("uof\r\n");
        cout << "user not available" << endl;
    }
}

void InCallState::checkUserAvailability(const string &data, User &context) const {
    string rem_number = data.substr(4);
    rem_number.erase(remove_if(rem_number.begin(), rem_number.end(), [](char x) { return isspace(x) || isblank(x); }),
                     rem_number.end());
    try {
        cout << "looking for user " << rem_number << " " << rem_number.size();
        auto rem_user = LoggedUsersMap::getInstance().findUser(rem_number);
        if (rem_user->getCurrentState() == &LoginState::getInstance()) { //if not busy
            context.writeToClient("uok\r\n", 0);// user ok 67368643 716154607
        } else {
            rem_user->writeToClient("png " + context.getUid(), 0);
        }

    } catch (out_of_range &e) {
        context.writeToClient("uof\r\n", 0);
        cout << "user not available" << endl;
    }
}
