//
// Created by user on 1/28/16.
//

#include "SMSServerConnectionFactory.h"
#include "SMSServerConnection.h"
#include "UserStructure/LoggedUsersMap.h"
#include "RSAKeys.h"

using namespace std;

TCPServerConnection *SMSServerConnectionFactory::createConnection(const StreamSocket &socket) {

    SMSServerConnection *smsConn=new SMSServerConnection(socket);
    smsConn->setHeartbeatParams(true,5,3,1);

    while (true) {

        const short BUF_SIZE = 2049;
        string buffer(BUF_SIZE, 0);
        // BufferedStreamBuf b(std::streamsize(5),OSin));

        int bytesReceived = const_cast<StreamSocket&>(socket).receiveBytes(&buffer[0], BUF_SIZE - 1);
        buffer.resize(bytesReceived);
        cout <<"\nSMS conn-> "<<bytesReceived << " bytes received : ";
        cout << buffer << endl;
        if(buffer.compare(0,3,"uid")==0){ //Add SMS connection to relevant user
            string uid= buffer.substr(4);
            if(!uid.empty() && (uid.at(uid.size()-1)=='\n' || uid.at(uid.size()-1)=='\r')){
                uid.pop_back();//remove \n or \r from uid
            }

            try {
                auto user = LoggedUsersMap::getInstance().findUser(uid);
                if(user.get()) {
                    user->setSmsConnection(smsConn);
                    smsConn->setContext(user);
                    string command("rgk "+string(RSA_PUB)+"\r\n");
                    const_cast<StreamSocket&>(socket).sendBytes(&command[0],command.size()-1);
                }

            }catch (const std::out_of_range& e){

                cout<<"Invalid user for SMS connection : "<<uid<<endl;
            }
            break;
        }
    }
    return smsConn;


   // return dynamic_cast<TCPServerConnection*>(&(usr->getServerConnection()));
}


