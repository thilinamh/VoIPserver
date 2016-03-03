//
// Created by tm on 19/01/16.
//

#ifndef POCOSERVER_STRINGHELPER_H
#define POCOSERVER_STRINGHELPER_H
#include <vector>
#include <memory>
#include<iostream>
using namespace std;
class StringHelper {
public:
 static unique_ptr<vector<string>> split(const string& str, char delimeter);
};


#endif //POCOSERVER_STRINGHELPER_H
