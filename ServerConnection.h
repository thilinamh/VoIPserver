/* 
 * File:   TCPconnection.h
 * Author: user
 *
 * Created on December 9, 2015, 12:31 PM
 */

#ifndef TCPCONNECTION_H
#define	TCPCONNECTION_H
#include "Poco/Net/TCPServerConnection.h"
#include <Poco/Net/StreamSocket.h>
#include <memory>
#include "Poco/StringTokenizer.h"
#include <iostream>
#include <mutex>

class User;
using Poco::Net::TCPServerConnection;
using Poco::Net::StreamSocket;

class ServerConnection : public TCPServerConnection {
public:
    /** called by TCPserver
     *ServerConnection(const ServerConnection& orig);
     */
    ServerConnection(const StreamSocket &client);

    /** TCPServer will invoke distructor when connection ended.
     * Therefore do not delete the object by yourself
     */

    virtual  ~ServerConnection();
    virtual void run() = 0;
    virtual void getSockError();

    /**
     *
     * @param idle_time_from_last_packet
     *       :The time (in seconds) the connection needs to remain
             idle before TCP starts sending keepalive probes (TCP_KEEPIDLE socket option)
     * @param ack_packets
     *  :The maximum number of keepalive probes TCP should
             send before dropping the connection. (TCP_KEEPCNT socket option)
     * @param ack_interval
     * :The time (in seconds) between individual keepalive probes.
            (TCP_KEEPINTVL socket option)
     *
     */
    virtual void setHeartbeatParams(bool enable, int idle_time_from_last_packet=0, int ack_packets=0, int ack_interval_seconds=0) final;

    virtual void setContext (std::shared_ptr<User> context) final;

    virtual int writeToSocket(const std::string &message, int timeout) final;//millis
    virtual int writeToSocketBlocking(const std::string &message, int timeout) final; //millis
    virtual void join_execution_Thread();
    void closeSocketConnection();


protected:

    std::shared_ptr<User> context;
    Poco::Thread* executionThread;
    bool exit;

private:
    int catch_signal(int sig, void (*handler)(int));

    std::mutex write_mutx;


};

#endif	/* TCPCONNECTION_H */

