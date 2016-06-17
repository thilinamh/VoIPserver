/* 
 * File:   User.cpp
 * Author: user
 * 
 * Created on December 11, 2015, 5:12 PM
 */
#define REMOTE_REJECT "rrj\r\n"
#include "User.hxx"
#include "iostream"
#include "../DBconnection.h"
#include <odb/transaction.hxx>
#include "User-odb.hxx"
#include "../Requests/RequestState/InitalState.h"
#include "../Requests/Behaviours/Community/BehaviourContainerCOMM.h"
#include "LoggedUsersMap.h"
#include "../Requests/RequestState/RegistrationState.h"
#include "../Requests/RequestState/VerificationState.h"
#include <odb/query.hxx>
#include "../SMSServerConnection.h"
#include "../Requests/RequestState/LoginState.h"
#include "../Requests/RequestState/InCallState.h"
#include "../CommandServerConnection.h"

using namespace std;

User::User(const StreamSocket &socket, BehaviourContainer *behaviourContainer) : _behaviours(behaviourContainer) {

    current_state = &InitalState::getInstance();
    tcpConnection = new CommandServerConnection(socket);// TCPServer owns this object
    tcpConnection->setHeartbeatParams(true,180,10,2); //sends keep alive packts after 2 mins
    smsConnection= nullptr;
    cout << "user created" << endl;

}

User::User(const User &orig) {

}


User::~User() {

    if (this->tcpConnection) { // this condition is to check weather this is not a ODB tempory user obj

       //This should be shifted to Concrete users, use strategy pattern
        bool r = LoggedUsersMap::getInstance().removeUser(this->uid_);
        if (r) {
            cout << this->uid_ << " removed from map" << endl;
        }

    }

    if (*(this->current_state) == RegistrationState::getInstance() ||
        *(this->current_state) == VerificationState::getInstance()) {

        if (this->uid_.size() > 0 && this->uuid.size() > 0 && (this->save() || this->update_db())) {
            cout << "state saved UID : " << this->uid_ << "UUID : " << this->uuid << endl;
        } else {
            cout << "state saving failed" << endl;
        }

    }

    cout << "User deleted " << uid_<<endl;
}

ServerConnection &User::getServerConnection() const {

    return *tcpConnection;
}

void User::bindWithServer() {

    // Before calling shared_from_this(),
    // there should be at least one std::shared_ptr p that owns *this
    tcpConnection->setContext(shared_from_this());
}


const string &User::getUid() const {
    return this->uid_;
}

void User::setUid(const string &uid) {
    this->uid_ = uid;

}

const string &User::getUuid() const {
    return this->uuid;
}

void User::setUuid(const string &uuid) {
    this->uuid = uuid;
}

bool User::save() {
    odb::database &db = DBconnection::getDb();
    odb::transaction t(db.begin());
    try {
        db.persist(*this);
        t.commit();
        cout << "user saved" << endl;
    } catch (const odb::object_already_persistent &e) {
        cout << "user already exist" << endl;
        t.rollback();
        return false;
    } catch (const odb::exception &e) {
        cerr << e.what() << endl;
        return false;
    }
    return true;

}

bool User::load_by_uid() {
    odb::database &db = DBconnection::getDb();
    odb::transaction t(db.begin());


    try {
        User a;
        //unique_ptr<User> jane(db.load<User>(uid_));
        db.load(this->uid_, a);
        this->uuid = a.uuid;

        //db.reload(this);
        t.commit();
        cout << "user loaded sucessfully " << " uuid : " << this->uuid << endl;
    } catch (const odb::object_not_persistent &e) {
        cout << "user does not exist" << endl;
        t.rollback();
        return false;
    } catch (odb::exception &e) {
        cerr << e.what() << endl;
        t.rollback();
        return false;
    }
    return true;

}

bool User::load_by_uuid(const string &uuid) {
    odb::database &db = DBconnection::getDb();
    odb::transaction t(db.begin());

    typedef odb::query<User> query;
    typedef odb::result<User> result;

    //query q (query::uuid == query::_ref (this->uuid));
    bool a = db.query_one<User>(query::uuid == query::_ref(uuid), *this);
    t.commit();

    return a;

}

void User::process(const string &data) {
    this->current_state->processRequest(data, *this);
}

void User::setCurrent_state(State &state) {
    this->current_state = &state;
}

