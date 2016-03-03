/*
 * OTPAuthRequest.cpp
 *
 *  Created on: May 8, 2015
 *      Author: shadowman
 */

#include "OTPAuthRequest.h"
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include "../../Utils.h"

#define OTP_SEP "|"

OTPAuthRequest::OTPAuthRequest() : OTPBase(){
	memset(password, 0x00, 33);
	memset(verif, 0x00, 10);

}

OTPAuthRequest::~OTPAuthRequest() {
	// TODO Auto-generated destructor stub
}

const char* OTPAuthRequest::getPassword() const {
	return password;
}

const char* OTPAuthRequest::getVerif() const {
	return verif;
}

void OTPAuthRequest::setPassword(const char *pwd){
	strcpy(password,pwd);
}

void OTPAuthRequest::setVerif(const char *v){
	char *b64buff;
	Base64Encode(v, &b64buff, strlen(v));

	strcpy(verif, b64buff);
	free(b64buff);
}

const char *OTPAuthRequest::getMessage(){
	const char *baseMsg = OTPBase::getMessage();
	char *msg = new char[strlen(baseMsg) + 1 + strlen(password) + 1 + strlen(verif) + 1];
	strcpy(msg, baseMsg);
	strcat(msg, OTP_SEP);
	strcat(msg, password);
	strcat(msg, OTP_SEP);
	strcat(msg, verif);

	return msg;


}


