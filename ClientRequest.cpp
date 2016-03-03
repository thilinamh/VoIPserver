/* 
 * File:   ClientRequest.cpp
 * Author: user
 * 
 * Created on December 14, 2015, 4:03 PM
 */

#include "ClientRequest.h"
#include "Utils.h"
#include <iostream>
#include "RSAKeys.h"
#include <openssl/rsa.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <string.h>


using namespace std;

ClientRequest::ClientRequest() {
}

ClientRequest::ClientRequest(const ClientRequest& orig) {
}

ClientRequest::~ClientRequest() {
}

void ClientRequest::decodeRequest(std::stringstream& str){
    
		//Base64Encoder encoder(str);
		//encoder << "The quick brown fox ";
		//encoder << "jumped over the lazy dog.";
		//encoder.close();
                //char a[20]={"asdasd asdasd"};
		Base64Decoder decoder(str);
		std::string s;
		int c = decoder.get();
		while (c != -1) { 
                    s += char(c); c = decoder.get(); 
                }
                cout<<s<<endl;
}

unique_ptr<string>  ClientRequest::decryptRequest(char *b64edata) {
	int dLen = 0x00;
	char *buff, raw[4096], *ret = NULL;
	BIO *priv_key_bio = NULL;
	RSA *pk = NULL;

	b64edata[strlen(b64edata) - 1] = 0x00;

	cout << "B64EDED = " << b64edata << endl;

	Base64Decode(b64edata, &buff, &dLen);

	priv_key_bio = BIO_new_mem_buf((void *) RSA_PRV, -1);

	memset(raw, 0x00, 4096);

	if (priv_key_bio == NULL) {
		cerr << "RSA Key BIO creation failed.." << endl;
		throw std::bad_exception();
	} else {

		if ((pk = PEM_read_bio_RSAPrivateKey(priv_key_bio, &pk, NULL, NULL)) //read the private key
				== NULL) {
			char * err = (char *) malloc(130);
			ERR_load_crypto_strings();
			ERR_error_string(ERR_get_error(), err);
			printf("%s ERROR: %s\n", "L1", err);
			free(err);
		} else {

			cout << "Decrypting using RSA private key..." << endl;

			if (RSA_private_decrypt(dLen, (const unsigned char*) buff,
					(unsigned char *) raw, pk, RSA_PKCS1_PADDING) > -1) { //using pk
				cout << "RSA decrypted " << endl;
				cout << "Decrypted data is " << raw << endl;

				free(buff);
				RSA_free(pk);
				unique_ptr<string> decrptedString(new string(raw));

				return decrptedString; // moves the ownership
				//ret = new char[strlen(raw) + 1];
				//strcpy(ret, raw);
			}else{
				throw std::invalid_argument("cannot decrypt data");
			}
		}
	}
	return  unique_ptr<string>(new string(""));

}
