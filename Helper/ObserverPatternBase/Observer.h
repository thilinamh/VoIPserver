//
// Created by user on 2/3/16.
//

#ifndef POCOSERVER_OBSERVER_H
#define POCOSERVER_OBSERVER_H

#include <iostream>

class Observer {
public:
    virtual void update()=0;
    virtual void update(const std::string& message)=0;
    virtual void onSessionExit()=0;
    virtual bool onUDPSessionStarted()=0;
    virtual void onUnregisterFromObserver()=0;
    virtual ~Observer() { }
};


#endif //POCOSERVER_OBSERVER_H
