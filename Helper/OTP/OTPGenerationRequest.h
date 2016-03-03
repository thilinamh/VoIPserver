/*
 * OTPGenerationRequest.h
 *
 *  Created on: Mar 16, 2015
 *      Author: shadowman
 */

#ifndef OTPGENERATIONREQUEST_H_
#define OTPGENERATIONREQUEST_H_

#include "OTPBase.h"

class OTPGenerationRequest : public OTPBase {
public:
	OTPGenerationRequest();
	virtual ~OTPGenerationRequest();

	const char *getPassword();

	const char *getCif();
	const char *getTelephoneNumber();
	const char *getEmailID();

	virtual const char *getMessage();

	void setPassword(const char *);

	void setCif(const char *);
	void setTelephoneNumber(const char *);
	void setEmailID(const char *);

private:
	char password[33];

	char cif[7];
	char telephone_number[21];
	char email_id[65];

};

#endif /* OTPGENERATIONREQUEST_H_ */
