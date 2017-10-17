#ifndef JWT_TOKEN_JSON_H
#define JWT_TOKEN_JSON_H

#include <iostream>
#include <string>
#include <sstream>
#include <set>
#include <algorithm>

using namespace std;

class Json
{
private:
    set<pair<string, string>> data;
    void convert(string str);

    void assign(string str1, string str2);
public:
    Json(){};

    Json(string str);

    string toString();

    const char* toChars();

    void input();

    void output();

    string operator[](string key);
};


#endif

