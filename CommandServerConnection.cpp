//
// Created by user on 2/19/16.
//

#include "CommandServerConnection.h"
#include "UserStructure/User.hxx"
void CommandServerConnection::run() {
    exit = false;
    executionThread=Poco::Thread::current();
    //catch_signal(SIGPIPE,interruptHandler);
    this->socket().setSendTimeout(Poco::Timespan (2, 0));
    int sockets[2];
    if (socketpair(AF_INET, SOCK_DGRAM, 0, sockets) != -1){
        cerr<<"failed"<<endl;
    }

    //setHeartbeatParams(1, 3, 1); // if peer lost it's connection to the server, socket will close automatically
                                     // distructor will be called when closing the socket
    //cerr<<this_thread::get_id()<<" "<<socket().peerAddress().toString()<<endl;
    while (!exit) {

        const short BUF_SIZE = 2049;
        std::string buffer(BUF_SIZE, 0);
       // BufferedStreamBuf b(std::streamsize(5),OSin));

        int bytesReceived = this->socket().receiveBytes(&buffer[0], BUF_SIZE-1);
        buffer.resize(bytesReceived);
        std::cout << bytesReceived << " bytes received : " ;
        //std::cout << buffer <<endl;

        {   /*
            std::stringstream duf;
            duf << buf;
            std::cout << duf.str() << endl;
            r.decryptRequest(duf);
            */

        }
        /*
         cEz2DFPoJr2UyWRRxptjas66mBw3UHnpBknMJ9bxB7Yx6Nm7lsnaSxs6RCAVJjzZrP7UKhFwpvBnJjzJ0WW8cuAMhzyEtFJppiUvdL6ZJWp8YqqJiXwucC8huaz5g0fAnS7CidhWq1Cv9WJP6jBVI5zrFUmzghSWLEVRp3yvMcs5qAxbsDhKr2S2xcfqNs/+RmpwJD1ACbLfH/bgMkQhSDk/gCkdLG+pbDb5Pp40Q0Vyge0mwjVR1m3DQfndCFJ8vR+qvUIZwFExNArEqhCvo5J/XfG33bffmH8vivS2C17nmOJ4yT14+GBKRi1b0xIp5gDRuBFOgqjB5zas+7Zuyg==
         Czr2v73+W5tDG38dDcAwAixlZl28w11BghNBSnShGjmDVuez472bb7Vcup9MK9ihhEM0Uq7T6ICjwRIPdSgLnH48ORPiXhjl+Ezb0MnhfdhoYrJqJ+dIX3R5c9/l/if7pOKEAPeT4KA4sdREr5AuOKxcK8ROjFA0tVbHGr6XYt7OWmhfXYntCq52Aqmk4qD/DfwyfWTUu0dpT8tiZkYYo7LWpM6TYCN3BvCPIBYvSDL1PWxPEZkoOlMPF/4L7bg3XZF+qUzAle5agNi951Zvk+jZ+BFx0KC790LuViQcZl97lJEmcBV5rGwuHDzWAh8pUTX9YT+w31MI8NhozzURIg==
         * */
        if (bytesReceived ==0) {
            //exit = true;
            //this->socket().close();
            std::cout << "No data" << std::endl;
            exit= true;
            break;
        }else if(bytesReceived<4){//ex:-rrq\n\r
            std::cout << "Not enough args" << std::endl;
            continue;
        } else {
            try {
                context->process(buffer);
            } catch (std::invalid_argument &e) {
                std::cerr << e.what() << std::endl;
                context->process(buffer);
            }catch(std::exception& e){
                std::cerr << "Fatal error " << e.what() << std::endl;

            }
        }

    }
    std::cerr << "read socket finished " << std::endl;

}

CommandServerConnection::CommandServerConnection(const StreamSocket &client):ServerConnection(client) {

}

CommandServerConnection::~CommandServerConnection() {
    std::cout << "CommandServerConnection distructor " << context->getUid()<<std::endl;
    context->onCommandConnectionLost();
    //when tcp server connection lost, it releases the context shared_ptr of SMSServer connection
    /*if(context.get()) { // This condition is to avoid SMSServer connection object to prevent this call
        std::cout << "ServerConnection distructor " << context->getUid() << " called from Thread " <<
        this_thread::get_id() << endl;
        if (context->tcpConnection != nullptr && context->tcpConnection == this) {
            context->onConnectionLost();


        }
    }*/

}
