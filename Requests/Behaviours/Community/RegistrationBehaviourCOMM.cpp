//
// Created by user on 12/21/15.
//

#include "RegistrationBehaviourCOMM.h"
#include "../../../Helper/OTP/OTPGenerationRequest.h"
#include <string.h>
#include <unistd.h>
#define GetCurrentDir getcwd
bool RegistrationBehaviourCOMM::registerUser(const string &uid, const string &uuid, const string &extra) {
    cout<<"uid"<< uid <<endl;
    cout<<"uuid"<<uuid<<endl;
    return send_OTP(uid, uuid, extra);

}

bool RegistrationBehaviourCOMM::send_OTP(const string &uid, const string &uuid, const string &email) {
    using namespace std;
    OTPGenerationRequest otg;
    char _tmp[10]{},tref[13]{};
    strncpy(_tmp, uuid.c_str(), 8);
    sprintf(tref, "%012li", strtol(_tmp, NULL, 16));

    otg.setAppType("IB");
    otg.setCif("");
    otg.setEmailID(email.c_str());
    otg.setTelephoneNumber(uid.c_str());
    otg.setPassword("550a141f12de6341fba65b0ad0433500");
    otg.setMessageType("01");
    otg.setTransRefNo(tref);
    cout << otg.getMessage() << endl;
    cout << "Using OTP ... "<<endl;
    char java_cmd[1024];
    strcpy(java_cmd, "java -jar /home/user/Desktop/OTP.jar ");
    strcat(java_cmd, "\"");
    strcat(java_cmd, otg.getMessage());
    strcat(java_cmd, "\"");


/*
    char cCurrentPath[FILENAME_MAX];

    if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
    {
        return errno;
    }

    cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

//    printf ("The current working directory is %s", cCurrentPath);




    //cout << "Executing Command pipe " << java_cmd << endl;
    FILE *f;
    f = popen(java_cmd, "r");

    if (!f) {
        cerr << "Command execution error !! " << endl;
        return false;
    }
    char pbuff[128]{};


    while (!feof(f)) {
        fgets(pbuff, 128, f);
    }
    pclose(f);

    cout << "OTP Result : " << pbuff << endl;
    return strcmp(pbuff, "00") == 0x00 ||
    strcmp(pbuff, "21") == 0x00;

    //Decrypted data is +94714618011,epiclanka@gmail.com,a232c5d4-7018-42c2-90bf-6a8211b34652
    //tref : 002721236436
    //Using OTP ... 01|002721236436|IB|550a141f12de6341fba65b0ad0433500||+94714618011|epiclanka@gmail.com

}
