//
// Created by tm on 23/01/16.
//

#include "LoggedUsersMap.h"

#include <typeinfo>

//shared_ptr<CallAggregator> LoggedUsersMap::session(new CallAggregator(15000));

shared_ptr<User> LoggedUsersMap::findUser(const string &id) { //throws out_of_range exception
    // if(logged_users.count(id)){// this should return 1 or 0
    Poco::ScopedReadRWLock lk(lock);
    try {
        weak_ptr<User> ref = _logged_users.at(id);
        return ref.lock();
    } catch (std::exception_ptr e) {
        rethrow_exception(e);
    }

}

bool LoggedUsersMap::addUser(User &user) {
    Poco::ScopedWriteRWLock lk(lock);
    string key = user.getUid();
    weak_ptr<User> value = user.shared_from_this();
    std::pair<string, weak_ptr<User>> element(key, value);
    auto result = _logged_users.insert(
            element); //http://www.cplusplus.com/reference/unordered_map/unordered_map/insert/
    return result.second;

}

bool LoggedUsersMap::removeUser(const string &uid) {

    Poco::ScopedWriteRWLock lk(lock);
    return _logged_users.erase(uid);

}

LoggedUsersMap::LoggedUsersMap() {
//session->startUDP();
}

shared_ptr<User> LoggedUsersMap::popUser(const string &id) {

    Poco::ScopedWriteRWLock lk(lock);
    if (_logged_users.count(id)) { //if user available
        auto iter = _logged_users.find(id);

        weak_ptr<User> user_wk_ptr = iter->second;
        _logged_users.erase(iter);

        return user_wk_ptr.lock();
    }else{
        throw out_of_range("failed to pop, User "+id+" not in map");
    }

}
