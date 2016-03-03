/* 
 * File:   ClientRequest.h
 * Author: user
 *
 * Created on December 14, 2015, 4:03 PM
 */

#ifndef CLIENTREQUEST_H
#define	CLIENTREQUEST_H

#include "Poco/Base64Encoder.h"
#include "Poco/Base64Decoder.h"
#include "Poco/Exception.h"


#include <sstream>
#include <memory>
using namespace std;

using Poco::Base64Encoder;
using Poco::Base64Decoder;
using Poco::DataFormatException;
class ClientRequest {
public:
    ClientRequest();
    ClientRequest(const ClientRequest& orig);
    virtual ~ClientRequest();
    void decodeRequest(std::stringstream& str );
    unique_ptr<string>  decryptRequest(char *b64edata);
private:
    //std::stringstream str;
   
};

#endif	/* CLIENTREQUEST_H */