/*
BehaviourContainer &User::getBehaviours() {
    cout<<"In user get behaviour"<<endl;
    _behaviours.reset(new BehaviourContainerCOMM());
    return *_behaviours;
}
*/

bool User::registerUser(const string &decrpted_data) {
    return this->_behaviours->getRegistrationBehaviour().registerUser(uid_, uuid, decrpted_data);
}

bool User::verifyRegistration(const string &verification_num) {
    return this->_behaviours->getVerificationBehaviour().verify(verification_num, this->uuid);
}


int User::writeToClient(const string &command, int timeout) {
//    unique_ptr<thread>  t(new thread(tcpConnection->writeToSocket, const_cast<string&>(command)));
   // write_thread.thread(tcpConnection->writeToSocket, const_cast<string&>(command));

    if(tcpConnection){
        cout<<"writing to socket :"<<command<<endl;
        return this->tcpConnection->writeToSocket(command, timeout);

    }else{
        throw Poco::NullPointerException("tcpSocket null");
    }

}
int User::writeToClientBlocking(const string &command, int timeout) {
    if(tcpConnection){
        return this->tcpConnection->writeToSocketBlocking(command, timeout);

    }else{
        throw Poco::NullPointerException("tcpSocket null");
    }
}



bool User::update_db() {
    odb::database &db = DBconnection::getDb();
    odb::transaction t(db.begin());
    try {
        User temp;
        temp.uid_ = this->uid_;
        temp.uuid = this->uuid;
        db.update(temp);
        t.commit();
        cout << "user updated" << endl;
    } catch (const odb::object_not_persistent &e) {
        cout << this->uid_ << " does not exist" << endl;
        t.rollback();
        return false;
    } catch (const odb::exception &e) {
        cerr << e.what() << endl;
        return false;
    }
    return true;
}

void User::setSession(shared_ptr<CallAggregator> &session) {

    this->session.reset(); // remove any previous sessions
    this->session=session;
    //this->session->registerObserver(shared_from_this());


}

void User::setSmsConnection(SMSServerConnection *smsConnection) {
    this->smsConnection = smsConnection;
}

void User::setMypubKey(const string &mypubKey) {
    this->mypubKey = mypubKey;
}

void User::update() {
cout<<this->uid_<<" called by call aggregator"<<endl;
}

shared_ptr<CallAggregator> &User::Session()  {
    return session;
}

 string &User::getMypubKey()  {
        return mypubKey;

}

void User::update(const std::string &message) {
    writeToClient(message, 0);
    cout<<message<<" sent to "<<uid_<<endl;
    //if(message==REMOTE_REJECT)
}

void User::onSessionExit() {

    session= nullptr; // release pointer
    cout<<"session removed from "<<uid_<<endl;
   // string rjct{0x19,0x91,0x01,0x05};
    string rjct("rrj\r\n");
    if(current_state==&InCallState::getInstance()){
        //rjct=(char)0x19;
    }
    writeToClient(rjct, 0); //REMOTE_REJECT
    current_state=&LoginState::getInstance();
}

bool User::onUDPSessionStarted() {
    if(current_state != &LoginState::getInstance()){
        current_state=&InCallState::getInstance();
        return true;
    }
    return false;
}

void User::onUnregisterFromObserver() {
    /*if(session.get()){
        cout<<"session reseted in "<<uid_<<endl;
        session.reset();
    }*/
}


void User::closeSockets() {
    if (tcpConnection) {//tcp connection will itself close the smsConnection
        tcpConnection->closeSocketConnection();
    }

}

void User::onCommandConnectionLost() { // called when tcpConnection lost
    std::lock_guard<std::mutex> lock(connection_close_mtx);// prevents parallel calls from sms connection and command connection
    bool r = LoggedUsersMap::getInstance().removeUser(this->uid_);
    if (r) {
        cout << this->uid_ << " removed from map" << endl;
    }
    if(tcpConnection){
        tcpConnection= nullptr;
    }if (smsConnection) {
        smsConnection->closeSocketConnection();
    }

}
void User::onSMSconnectionLost() {
    std::lock_guard<std::mutex> lock(connection_close_mtx);// prevents parallel calls from sms connection and command connection
    if(smsConnection) {
        smsConnection = nullptr;
    }
}
const State * User::getCurrentState() const {
    return current_state;
}



