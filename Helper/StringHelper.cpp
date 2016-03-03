//
// Created by tm on 19/01/16.
//

#include "StringHelper.h"
#include <sstream>

unique_ptr<vector<string>> StringHelper::split(const std::string& str, char delimeter) {

    std::istringstream ss(str);
    std::string token;
    unique_ptr<vector<string>> tokens(new vector<string>());


    while(std::getline(ss, token, delimeter)) {
        tokens->push_back(token);
    }
    return tokens;

}
