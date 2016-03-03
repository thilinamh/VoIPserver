//
// Created by tm on 19/01/16.
//

#ifndef POCOSERVER_CRYPTO_H
#define POCOSERVER_CRYPTO_H

#include <iostream>
#include <memory>
using namespace std;
class Crypto {
public:
static unique_ptr<string>  decryptRequest(char *b64edata);
};


#endif //POCOSERVER_CRYPTO_H
