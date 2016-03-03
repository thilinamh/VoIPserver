/*
 * OTPGenerationRequest.cpp
 *
 *  Created on: Mar 16, 2015
 *      Author: shadowman
 */

#include "OTPGenerationRequest.h"
#include <string.h>
#include <iostream>

using namespace std;
OTPGenerationRequest::OTPGenerationRequest() : OTPBase() {
	memset(password, 0x00, 33);

	memset(cif, 0x00, 0x07);
	memset(telephone_number, 0x00, 21);
	memset(email_id, 0x00, 65);
}

OTPGenerationRequest::~OTPGenerationRequest()  {
}

const char *OTPGenerationRequest::getPassword(){
	return password;
}
void OTPGenerationRequest::setPassword(const char *pwd){
	strcpy(password, pwd);
}

const char *OTPGenerationRequest::getCif(){
	return cif;
}
void OTPGenerationRequest::setCif(const char *cif){
	strcpy(this->cif, cif);
}

const char *OTPGenerationRequest::getTelephoneNumber(){
	return telephone_number;
}
void OTPGenerationRequest::setTelephoneNumber(const char *telno){
	strcpy(telephone_number, telno);
}

const char *OTPGenerationRequest::getEmailID(){
	return email_id;
}
void OTPGenerationRequest::setEmailID(const char *eml){
	strcpy(email_id, eml);
}

const char *OTPGenerationRequest::getMessage() {
	const char *baseMsg = OTPBase::getMessage();
	char *msg = new char[
	                           strlen(baseMsg) + 1 +
	                           sizeof(password) + 1 +
	                           sizeof(cif) + 1 +
	                           sizeof(telephone_number) + 1 +
	                           sizeof(email_id) + 1
	                           ];

	strcpy(msg, baseMsg);
	strcat(msg, OTP_SEP);
	strcat(msg, password);
	strcat(msg, OTP_SEP);
	strcat(msg, cif);
	strcat(msg, OTP_SEP);
	strcat(msg, telephone_number);
	strcat(msg, OTP_SEP);
	strcat(msg, email_id);

	delete [] baseMsg;

	return msg;
}

