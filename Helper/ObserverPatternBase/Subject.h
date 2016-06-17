//
// Created by user on 2/3/16.
//

#ifndef POCOSERVER_SUBJECT_H
#define POCOSERVER_SUBJECT_H

#include <iostream>
#include <memory>
#include <algorithm>    // std::find_if
#include <list>
#include "Observer.h"
using namespace std;
class Observer;
class Subject {
public:
    //returns the sharedKey

    virtual void notifyAll();
    virtual void sendToAll(const string &command);
    virtual void registerObserver(shared_ptr<Observer> observer);
    virtual ~Subject() { }

protected:

    list<weak_ptr<Observer>> observerList;
    virtual void exitSession();

    virtual void unregisterObserver(shared_ptr<Observer> observer);
    virtual string getSessionKey()=0; // registerObserver() uses this to get session key
    virtual bool inline equals(Observer *obs0, Observer *obs1){
        return obs0==obs1;
    }
};


#endif //POCOSERVER_SUBJECT_H
