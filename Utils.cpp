/*
 * Utils.cpp
 *
 *  Created on: Feb 18, 2015
 *      Author: shadowman
 */

#include "Utils.h"
#include <string.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/des.h>
#include <string.h>
#include <stdio.h>
#include <string>
#include <math.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>




void trim(char *s){
    int cpos = strlen(s) - 1;
    while(cpos > -1){
        if( *(s+cpos) == '\r' || *(s+cpos) == '\n')
            *(s+cpos) = 0x00;
        cpos--;
    }
}

int calcDecodeLength(const char* b64input) { //Calculates the length of a decoded base64 string
    int len = strlen(b64input);
    int padding = 0;

    if (b64input[len-1] == '=' && b64input[len-2] == '=') //last two chars are =
        padding = 2;
    else if (b64input[len-1] == '=') //last char is =
        padding = 1;

    return (int)len *0.75 - padding;
}

int Base64Decode(char* b64message, char** buffer, int *decl) { //Decodes a base64 encoded string
    BIO *bio, *b64;
    int decodeLen = calcDecodeLength(b64message),
            len = 0;
    *buffer = (char*)malloc(decodeLen+1);
    FILE* stream = fmemopen(b64message, strlen(b64message), "r");
    memset(*buffer, 0x00, decodeLen+1);

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new_fp(stream, BIO_NOCLOSE);
    // bio = BIO_new_mem_buf(b64message,strlen(b64message) );
    bio = BIO_push(b64, bio);
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); //Do not use newlines to flush buffer
    len = BIO_read(bio, *buffer, strlen(b64message));
    //Can test here if len == decodeLen - if not, then return an error
    //(*buffer)[len] = '\0';

    // BIO_free(b64);
    BIO_free_all(bio);

    fclose(stream);

    *decl = len;
    return (0); //success
}

int Base64Encode(const char* message, char** buffer, int size) { //Encodes a string to base64
    BIO *bio, *b64;
    FILE* stream;
    int encodedSize = 4*ceil((double)size/3);
    *buffer = (char *)malloc(encodedSize+1);

    stream = fmemopen(*buffer, encodedSize+1, "w");
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new_fp(stream, BIO_NOCLOSE);
    bio = BIO_push(b64, bio);
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); //Ignore newlines - write everything in one line
    BIO_write(bio, message, size);
    BIO_flush(bio);
    BIO_free_all(bio);
    fclose(stream);

    return (0); //success
}

#ifdef ENT
char *ServerConf::keyStream = "GZEBBSAAGYcJGCAAIBQCICAAIBQCJ+r/";

char *ServerConf::getKey(){
	char *k = NULL;
	int sz = 0x00;
	Base64Decode(keyStream, &k, &sz);

	if(sz == 24)
		return k;
	else
		return NULL;
}

bool ServerConf::isEligible(){
	mysqlpp::Connection con = mysqlpp::Connection(false);
	mysqlpp::Query q = NULL;
	mysqlpp::StoreQueryResult qr;

	char *dbKeyBytes;
	char *sharedKey;
	char buffer[25];

	int b64decodedSize;

	char yyyy[5], mm[3], dd[3], fdd[5];

	DES_cblock cb1, cb2, cb3;
	DES_key_schedule ks1, ks2, ks3;

	time_t now = time(0);
	time_t future;

	struct tm expire_time;
	struct tm creation_time;

	memset(buffer, 0x00, 25);
	memset(yyyy, 0x00, 5);
	memset(mm, 0x00, 3);
	memset(dd, 0x00, 3);
	memset(fdd, 0x00, 5);

	memset(&expire_time, 0x00, sizeof(struct tm));

	if( con.connect(mysql_sentry_dbname, mysql_sentry_ip, mysql_sentry_user, mysql_sentry_password, 3306) ){
		q = con.query("SELECT activationkey FROM commonconfiguration");
		//q = con.query("select 'et6hyYTfpfhqZqlPP6WRJRjI09MNuOSJ' as 'activationkey'");
		qr = q.store();
		if(qr.size() == 0){
			std::cerr << "Key info not found" << std::endl;
		}else{
			std::cout << "Base64 Encoded data is " << (*qr.data())[0] << std::endl;

			Base64Decode((char *)((*qr.data())[0]).c_str(), &dbKeyBytes, &b64decodedSize);

			if(b64decodedSize < 0x0A)
				std::cerr << "Invalid Key" << std::endl;
			else{
				sharedKey = getKey();



				memcpy((DES_cblock *)cb1, sharedKey,8);
				memcpy((DES_cblock *)cb2, sharedKey+8,8);
				memcpy((DES_cblock *)cb3, sharedKey+16,8);

				/*DES_set_key(&cb1,&ks1);
				DES_set_key(&cb2,&ks2);
				DES_set_key(&cb3,&ks3);*/

				des_set_odd_parity(&cb1);
				des_set_odd_parity(&cb2);
				des_set_odd_parity(&cb3);


				if(DES_set_key_checked(&cb1, &ks1) || DES_set_key_checked(&cb2, &ks2) || DES_set_key_checked(&cb3, &ks3) ){
					std::cerr << "3DES key scheduling error" << std::endl;
				}else{

					DES_ecb3_encrypt((C_Block *)dbKeyBytes,(C_Block *)buffer, &ks1, &ks2, &ks3, DES_DECRYPT);
					DES_ecb3_encrypt((C_Block *)dbKeyBytes+1,(C_Block *)buffer+1, &ks1, &ks2, &ks3, DES_DECRYPT);
					DES_ecb3_encrypt((C_Block *)dbKeyBytes+2,(C_Block *)buffer+2, &ks1, &ks2, &ks3, DES_DECRYPT);

					free(sharedKey);
					free(dbKeyBytes);

					//std::cout << "Buffer " << buffer << std::endl;

					strncpy(yyyy, buffer+8, 4);
					strncpy(mm, buffer+12, 2);
					strncpy(dd, buffer+14, 2);
					strncpy(fdd, buffer+16, 4);

					expire_time.tm_mday = atoi(dd);
					expire_time.tm_mon = atoi(mm)-1;
					expire_time.tm_year = atoi(yyyy) - 1900;

					creation_time = expire_time;

					expire_time.tm_mday +=   atoi(fdd);

					future = mktime(&creation_time);

					if(now > future)
					{
						future = mktime(&expire_time);

						if(future >= now)
							return true;
					}

				}
			}
		}
	}else{
		std::cerr << "Unable to check eligibility, Connection failed..." << std::endl;
	}

	if(con.connected())
		con.disconnect();

	return false;

}

#endif

void _md(char *dirs){
    char x[1000];
    strcpy(x,  dirs);
    char *curDir = strtok(x, "/");
    std::string fpath;

    struct stat buf;

    if(curDir)
    if(strlen(curDir) == 0x00){
        _md(curDir);
    }else{
        if(*dirs == '/'){
            fpath = '/';
            fpath.append(curDir);
        }else{
            fpath = curDir;
        }
        if(stat(fpath.c_str(), &buf) != 0x00)
            mkdir(fpath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

        chdir(fpath.c_str());
        _md(curDir+strlen(curDir)+1);

    }
}

void _mkdir(char *dirs){
    std::string cwd = get_current_dir_name();
    _md(dirs);
    chdir(cwd.c_str());
}



