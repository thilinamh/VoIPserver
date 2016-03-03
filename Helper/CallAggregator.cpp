//
// Created by tm on 26/01/16.
//

#include "CallAggregator.h"
#include <iostream>
#include <thread>
#include <Poco/Net/SocketStream.h>
#include <Poco/Net/SocketImpl.h>
#include "../UserStructure/User.hxx"
using namespace std;

unordered_set<int> CallAggregator::ports_pool(createPortPool());

void CallAggregator::startUDP() {
    cout<<"sock size"<<socketConnectionList.size()<<endl;
    if(!(socketConnectionList.empty())) {

        for(auto observer_wk_ptr:observerList){// let every observer know about session started
            if(auto observer_shrd_ptr=observer_wk_ptr.lock()){
                observer_shrd_ptr->onUDPSessionStarted();
            }
        }

        for(auto& observerConnection:socketConnectionList){
            if(!observerConnection->_thread.isRunning()) {
                observerConnection->_thread.start(*observerConnection);
            }
        }


    }else{
        throw Poco::NotFoundException("socket connection list empty",0);
    }
}




CallAggregator::CallAggregator(): exception_ptr1( nullptr) {
    observerList.clear();
    cerr<<"Observer size "<<observerList.size()<<endl;

}

string CallAggregator::getSessionKey() {
    return "1234567895";
}

