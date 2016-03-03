//
// Created by user on 12/18/15.
//

#ifndef POCOSERVER_STATE_H
#define POCOSERVER_STATE_H
#include <odb/core.hxx>
#include <memory>
#include <iostream>

class User;
class State{

public:

    virtual void processRequest(const std::string &data, User &context) =0;
    bool operator ==( const State &b);
    bool operator !=( const State &b);

protected:
    State();
    //Derived objects should never be deleted by pointers of this type since they are singletons
    ~State();

private:

    State(const State& src)=delete;
    State& operator=(const State& rhs) =delete;
};
#endif //POCOSERVER_STATE_H
