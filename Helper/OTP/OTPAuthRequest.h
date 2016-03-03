/*
 * OTPAuthRequest.h
 *
 *  Created on: May 8, 2015
 *      Author: shadowman
 */

#ifndef OTPAUTHREQUEST_H_
#define OTPAUTHREQUEST_H_

#include "OTPBase.h"

class OTPAuthRequest : public OTPBase{
public:
	OTPAuthRequest();
	virtual ~OTPAuthRequest();
	const char* getPassword() const;
	const char* getVerif() const;

	void setPassword(const char *);
	void setVerif(const char *);

	virtual const char *getMessage();


private:
	char password[33];
	char verif[10];


};

#endif /* OTPAUTHREQUEST_H_ */
