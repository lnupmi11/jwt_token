#ifndef JWT_TOKEN_JSON_H
#define JWT_TOKEN_JSON_H

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>

using namespace std;

class Json
{
private:
    map<string, string> data;
    void convert(string str);

public:
    Json() = default;;

	explicit Json(string str);

    string toString(bool pretty=false) const;

    void input();

    string operator[](string key);

	friend ostream &operator<<(ostream &os, const Json &json);
};


#endif

