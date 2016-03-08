//
// Created by user on 2/3/16.
//

#include "Subject.h"

void Subject::registerObserver(shared_ptr<Observer> observer) {
    observerList.push_front(weak_ptr<Observer>(observer));
}

void Subject::unregisterObserver(shared_ptr<Observer> observer) {
    list<weak_ptr<Observer>>::iterator it=observerList.begin();

    //Equals pred(observer.get());
    //observerList.remove_if(pred);
    Observer * obs=observer.get();
    for (;it!=observerList.end();it++){

        if(equals(it->lock().get(),obs)){
            obs->onUnregisterFromObserver();
            observerList.erase(it);
            break;
        }

    }
}



void Subject::notifyAll() {
    if(observerList.size()<2){
        return;
    }
    for(auto& obs:observerList){
        auto observer(obs.lock());//get shared from weak
        if(observer!= nullptr){
            observer->update();
        }
    }
}

void Subject::sendToAll(const string &command) {
    if(observerList.size()<2){
        return;
    }
    for(auto& obs:observerList){
        auto observer(obs.lock());//get shared from weak
        if(observer!= nullptr){
            observer->update(command);
        }
    }
}

void Subject::exitSession() {
    for(auto&obs_wk_ptr:observerList){
        auto observer_shrd_ptr(obs_wk_ptr.lock());//get shared from weak
        if(observer_shrd_ptr != nullptr){
            observer_shrd_ptr->onSessionExit();
        }
    }
    cout<<"Deleted all observers"<<endl;
}
