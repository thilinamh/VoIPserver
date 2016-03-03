#include "Poco/ThreadPool.h"
#include "Poco/Runnable.h"
#include "Poco/Foundation.h"
#include <Poco/Net/TCPServer.h>
#include <iostream>
#include <memory>
#include "ServerConnection.h"
#include <Poco/Net/TCPServerConnectionFactory.h>
#include <thread>
#include "ServerConnectionFactory.h"


#include "UserStructure/User.hxx"
#include "UserStructure/User-odb.hxx"
#include "DBconnection.h"
#include "SMSServerConnectionFactory.h"
#include <signal.h>
using Poco::Net::TCPServer;
using Poco::Net::TCPServerConnectionFactory;
using namespace std;


int main(int argc, char** argv)
{
    DBconnection::getDb();

        try{
            /*The server takes ownership of the TCPServerConnectionFactory
             and deletes it when it's no longer needed.*/


            TCPServer clientTCP_listner {new ServerConnectionFactory() ,7003};
            //clientTCP_listner.socket().setReusePort(true);
            clientTCP_listner.start();
            cout<<"Command Listner : "<<clientTCP_listner.port()<<endl;

            TCPServer sms_listner {new SMSServerConnectionFactory() ,7004};
            sms_listner.start();
            cout<<"SMS Listner : "<<sms_listner.port()<<endl;
            //Poco::Thread::current()->join();
            signal(SIGPIPE,SIG_IGN); // ignore signals when writing to half opened sockets for process. Uses SO_ERROR instead
            while(1) {
                this_thread::sleep_for(chrono::milliseconds(1));
                //should use a conditonal_variable
            }
    }catch(Poco::IOException e){
        cout<<e.displayText()<<endl;
    }




    return 0;
}