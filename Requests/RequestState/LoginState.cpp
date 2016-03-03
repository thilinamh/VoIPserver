//
// Created by tm on 11/01/16.
//
#define PROCESSING_ERROR "per\r\n"
#define LOGIN_OK "rgk "
#define REG_ERROR "rge\r\n"
#include "LoginState.h"

#include "InitalState.h"
#include "../../UserStructure/User.hxx"
#include "../../Helper/Crypto.h"
#include "../../Helper/StringHelper.h"
#include "../../UserStructure/LoggedUsersMap.h"
#include "../../Helper/CallAggregator.h"
#include "CallSetuptState.h"
#include <thread>
void LoginState::processRequest(const std::string &data, User &context) {
    std::cout << context.getUid()<<" in LoginState " << data.substr(0,20) << std::endl;
    if (data.compare(0, 3, "uid") == 0) {

        string encrypted = data.substr(4); //take data part from 4th onwards
        try {

            unique_ptr<string> decrptedStr = Crypto::decryptRequest(&encrypted[0]);
            auto elements = StringHelper::split(*decrptedStr, ',');
            string uid=(*elements)[0];
            string uuid=(*elements)[1];

            cout << "loading user \nuid : " << uid << endl<<uuid<<endl;
            context.setUid(uid);

            bool found = context.load_by_uid();

            if (found && context.getUuid().compare(uuid)==0) {
                context.port=CallAggregator::getPortFromPool();
                //CallAggregator *session1 =new CallAggregator(port);


                string reg_ok_command(LOGIN_OK+std::to_string(context.port)+"\r\n");
                //context.writeToClient(reg_ok_command);
                cout<<"user logged "<<uid<<" port "<<context.port<<endl;

                if(LoggedUsersMap::getInstance().addUser(context)){

                    try {
                        context.writeToClient(reg_ok_command, 0);
                        cout<<"User added to map"<<endl;
                    }catch (exception& e){
                        cerr<<"fatal error when adding user to map"<<endl;

                    }
                }else{
                    cout<<context.getUid()<<" is already logged in. closing sockets"<<endl;
                    shared_ptr<User> old_user;
                    try {
                        old_user = LoggedUsersMap::getInstance().popUser(context.getUid());
                        old_user->closeSockets(); //

                    }catch (exception& e){//out_of_range
                        cout<<e.what()<<endl;
                    }

                    while(!old_user.unique()){// wait till socket closes and deletes their shared_ptrs to the user
                        std::this_thread::sleep_for(std::chrono::milliseconds(50));
                    }

                    bool added=LoggedUsersMap::getInstance().addUser(context);
                    if(added){
                        cout<<"user added to map"<<endl;
                        context.writeToClient(reg_ok_command, 0);
                    } else{
                        cerr<<"fatal err when adding user to map"<<endl;
                    }
                }
                //context.setCurrent_state(InitalState::getInstance());

            }else{
                cout<<uid<<" needs re registration"<<endl;
                context.writeToClient(REG_ERROR, 0);
            }

        }catch (std::bad_exception &e) {
            context.writeToClient(PROCESSING_ERROR, 0);
            cout << e.what() << endl;
        } catch (std::invalid_argument &e) {
            context.writeToClient(PROCESSING_ERROR, 0);
            cout << e.what() << endl;
        } catch (exception& e) {
            cout << "error Login unknown expt" <<e.what()<<endl;
        }
    }else if (data.compare(0, 3, "cnr") == 0 || data.compare(0, 3, "kcs") == 0){
        context.setCurrent_state(CallSetuptState::getInstance());
        throw std::invalid_argument("forwarded to CallSetup");
    }

}