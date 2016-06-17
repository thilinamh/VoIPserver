//
// Created by tm on 26/01/16.
//
#define REMOTE_USER_NOT_AVAILABLE "una\r\n"
#define CERT_REQUEST "krq %s\r\n"
#define DISCONNECT "dec\r\n"

#include "CallSetuptState.h"
#include "../../Helper/StringHelper.h"
#include <algorithm>
#include "../../UserStructure/User.hxx"
#include ".././../UserStructure/LoggedUsersMap.h"
#include "RingingState.h"
#include "LoginState.h"
#include "CallingState.h"


void CallSetuptState::processRequest(const std::string &data, User &context) {
    std::cout << context.getUid() << " in CallSetuptState " << data.substr(0, 10) << std::endl;

    if (data.compare(0, 3, "cnr") == 0) { // client wants to dial a peer

        string encrypted = data.substr(4); //take data part

        auto elements = StringHelper::split(encrypted, ',');

        string dest_uid(elements->at(0));
        string my_pub_key(elements->at(1));
        std::replace(my_pub_key.begin(), my_pub_key.end(), ':', '\n');
        context.setMypubKey(my_pub_key);
        try {
            if (context.getUid() == dest_uid) {
                throw Poco::InvalidAccessException("You cannot call yourself ");
            }
            auto receiver = LoggedUsersMap::getInstance().findUser(dest_uid); //throws out_of_range exception
            if (receiver != nullptr && receiver->getCurrentState()==&LoginState::getInstance()) { //this is not necessary. But checking it won't kill you
                shared_ptr <CallAggregator> session(new CallAggregator());
                context.setSession(session);
                receiver->setSession(context.Session()); // receiver will register itself as a observer on 'kcs' event

                int bytes = receiver->writeToClientBlocking(CERT_REQUEST, 5000);
                if (bytes < 0) {
                    throw IOException("write failed");
                }
                receiver->setCurrent_state(CallSetuptState::getInstance());

                context.Session()->registerAsCaller(context.shared_from_this());
                cout << "registered as a caller" << endl;
                std::thread async_thread(std::function<void()>([&context]() -> void {
                    //local variable should be copied
                    auto caller = context.shared_from_this();//we can use 'context' reference but need to keep User object until thread ends
                    string sessionKey = context.Session()->getSessionKey();

                    try {

                        cout << "waiting for receiver TID :" << std::this_thread::get_id() << endl;

                        bool timeoutReached = context.Session()->wait_for_receiver(10);//seconds
                        //encrpt and send skey
                        if (!timeoutReached &&
                            caller.use_count() > 1) { //if time out not reached and this is not the only reference
                            caller->_behaviours->getCallInitializerBehaviour().sendSessionKey(sessionKey, *caller,
                                                                                              false); //throws exceptions
                            caller->setCurrent_state(CallingState::getInstance());
                        } else {
                            throw Poco::NotFoundException("Receiver end failed");
                        }
                    } catch (const Poco::Exception &e) {
                        cerr << e.message() << endl;
                        caller->writeToClient(REMOTE_USER_NOT_AVAILABLE);
                        context.writeToClient(DISCONNECT);// DISCONNECT does not work

                        if (caller->Session().get()) {
                            caller->Session()->unregisterObserver(caller);
                            caller->Session() = nullptr;
                            cout << "session cleared" << endl;
                        }
                        caller->setCurrent_state(LoginState::getInstance());

                    } catch (exception &e) {
                        cerr << "In async thread " << e.what() << endl;
                    }
                }));
                /*std::this_thread::sleep_until(std::chrono::system_clock::now() +
                                              std::chrono::nanoseconds(1000));//wait till lambda func till copy args*/
                async_thread.detach(); // now thread object can be deleted without affecting the running thread


            } else{
                cout << "user " << dest_uid << " not available" << endl;
                //this_thread::__sleep_for(std::chrono::seconds(3),std::chrono::nanoseconds(0));
                context.writeToClient(REMOTE_USER_NOT_AVAILABLE);
                context.setCurrent_state(LoginState::getInstance());
            }
        } catch (const std::out_of_range &e) { // from find(uid)
            cout << "user " << dest_uid << " not available" << endl;
            //this_thread::__sleep_for(std::chrono::seconds(3),std::chrono::nanoseconds(0));
            context.writeToClient(REMOTE_USER_NOT_AVAILABLE);
            context.setCurrent_state(LoginState::getInstance());

        } catch (const Poco::InvalidAccessException &e) { //if caller==callie
            context.writeToClient(REMOTE_USER_NOT_AVAILABLE, 0);
            cerr << e.message() << endl;
            context.setCurrent_state(LoginState::getInstance());

        } catch (const IOException &e) { //when writing to a closed Socket
            context.writeToClient(REMOTE_USER_NOT_AVAILABLE, 0);
            cerr << e.message() << endl;
            context.setCurrent_state(LoginState::getInstance());

        } catch (const Poco::NullPointerException &e) { //when trying to write after SocketConnection is deleted;
            context.writeToClient(REMOTE_USER_NOT_AVAILABLE, 0);
            cerr << e.message() << endl;
            context.setCurrent_state(LoginState::getInstance());
        }
        // my_pub_key.replace(':','',);


    }//this part has to be extracted to a seperte state.
        // unless receiver can process call request prior receiving cert request
    else if (data.compare(0, 3, "kcs") == 0) { //certificate response to server's request
        string my_pub_key = data.substr(4); //take data part
        std::replace(my_pub_key.begin(), my_pub_key.end(), ':', '\n');
        context.setMypubKey(my_pub_key);

        if (context.Session().get()) {// session could have been ended by caller
            context.Session()->registerAsReceiver(context.shared_from_this());
            string sKey(context.Session()->getSessionKey());
            context.Session()->notify_caller(); //Notify waiting caller

            try {
                context._behaviours->getCallInitializerBehaviour().sendSessionKey(sKey, context, true);
                context.setCurrent_state(RingingState::getInstance());

            } catch (const Poco::Exception &e) {
                context.writeToClient(DISCONNECT);
                context.setCurrent_state(LoginState::getInstance());
            }

        } else {
            context.setCurrent_state(LoginState::getInstance());
        }
    } else if (data.compare(0, 3, "rej") == 0) {//while caller is waiting itself can cancel the call
        if (context.Session().get()) {
            context.Session()->interrupt_caller();

        }
    }
}
