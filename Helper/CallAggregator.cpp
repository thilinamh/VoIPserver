//
// Created by tm on 26/01/16.
//

#include "CallAggregator.h"
#include <iostream>
#include <thread>
#include <Poco/Net/SocketStream.h>
#include <Poco/Net/SocketImpl.h>
#include "../UserStructure/User.hxx"

#define CONNECT "con "
using namespace std;

unordered_set<int> CallAggregator::ports_pool(createPortPool());

void CallAggregator::startUDP() {
    cout << "sock size" << socketConnectionList.size() << endl;
    if (!(socketConnectionList.empty())) {

        for (auto observer_wk_ptr:observerList) {// let every observer know that session started
            if (auto observer_shrd_ptr = observer_wk_ptr.lock()) {
                observer_shrd_ptr->onUDPSessionStarted();
            }
        }

        for (auto &udpConnection:socketConnectionList) {
            if (!udpConnection->_thread.isRunning()) {
                udpConnection->_thread.start(*udpConnection);
                udpConnection->_client->writeToClient(CONNECT + to_string(udpConnection->_port)+"\r\n");
            }
        }


    } else {
        throw Poco::NotFoundException("socket connection list empty", 0);
    }
}


CallAggregator::CallAggregator() : exception_ptr1(nullptr) {
    observerList.clear();
    cerr << "Observer size " << observerList.size() << endl;

}

string CallAggregator::getSessionKey() {
    return "1234567895";
}

bool CallAggregator::wait_for_receiver(int timeout) {
    std::unique_lock<std::mutex> lck(mtx);
    if (exception_ptr1.get()) {
        exception_ptr1.reset(nullptr);// free previous exceptions
    }
    bool timeoutReached = false;
    auto now = std::chrono::system_clock::now();
    while (!ready) {
        if (cv.wait_until(lck, now + std::chrono::seconds(timeout)) == cv_status::timeout) {
            timeoutReached = true;
            break;
        }

    }
    cout << "notifified" << endl;
    if (exception_ptr1.get()) {//if a interrupt has been set
        exception_ptr1.get()->rethrow();
    }
    // ...
    std::cout << "Thread " << std::this_thread::get_id() << " woken\n";
    //ready= false; // needed in static CallAggregator obj
    return timeoutReached;
}

void CallAggregator::notify_caller() {

    std::unique_lock<std::mutex> lck(mtx);
    ready = true;
    cv.notify_all();
}

void CallAggregator::interrupt_caller() {
    std::unique_lock<std::mutex> lck(mtx);
    ready = true;
    exception_ptr1.reset(new Poco::NullValueException("Caller Interrupted", 0));
    cv.notify_all();
}

void CallAggregator::createSocketConnection(shared_ptr<User> client) {


    unique_ptr<SocketConnection> ptr(new SocketConnection(this, client));
    socketConnectionList.push_back(move(ptr));
}

void CallAggregator::registerAsCaller(shared_ptr<User> caller) {
    this->caller = caller;

    for (auto &observer:observerList) {
        if (observer.lock().get() == caller.get()) {//if the caller is already in observer list
            return;//do nothing
        }
    }

    createSocketConnection(caller);
    Subject::registerObserver(caller);
}

void CallAggregator::registerAsReceiver(shared_ptr<User> receiver) {
    this->receiver = receiver;

    for (auto &observer:observerList) {
        if (observer.lock().get() == receiver.get()) {//if the receiver is already in observer list, that is impossible
            throw Poco::LogicException("Call waiting not supported");
        }
    }

    createSocketConnection(receiver);
    Subject::registerObserver(receiver); //else reg as observer

}

User &CallAggregator::getReceiver() {

    return *(receiver.lock());
}

User &CallAggregator::getCaller() {
    return *(caller.lock());
}


int CallAggregator::getPortOf(User &client) {
    for (auto &udpConn: socketConnectionList) {
        if (udpConn->_client.get() == &client) {
            return udpConn->_port;
        }
    }
    return -1;
}

