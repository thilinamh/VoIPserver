//
// Created by user on 12/16/15.
//

#ifndef POCOSERVER_REGRQUEST_H
#define POCOSERVER_REGRQUEST_H

#include <iostream>
using namespace std;

class RegistrationBehavior {

public:
    virtual bool registerUser(const string &uid, const string &uuid, const string &extra) = 0;
};


#endif //POCOSERVER_REGRQUEST_H
