/*
 * OTPBase.cpp
 *
 *  Created on: Mar 16, 2015
 *      Author: shadowman
 */

#include "OTPBase.h"
#include <iostream>
#include <string.h>

using namespace std;
OTPBase::OTPBase() {
	memset(message_type, 0x00, 0x03);
	memset(trans_ref_no, 0x00, 13);
	memset(app_type, 0x00, 0x05);
}

OTPBase::~OTPBase() {
	// TODO Auto-generated destructor stub
}

void OTPBase::setMessageType(const char *msgt){
	strcpy(message_type, msgt);
}

void OTPBase::setTransRefNo(const char *refno){
	strcpy(trans_ref_no, refno);
}

void OTPBase::setAppType(const char *appt){
	strcpy(app_type, appt);
}


const char *OTPBase::getMessageType(){
	return message_type;
}

const char *OTPBase::getTransRefNo(){
	return trans_ref_no;
}

const char *OTPBase::getAppType(){
	return app_type;
}

const char *OTPBase::getMessage(){
	char *msg = new char[
	                     sizeof(message_type) + 1 +
	                     sizeof(trans_ref_no) + 1 +
	                     sizeof(app_type) + 1
	                     ];
	strcpy(msg, message_type);
	strcat(msg, OTP_SEP);
	strcat(msg, trans_ref_no);
	strcat(msg, OTP_SEP);
	strcat(msg, app_type);

	return msg;

}