void CallAggregator::unregisterObserver(shared_ptr<Observer> observer) {
    peerCount--;
    Subject::unregisterObserver(observer);
    if (observerList.size() <= 1) {//when only one peer left
        //shouldExit= true;
        Subject::exitSession();
        cerr << "only one peer left. exitSession called" << endl;
    }
}

CallAggregator::~CallAggregator() {


    std::cout << "CallAggregator deleting " << std::endl;
}


//------------------Static funcs---------------------------------------------
unordered_set<int> CallAggregator::createPortPool() {
    std::unordered_set<int> set;
    for (int i = 10000; i < 20001; i++) {
        set.insert(i);
    }
    return set;
}

int CallAggregator::getPortFromPool() {
    lock_guard<mutex> lck(mtx);
    auto node_iterator = ports_pool.erase(ports_pool.begin());
    return *node_iterator;

}

/*-----------------------------Socket Connection Inner class--------------------------------------*/

CallAggregator::SocketConnection::SocketConnection(CallAggregator *aggregator, shared_ptr<User> client)
        : _aggregator(aggregator), _client(client), _normalListner(this), _advancedListner(this) {

    _port = _aggregator->getPortFromPool();
    Poco::Net::SocketAddress sAdress(Poco::Net::IPAddress().toString(), _port); //static_cast<Poco::UInt32>(port)
    _dgramSocket.bind(sAdress, true);
    _dgramSocket.setReusePort(false);
    _dgramSocket.setOption(SOL_SOCKET, SO_NO_CHECK, 1);//Disable the Checksum Computation

    _listner =&_advancedListner;
    shouldExit= false;
    cerr << "Sock port " << _dgramSocket.address().port() << " for " << _client->getUid() << endl;

}

void CallAggregator::SocketConnection::fetchPeerAddress() {

    lock_guard<mutex> lck(_aggregator->mtx);
    if(_client) {

        cout <<_client->getUid() <<" socket " << _port << "In fetchAddress" << endl;

        //_client->writeToClient(CONNECT + _port);// tell mobile client to start sending voice data

        SocketAddress sender;
        const int BUFFERSIZE = 1440;
        char buff[BUFFERSIZE];

        //for (int i = 0; i < 3; i++) {// sample packets
        _dgramSocket.setReceiveTimeout(Poco::Timespan(5,0));
        _dgramSocket.receiveFrom(buff, BUFFERSIZE, sender);
        //}
        //this_thread::sleep_for(chrono::seconds(2));
        this->peerAddress = sender;

        exchangePunchPorts();

        cout << "socket " << _port << "done fetching Address" << endl;

    }
}

void CallAggregator::SocketConnection::exchangePunchPorts() const {

    for (auto &udp_session:this->_aggregator->socketConnectionList) {
            if (udp_session.get() != this) {//only if other udp_session

                auto &rem_client = udp_session->_client;
                if (rem_client.get()) {
                    cout << this->_client->getUid() << " writing to client" << endl;
                    //this_thread::sleep_for(chrono::seconds(2));
                    this->_client->writeToClientBlocking("pnh " + to_string(udp_session->_port) + "\r\n"); //tell our mobile client to send punch  packets to others udp sockets
                    //then others will start sending voice packets to  our mobile client
                    //udp_session->switch_to_advancedReadingMode(); //set their reading mode to advanced
                    if(this->_aggregator->socketConnectionList.size() > 2) {//only for a conference caller
                        //this->switch_to_advancedReadingMode(); // set our reading mode to advanced
                        rem_client->writeToClientBlocking("pnh " + to_string(this->_port) + "\r\n");//now tell others mobile clients to send punch data to our udp socket

                    }
                }
            }
        }
}

void CallAggregator::SocketConnection::run() {


    //SocketAddress sender;
    const int BUFFERSIZE = 514;
    char buff[BUFFERSIZE];
    try {
        fetchPeerAddress();
    }catch (exception& e){
        cerr<<"could not init "<<_client->getUid()<<" :"<<e.what()<<endl;
        return;
    }
    cout << peerAddress.toString() << endl;
    int count = 0;
    while (!shouldExit) {

        try {

            _listner->read(buff);
        }catch (Poco::Exception& e){
            cerr<<_client->getUid()<<" : "<<e.what()<<endl;
        }

    }

    cout << "exit from thread " << peerAddress.toString() << endl;
    string exit_message("exit");
    _dgramSocket.sendTo(&exit_message[0], exit_message.size() - 1, peerAddress);

}

