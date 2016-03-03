//
// Created by tm on 19/01/16.
//

#ifndef POCOSERVER_VERIFICATIONBEHAVIOUR_H
#define POCOSERVER_VERIFICATIONBEHAVIOUR_H

#include <iostream>

using namespace std;
class VerificationBehaviour {

public:
    virtual bool verify(const std::string &verification_num, const std::string &uuid)=0;

};


#endif //POCOSERVER_VERIFICATIONBEHAVIOUR_H
