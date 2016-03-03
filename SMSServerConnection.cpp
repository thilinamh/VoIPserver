//
// Created by user on 1/28/16.
//

#include "SMSServerConnection.h"
#include "iostream"
#include "UserStructure/User.hxx"
using namespace std;

SMSServerConnection::SMSServerConnection(const StreamSocket &client): ServerConnection(client) {

}

void SMSServerConnection::run() {
    bool exit = false;
    executionThread=Poco::Thread::current();
    //setHeartbeatParams(1, 3, 1); // if peer lost it's connection to the server, socket will close automatically
    // distructor will be called when closing the socket

    while (!exit) {

        const short BUF_SIZE = 2049;
        string buffer(BUF_SIZE, 0);
        // BufferedStreamBuf b(std::streamsize(5),OSin));

        int bytesReceived = this->socket().receiveBytes(&buffer[0], BUF_SIZE - 1);
        buffer.resize(bytesReceived);
        std::cout << bytesReceived << " bytes received : ";
        std::cout << buffer << endl;

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
        if (bytesReceived == 0) {
            //exit = true;
            this->socket().close();
            std::cout << "No data" << endl;
            break;

        } else {
            try {

            } catch (std::invalid_argument &e) {
                cerr << e.what() << endl;

            }
        }

    }
}

SMSServerConnection::~SMSServerConnection() {

    cout<<"SMSServerConnection distructor"<<endl;
    context->onSMSconnectionLost();
}


