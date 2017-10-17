#include "../Include/Json.h"


using namespace std;

//string example: "alg:Base64 type:JWT name:John"
void Json::convert(string str)
{
    string key, value, pair;
    size_t pos;
    stringstream ss(str);
    while(getline(ss, pair, ' '))
    {
        pos = pair.find(':');
        key = pair.substr(0, pos);
        value = pair.substr(pos + 1);
        if(key.empty())
        {
            continue;
        }
        assign(key, value);
    }
}

void Json::assign(string str1, string str2)
{
    data.insert(make_pair(str1, str2));
}

Json::Json(string str)
{
    convert(str);
}

string Json::toString()
{
    string res;
    set<pair<string, string>>::iterator iter;
    for (iter = data.begin(); iter != data.end(); iter++)
    {
        res += iter->first + ":";
        res += iter->second + " ";
    }
    return res;
}

const char* Json::toChars()
{
    return toString().c_str();
}

void Json::input()
{
    string key;
    string value;
    cout << "Type \"Stop!\" as a key to break input:\n";
    while(true)
    {
        cout << "Key: ";
        cin >> key;
        if(key == "Stop!")
        {
            break;
        }
        cout << "Value: ";
        cin >> value;
        assign(key, value);
    }
}

void Json::output()
{
    set<pair<string, string>>::iterator iter;
    cout << "{\n";
    for (iter = data.begin(); iter != data.end(); iter++)
    {
        cout << "\t\"" << iter->first << "\"" << " : ";
        cout << "\"" << iter->second << "\"" << ";\n";
    }
    cout << "}\n";
}

string Json::operator[](string key)
{
    string empy_str;
    bool entrance;
    auto it = find_if(data.begin(), data.end(),
                       [&](const pair<string, string>& val) -> bool
                       {
                           entrance = (val.first == key);
                           return entrance;
                       });
    if(entrance)
    {
        return it->second;
    }
    else
    {
        return empy_str;
    }
}