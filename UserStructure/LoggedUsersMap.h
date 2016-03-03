//
// Created by tm on 23/01/16.
//

#ifndef POCOSERVER_LOGGEDUSERS_H
#define POCOSERVER_LOGGEDUSERS_H


#include <unordered_map>
#include <algorithm>
#include <memory>
#include "User.hxx"
#include "../Helper/Singleton.h"

#include "Poco/RWLock.h"


using namespace std;
using namespace Poco::Net;
using namespace Poco;

class LoggedUsersMap : public Singleton<LoggedUsersMap> {

public:
     bool addUser( User& user);
     shared_ptr<User> findUser(const string& id); //throws out_of_range exception
     shared_ptr<User> popUser(const string& id); //throws out_of_range exception
     bool removeUser(const string& uid );

     //static  shared_ptr<CallAggregator> session ;
private:
    friend class Singleton;
    Poco::RWLock lock; //http://pocoproject.org/docs/Poco.RWLock.html
    LoggedUsersMap();
    LoggedUsersMap(const LoggedUsersMap & src)=delete;
    LoggedUsersMap & operator=(const LoggedUsersMap & rhs)=delete;

    unordered_map<string,weak_ptr<User>> _logged_users;
    //We are not allocating memeory from heap for the map
    //Because we do not need much memory since we only
    //keep pointers to Users


};


#endif //POCOSERVER_LOGGEDUSERS_H
