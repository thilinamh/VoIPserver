/* 
 * File:   User.h
 * Author: user
 *
 * Created on December 11, 2015, 5:12 PM
 */

#ifndef USER_H
#define	USER_H
#include <string>
#include <memory>
#include <odb/core.hxx>
#include <thread>
#include "../ServerConnection.h"
#include "Poco/Net/StreamSocket.h"
#include "../Requests/RequestState/State.h"
#include "../Requests/Behaviours/RegistrationBehavior.h"
#include "../Requests/Behaviours/BehaviourContainer.h"
#include "../Helper/CallAggregator.h"


using Poco::Net::StreamSocket;
using namespace std;


class ServerConnection;
class SMSServerConnection;
class BehaviourContainer;
class User: public enable_shared_from_this<User> ,public Observer{
    friend  class odb::access;
public:

    User(const StreamSocket &socket, BehaviourContainer *behaviourContainer);
    User(const User& orig);
    virtual ~User(); // essential for polymorphic behaviour
    /**
     * tcpConnection will be given the current User object
     * */
    virtual void bindWithServer();
    void setSmsConnection(SMSServerConnection *smsConnection) ;
    ServerConnection& getServerConnection() const;
    virtual void onCommandConnectionLost();
    virtual void onSMSconnectionLost();
    int writeToClient(const string &command, int timeout=3000);//millis
    int writeToClientBlocking(const string &command, int timeout=3000);//millis
    void closeSockets();

public:
    const string & getUid() const;
    string & getMypubKey();
    void setUid(const string &uid);

    const string &getUuid() const;
    void setMypubKey(const string &mypubKey) ;
    void setUuid(const string &uuid);
    bool registerUser(const string &decrpted_data);
    bool verifyRegistration(const string &verification_num);
    bool login();
    virtual bool update_db();

    virtual void update();

    virtual bool onUDPSessionStarted();

    virtual bool save();
    virtual bool load_by_uid();
    virtual bool load_by_uuid(const string &uuid);
    void process(const std::string& );
    void setCurrent_state(State &state) ;
    const State * getCurrentState() const;
    //virtual BehaviourContainer & getBehaviours();


    void setSession(shared_ptr<CallAggregator> &session);


    virtual void onUnregisterFromObserver();

    virtual void onSessionExit();

    virtual void update(const std::string &message);

    shared_ptr<CallAggregator> &Session() ;
    shared_ptr<CallAggregator> &Session() const;

    int port;
private:

    #pragma db id
    std::string uid_;

    std::string uuid;

    std::string mypubKey;

    State* current_state; //do not own the object

    shared_ptr<CallAggregator> session;

    std::thread write_thread;

    std::mutex connection_close_mtx;


public:
    unique_ptr<BehaviourContainer> _behaviours;


protected:
    friend class ServerConnection;

    ServerConnection* tcpConnection; //do not own the object
    SMSServerConnection* smsConnection;//do not own the object
    User()=default; //reserved for odb


};
#pragma db object(User) polymorphic
#pragma db member(User::uuid) not_null
#pragma db member(User::_behaviours) transient
#pragma db member(User::tcpConnection) transient
#pragma db member(User::current_state) transient
#pragma db member(User::session) transient
// odb --std c++11 -d mysql --generate-query --schema-format embedded  --generate-schema User.hxx
#endif	/* USER_H */

