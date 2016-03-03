/* 
 * File:   TCPconnection.cpp
 * Author: user
 * 
 * Created on December 9, 2015, 12:31 PM
 */

#include "ServerConnection.h"
#include "UserStructure/User.hxx"
#include "ClientRequest.h"
#include "Helper/Crypto.h"
#include <signal.h>
#include <Poco/BufferedStreamBuf.h>

using Poco::BufferedStreamBuf;
using namespace std;
using namespace std::chrono;

void handle(int c);

ServerConnection::ServerConnection(const StreamSocket &client) : TCPServerConnection(client) {
    std::cout << "Server connection constuctor" << client.peerAddress().toString() << endl;


}

void ServerConnection::setContext(shared_ptr<User> context) {
    std::cout << "user assigned to socket : " << context->getUid() << endl;
    this->context = context;
    // this assignement is valid until context is a shared_ptr;
    // if you receive a raw ptr, you need to use reset(raw_ptr) method

}


ServerConnection::~ServerConnection() {

}

void ServerConnection::setHeartbeatParams(bool enable,int idle_time, int ack_packets, int ack_interval_seconds) {


/*
    this->socket().setKeepAlive(enable);
    this->socket().setReusePort(false);

    this->socket().setOption(IPPROTO_TCP, TCP_KEEPIDLE,idle_time);

    this->socket().setOption(IPPROTO_TCP, TCP_KEEPCNT,ack_packets);

    this->socket().setOption(IPPROTO_TCP, TCP_KEEPINTVL,ack_interval_seconds );
  */

}

int ServerConnection::writeToSocket(const std::string &message, int timeout) {
    std::lock_guard<std::mutex> lock(write_mutx);
    //this->socket().setOption(SOL_TCP,TCP_USER_TIMEOUT,timeout);

    int msgSize = static_cast<int>(message.size());
    timeval t{6, 0};

    socket().impl()->setRawOption(SOL_SOCKET, SO_SNDTIMEO, (void *) &t, sizeof(t));

    /*for(char c:message) {
        socket().sendUrgent(c);
    }*/
    auto sent = this->socket().sendBytes(message.c_str(), msgSize - 1);

    int val;
    socket().getOption(SOL_SOCKET, SO_SNDTIMEO, val);

    return sent;
}

int ServerConnection::writeToSocketBlocking(const std::string &message, int timeout) {
    std::lock_guard<std::mutex> lock(write_mutx);
    int msgSize = static_cast<int>(message.size());

    auto sent = this->socket().sendBytes(message.c_str(), msgSize - 1);

    tcp_info info;
    poco_socklen_t size = sizeof(info);
    auto expireAt = system_clock::now() + milliseconds(timeout);

    do {
        this_thread::sleep_for(milliseconds(50));
        socket().impl()->getRawOption(SOL_TCP, TCP_INFO, (void *) &info, size);

    } while (info.tcpi_unacked > 0 && expireAt > system_clock::now());//wait till all packets acknowledged or time expires

    if(info.tcpi_unacked>0) {
        cerr << "unacked packets :" << info.tcpi_unacked << endl;
        return -1;
    }

    return sent;
}

void ServerConnection::closeSocketConnection() {
    cout << context->getUid() << " sock closing" << endl;
    this->socket().shutdown();
}

void handle(int c) {
    cerr << "err " << c << endl;
}

int ServerConnection::catch_signal(int sig, void (*handler)(int)) {
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    return sigaction(sig, &action, NULL);
}


void ServerConnection::getSockError() {
    int err = 0;
    this->socket().getOption(SOL_SOCKET, SO_ERROR, err);
    cerr << "socket msg of " << context->getUid() << " :" << strerror(err) << endl;

}


void ServerConnection::join_execution_Thread() {
    executionThread->join();
}
