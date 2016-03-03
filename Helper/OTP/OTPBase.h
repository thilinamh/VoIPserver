/*
 * OTPBase.h
 *
 *  Created on: Mar 16, 2015
 *      Author: shadowman
 */

#ifndef OTPBASE_H_
#define OTPBASE_H_

#define OTP_SEP "|"

class OTPBase {
public:
	OTPBase();
	virtual ~OTPBase();

	const char *getMessageType();
	const char *getTransRefNo();
	const char *getAppType();

	virtual const char *getMessage();

	void setMessageType(const char *);
	void setTransRefNo(const char *);
	void setAppType(const char *);


protected:
	char message_type[3];
	char trans_ref_no[13];
	char app_type[5];

};

#endif /* OTPBASE_H_ */
