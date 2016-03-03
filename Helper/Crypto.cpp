//
// Created by tm on 19/01/16.
//

#include "Crypto.h"


#include <iostream>
#include "../Utils.h"
#include "../RSAKeys.h"
#include <openssl/rsa.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <string.h>

unique_ptr<string>  Crypto::decryptRequest(char *b64edata) {
    int dLen = 0x00;
    char *buff, raw[4096], *ret = NULL;
    BIO *priv_key_bio = NULL;
    RSA *pk = NULL;

    b64edata[strlen(b64edata) - 1] = 0x00;

    //cout << "B64EDED = " << b64edata << endl;

    Base64Decode(b64edata, &buff, &dLen);

    priv_key_bio = BIO_new_mem_buf((void *) RSA_PRV, -1);

    memset(raw, 0x00, 4096);

    if (priv_key_bio == NULL) {
        cerr << "RSA Key BIO creation failed.." << endl;
        throw std::bad_exception();
    } else {

        if ((pk = PEM_read_bio_RSAPrivateKey(priv_key_bio, &pk, NULL, NULL)) //read the private key
            == NULL) {
            char *err = (char *) malloc(130);
            ERR_load_crypto_strings();
            ERR_error_string(ERR_get_error(), err);
            printf("%s ERROR: %s\n", "L1", err);
            free(err);
        } else {

            //cout << "Decrypting using RSA private key..." << endl;

            if (RSA_private_decrypt(dLen, (const unsigned char *) buff,
                                    (unsigned char *) raw, pk, RSA_PKCS1_PADDING) > -1) { //using pk
                cout << "RSA decrypted " << endl;
                //cout << "Decrypted data is " << raw << endl;

                free(buff);
                RSA_free(pk);
                unique_ptr<string> decrptedString(new string(raw));

                return decrptedString; // moves the ownership
                //ret = new char[strlen(raw) + 1];
                //strcpy(ret, raw);
            } else {
                throw std::invalid_argument("cannot decrypt data");
            }
        }
    }
}
