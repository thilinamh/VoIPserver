//
// Created by tm on 26/01/16.
//

#ifndef POCOSERVER_CALLAGGREGATOR_H
#define POCOSERVER_CALLAGGREGATOR_H



//#include <thread>
#include <iostream>
#include <condition_variable>
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "ObserverPatternBase/Subject.h"
#include <unordered_set>
#include <forward_list>
#include <deque>

using namespace Poco::Net;

class User;

class CallAggregator : public Subject {

public:
    int peerCount = 2;

    CallAggregator();

    void startUDP();

    bool wait_for_receiver(int timeout);

    void notify_caller();

    void interrupt_caller();

    void registerAsCaller(shared_ptr<User> caller);

    void registerAsReceiver(shared_ptr<User> receiver);

    void unregisterObserver(shared_ptr<Observer> observer) override;

    User &getCaller();

    User &getReceiver();

    string getSessionKey();

    int getPortOf(User& client);

    ~CallAggregator();

private:

    static unordered_set<int> ports_pool;
    int getPortFromPool();

    static unordered_set<int> createPortPool();

    std::mutex mtx;
    condition_variable cv;

    bool ready = false;
    unique_ptr<Poco::Exception> exception_ptr1; // exception ptr holding interrupt exception
    weak_ptr<User> caller;
    weak_ptr<User> receiver;

    mutex mtx_data_ready;



    class SocketConnection : public Poco::Runnable {

    public:

        DatagramSocket _dgramSocket;
        int _port;
        SocketAddress peerAddress; // holds the udp end point address of the client
        vector<SocketAddress> receiversList; // end point addresses of those who wants to listen to our client
        unordered_set<string> receiversStringAddrList;
        Poco::Thread _thread;
        bool shouldExit;
        shared_ptr<User> _client; // one who sends packets to this udp socket

        SocketConnection(CallAggregator *aggregator, shared_ptr<User> client);
        SocketConnection(SocketConnection&& source); //move constructor

        void fetchPeerAddress(); // fetches client's endpoint address
        void run() override;
        void switch_to_advancedReadingMode();
        void switch_to_normalReadingMode();
        ~SocketConnection();
       /* bool operator ==(const SocketConnection& obj){

            return (this->_dgramSocket==obj._dgramSocket) && (_port == obj._port);
        };
        bool operator !=(const SocketConnection& obj){
            return !(this->_dgramSocket==obj._dgramSocket && _port == obj._port);
        };*/

        SocketConnection& operator=(SocketConnection&& source);




    public:
        class Listner{
        public:

            Listner(SocketConnection *_conn) ;
            //void setUdpConnection(SocketConnection *_conn);
            virtual void read(void *buff) =0;

        protected:
            const int BUFFERSIZE = 514;
            SocketConnection *_udp_conn;
        };

    public:
        class NormalListner:public Listner{

        public:
            NormalListner(SocketConnection *_conn) ;

            virtual void read(void *buff) override;
        };

    public:
        class AdvancedListner:public Listner{

        public:
            AdvancedListner(SocketConnection *_conn) ;

            virtual void read(void *buff) override;

        private:
            SocketAddress current_socketAddress;
        };


    private:
        CallAggregator *_aggregator;
        Listner *_listner;
        NormalListner _normalListner;
        AdvancedListner _advancedListner;

        void exchangePunchPorts() const;
    };


    list<unique_ptr<SocketConnection>> socketConnectionList;

    void createSocketConnection(shared_ptr<User> client);





};


#endif //POCOSERVER_CALLAGGREGATOR_H
