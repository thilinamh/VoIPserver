//
// Created by tm on 26/01/16.
//

#include <openssl/bio.h>
#include "CallInItializerBehaviour.h"
#include "../../UserStructure/User.hxx"
#include "../../Utils.h"
#include <iostream>
#include <openssl/pem.h>
#include <openssl/err.h>

using namespace std;
void CallInItializerBehaviour::sendSessionKey(string skey, User &user, bool isReciever)

{
    unsigned char session_key[skey.size()];
    strcpy((char *)session_key,skey.c_str());
    char *full_cmd;

    unsigned char encryptedKey[4098];
    unsigned char *b64e;

    int total_length = 30; // "sky <twenti_five_digit_num>,"

    int rsaRes, r1;

    RSA *my_pub_key = NULL;


    BIO *kb1;

    kb1 = BIO_new_mem_buf(&(user.getMypubKey()[0]), -1);

    if ((my_pub_key = PEM_read_bio_RSA_PUBKEY(kb1, &my_pub_key, NULL, NULL)) == NULL) {

        char * err = (char *) malloc(130);
        ERR_load_crypto_strings();
        ERR_error_string(ERR_get_error(), err);
        printf("%s ERROR: %s\n", "L1", err);
        free(err);
        throw Poco::LibraryLoadException(1);

    } else {
            //rsa_encrypt with this->my_certificate

            rsaRes = RSA_public_encrypt(10, session_key, encryptedKey, my_pub_key, RSA_PKCS1_PADDING);

            RSA_free(my_pub_key);
            BIO_free(kb1);
        if (rsaRes > -1) {
            r1 = rsaRes;
            //ras encrypt with this->remote->my_certificate
                if (Base64Encode((const char *) encryptedKey, (char **) &b64e, r1) == 0) {
                    //total_length += rsa1_size
                    total_length += strlen((const char *) b64e);

                    full_cmd = new  char[total_length + 3];
                    memset(full_cmd, 0x00, total_length + 3);
                    strcpy(full_cmd, "sky ");
                    if(isReciever) {
                        strcat( full_cmd, user.getUid().c_str());
                        strcat( full_cmd, ",");
                        cout<< "Encoded Session Key Sent To Receiver";
                        cout << ": My base64 encoded key is " << b64e << endl;
                    }else{
                        cout<< "Encoded Session Key Sent To Caller";
                        cout << ": My base64 encoded key is " << b64e << endl;
                    }
                    strcat( full_cmd, (const char *) b64e);
                    strcat( full_cmd, "\r\n");

                    //send_to_callie
                    string command;
                    command.assign(full_cmd);
                    user.writeToClient(command, 0);



                    delete[] full_cmd;
                    free(b64e);
                }

        } else {
            char * err = (char *) malloc(130);
            ERR_load_crypto_strings();
            ERR_error_string(ERR_get_error(), err);
            printf("%s ERROR: %s\n", "L1", err);
            free(err);
            throw Poco::DataException(1);
            //disconnect();

        }

    }


}
