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

    static int getPortFromPool();


    CallAggregator();

    void startUDP();

    bool wait_for_receiver(int timeout);

    void notify_caller();

    void interrupt_caller();

    string registerAsCaller(User &caller);

    string registerAsReceiver(shared_ptr<User> receiver);

    void unregisterObserver(shared_ptr<Observer> observer) override;

    User &getCaller();

    User &getReceiver();

    ~CallAggregator();

private:

    static unordered_set<int> ports_pool;

    static unordered_set<int> createPortPool();

    std::mutex mtx;
    condition_variable cv;

    bool ready = false;
    unique_ptr<Poco::Exception> exception_ptr1;
    weak_ptr<User> caller;
    weak_ptr<User> receiver;





    list<SocketAddress> receivers;

    mutex mtx_data_ready;



    class SocketConnection : public Poco::Runnable {
    public:
        DatagramSocket _dgramSocket;
        int _port;
        SocketAddress peerAddress;
        vector<SocketAddress> receiversList;
        Poco::Thread _thread;
        bool shouldExit;

        SocketConnection(int port,CallAggregator* aggregator);

        SocketConnection(SocketConnection&& source); //move constructor
        void fetchPeerAddress();
        void run() override;

        ~SocketConnection();
        bool operator ==(const SocketConnection& obj){
            return (this->_dgramSocket==obj._dgramSocket) && (_port == obj._port);
        };
        bool operator !=(const SocketConnection& obj){
            return !(this->_dgramSocket==obj._dgramSocket && _port == obj._port);
        };

        SocketConnection& operator=(SocketConnection&& source);




    private:
        CallAggregator *_aggregator;
    };


    list<unique_ptr<SocketConnection>> socketConnectionList;

    void createSocketConnection(int port);


protected:
    virtual string getSessionKey();


};


#endif //POCOSERVER_CALLAGGREGATOR_H