CallAggregator::SocketConnection::~SocketConnection() {
    shouldExit = true;
    //_dgramSocket.impl()->shutdownReceive();
    shutdown(_dgramSocket.impl()->sockfd(), SHUT_RD);
    _thread.tryJoin(2000);
    _dgramSocket.close();
    _aggregator->ports_pool.insert(_port);
    cout<<"udp socket deleted "<<_port<<endl;

}


CallAggregator::SocketConnection::SocketConnection(CallAggregator::SocketConnection &&source) :
        _dgramSocket(move(source._dgramSocket)), peerAddress(move(source.peerAddress)),
        receiversList(move(source.receiversList)), _normalListner(this), _advancedListner(this){

    shouldExit = source.shouldExit;
    _port = source._port;

}

CallAggregator::SocketConnection &CallAggregator::SocketConnection::operator=(
        CallAggregator::SocketConnection &&source) {
    _dgramSocket = move(source._dgramSocket);
    peerAddress = source.peerAddress;
    receiversList = move(source.receiversList);
    shouldExit = source.shouldExit;
    _port = source._port;

}

void CallAggregator::SocketConnection::switch_to_advancedReadingMode() {
    _listner =&_advancedListner;
}

void CallAggregator::SocketConnection::switch_to_normalReadingMode() {
    _listner =&_normalListner;
}


//------------------------------Listner classes-----------------


void CallAggregator::SocketConnection::NormalListner::read(void *buff) {
    //cout<<"In normal listner mode "<<endl;
    auto data_len = _udp_conn-> _dgramSocket.receiveBytes(buff, BUFFERSIZE - 1);
    //cout<<peerAddress.toString()<<" got data "<<endl;
    //cout<<peerAddress.toString()<<" data len"<<data_len<<endl;
    /*if(count<2){
        cout << peerAddress.toString() << " receiver list size :" << receiversList.size() << endl;
        count++;
    }*/
    for (SocketAddress &receiver:_udp_conn-> receiversList) {
        _udp_conn->_dgramSocket.sendTo(buff, data_len - 1, receiver);
    }
}

void CallAggregator::SocketConnection::AdvancedListner::read(void *buff) {
    //cout<<"In advanced listner mode "<<endl;
    auto data_len = _udp_conn-> _dgramSocket.receiveFrom(buff, BUFFERSIZE - 1, current_socketAddress);

    if(current_socketAddress != _udp_conn->peerAddress){
        auto result=_udp_conn->receiversStringAddrList.insert(current_socketAddress.toString());
        if(result.second){ // if successfully added
            cout<<_udp_conn->_client->getUid()<<" from port :"<<_udp_conn->_port<<" ";
            cout<<"incoming listner detected "<<current_socketAddress.toString()<<endl;
            _udp_conn->receiversList.push_back(current_socketAddress);
            //_udp_conn->switch_to_normalReadingMode();
        }

    }
    //cout<<current_socketAddress.toString()<<endl;
    //cout<<peerAddress.toString()<<" got data "<<endl;
    //cout<<peerAddress.toString()<<" data len"<<data_len<<endl;
    /*if(count<2){
        cout << peerAddress.toString() << " receiver list size :" << receiversList.size() << endl;
        count++;
    }*/
    for (SocketAddress &receiver:_udp_conn-> receiversList) {
        _udp_conn->_dgramSocket.sendTo(buff, data_len - 1, receiver);
    }
}


CallAggregator::SocketConnection::Listner::Listner(CallAggregator::SocketConnection *conn) {

    if (conn) {
        _udp_conn = conn;
    } else {
        throw invalid_argument("null udp connection");
    }
}

CallAggregator::SocketConnection::NormalListner::NormalListner(CallAggregator::SocketConnection *_conn):Listner(_conn) {

}

CallAggregator::SocketConnection::AdvancedListner::AdvancedListner(CallAggregator::SocketConnection *_conn):Listner(_conn) {

}

