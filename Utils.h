/*
 * Utils.hpp
 *
 *  Created on: Feb 18, 2015
 *      Author: shadowman
 */

#ifndef UTILS_HPP_
#define UTILS_HPP_

void trim(char *);
int Base64Decode(char *, char **, int *);
int Base64Encode(const char *, char **, int);
void _mkdir(char *);

#define ERR_EXP "exr\r\n"
#define ERR_DEA "dea\r\n"
#define ERR_REM_DEA "rda\r\n"

#define ERR_NO_CREDITS "nec\r\n"

#define DEF_SEC 0x00000000000000FF
#define DEF_MSG 0x000000FF
#define DEF_MED 0x000000FF

#define DEF_EXP 0x000000FF
#define DEF_PRD "F"



class ServerConf {
public:
    static int SMS_PORT;
    static int CALL_PORT;
    static char mysql_sentry_ip[25];
    static char mysql_sentry_dbname[25];
    static char mysql_sentry_user[25];
    static char mysql_sentry_password[25];
    static char mysql_remote_ip[25];
    static char mysql_remote_dbname[25];
    static char mysql_remote_user[25];
    static char mysql_remote_password[25];
    static char user_web_dir[25];
    static char user_upload_dir[25];




private:

};




#endif /* UTILS_HPP_ */