bool CallAggregator::wait_for_receiver(int timeout) {
    std::unique_lock<std::mutex> lck(mtx);
    if(exception_ptr1.get()) {
        exception_ptr1.reset(nullptr);// free previous exceptions
    }
    bool timeoutReached = false;
    auto now = std::chrono::system_clock::now();
    while (!ready) {
        if(cv.wait_until(lck,now + std::chrono::seconds(timeout))==cv_status::timeout){
            timeoutReached = true;
            break;
        }

    }
    cout<<"notifified"<<endl;
    if(exception_ptr1.get()){//if a interrupt has been set
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
    exception_ptr1.reset(new Poco::NullValueException("Caller Interrupted",0));
    cv.notify_all();
}
void CallAggregator::createSocketConnection(int port) {
    Poco::Net::SocketAddress sAdress(Poco::Net::IPAddress().toString(),port); //static_cast<Poco::UInt32>(port)
    unique_ptr<SocketConnection> ptr(new SocketConnection(port,this));
    socketConnectionList.push_back(move(ptr));
}

string CallAggregator::registerAsCaller(User &caller) {
    this->caller=caller.shared_from_this();

    for(auto& observer:observerList){
        if(observer.lock().get()==&caller){//if the caller is already in observer list
            return getSessionKey();//just return the session Key
        }
    }

    createSocketConnection(caller.port);
    return Subject::registerObserver(caller.shared_from_this());
}

string CallAggregator::registerAsReceiver(shared_ptr<User> receiver) {
    this->receiver= receiver;
    for(auto& observer:observerList){
        if(observer.lock().get()==receiver.get()){//if the receiver is already in observer list
            return getSessionKey();//just break the loop & return the session Key
        }
    }
    createSocketConnection(receiver->port);
    return Subject::registerObserver(receiver); //else reg as observer
}

User &CallAggregator::getReceiver() {

    return *(receiver.lock());
}

User &CallAggregator::getCaller() {
    return *(caller.lock());
}


void CallAggregator::unregisterObserver(shared_ptr<Observer> observer) {
    peerCount--;
    Subject::unregisterObserver(observer);
    if(observerList.size()<=1){//when only one peer left
        //shouldExit= true;
        Subject::exitSession();
        cerr<<"only one peer left. exitSession called"<<endl;
    }
}

CallAggregator::~CallAggregator() {


    std::cout<<"CallAggregator deleting "<<std::endl;
}


//------------------Static funcs---------------------------------------------
unordered_set<int> CallAggregator::createPortPool() {
    std::unordered_set<int> set;
    for(int i=10000;i<20001;i++){
        set.insert(i);
    }
    return set;
}

int CallAggregator::getPortFromPool() {
    auto node_iterator =ports_pool.erase(ports_pool.begin());
    return  *node_iterator;

}
/*-----------------------------Socket Connection Inner class--------------------------------------*/

CallAggregator::SocketConnection::SocketConnection(int port, CallAggregator* aggregator):_aggregator(aggregator) {
    Poco::Net::SocketAddress sAdress(Poco::Net::IPAddress().toString(),port); //static_cast<Poco::UInt32>(port)
    _dgramSocket.bind(sAdress, true);
    _dgramSocket.setReusePort(false);
    _dgramSocket.setOption(SOL_SOCKET,SO_NO_CHECK,1);//Disable the Checksum Computation
    _port=port;
    cerr<<"Sock port "<<_dgramSocket.address().port()<<endl;

}

void CallAggregator::SocketConnection::fetchPeerAddress() {
    cout << "socket " << _port << "In fetchAddress"<<endl;
    lock_guard<mutex> lck(_aggregator->mtx);
    cout << "port " << _port << " udp SocketConnection in action" << endl;

    SocketAddress sender;
    const int BUFFERSIZE=1440;
    char buff[BUFFERSIZE];

    for(int i=0;i<3;i++){// sample packets
        _dgramSocket.receiveFrom(buff,BUFFERSIZE,sender);
    }
    this->peerAddress=sender;

    for(auto&udp_session:_aggregator->socketConnectionList){
        if(udp_session.get() != this) {//only other udp_session
            //receiversList.push_back(udp_session->peerAddress); //take their peer address and push it in list
            udp_session->receiversList.push_back(sender); // add our peer Adress to their receiver list
        }
    }
}

void CallAggregator::SocketConnection::run() {


    //SocketAddress sender;
    const int BUFFERSIZE=514;
    char buff[BUFFERSIZE];
    fetchPeerAddress();
    cout<<peerAddress.toString()<<endl;
    int count=0;
    while(!shouldExit){
        //_reader->relayPackets(sender);
        //cout<<peerAddress.toString()<<" waiting"<<endl;
        auto data_len=_dgramSocket.receiveBytes(buff,BUFFERSIZE-1);
        //cout<<peerAddress.toString()<<" got data "<<endl;
        //cout<<peerAddress.toString()<<" data len"<<data_len<<endl;
        /*if(count<2){
            cout << peerAddress.toString() << " receiver list size :" << receiversList.size() << endl;
            count++;
        }*/
        for(SocketAddress& receiver: receiversList){
            _dgramSocket.sendTo(buff,data_len-1,receiver);
        }
    }

    cout<<"exit from thread "<<peerAddress.toString()<<endl;
    string exit_message("exit");
    _dgramSocket.sendTo(&exit_message[0], exit_message.size()-1,peerAddress);

}

CallAggregator::SocketConnection::~SocketConnection() {
    shouldExit= true;
    //_dgramSocket.impl()->shutdownReceive();
    shutdown(_dgramSocket.impl()->sockfd(),SHUT_RD);
    _thread.join();
    _dgramSocket.close();
    _aggregator->ports_pool.insert(_port);

}


CallAggregator::SocketConnection::SocketConnection(CallAggregator::SocketConnection &&source):
        _dgramSocket(move(source._dgramSocket))
        , peerAddress(move(source.peerAddress))
        , receiversList(move(source.receiversList)) {
    shouldExit=source.shouldExit;
    _port =source._port;

}

CallAggregator::SocketConnection &CallAggregator::SocketConnection::operator=(
        CallAggregator::SocketConnection &&source) {
    _dgramSocket=move(source._dgramSocket);
    peerAddress=source.peerAddress;
    receiversList =move(source.receiversList);
    shouldExit=source.shouldExit;
    _port =source._port;

}
