#include <utility>

#include "../Include/Json.h"


using namespace std;

//string example: "alg:Base64 type:JWT name:John"
void Json::convert(string str)
{
	string key, value, pair;
	size_t pos;
	stringstream ss(str);
	// TODO: doesn't work
	// Expected: { "url": "Bad code" }
	// Actual: {""Bad":""Bad", ""url"":"", "code"":"code"", "{":"{", "}":"}", }
	while (getline(ss, pair, ' '))
	{
		pos = pair.find(':');
		key = pair.substr(0, pos);
		value = pair.substr(pos + 1);
		if (key.empty())
		{
			continue;
		}
		data[key] = value;
	}
}

Json::Json(string str)
{
	convert(std::move(str));
}

string Json::toString(bool pretty) const
{
	string res;
	res += "{ ";
	if (pretty) res += "\n";
	for (const auto &iter : data)
	{
		res += "\"" + iter.first + "\"" + ":";
		res += "\"" + iter.second + "\"" + ", ";
		if (pretty) res += "\n";
	}
	res += "}";
	return res;
}

void Json::input()
{
	string key;
	string value;
	cout << "Type \"Stop!\" as a key to break input:\n";
	while (true)
	{
		cout << "Key: ";
		cin >> key;
		if (key == "Stop!")
		{
			break;
		}
		cout << "Value: ";
		cin >> value;
		data[key] = value;
	}
}

ostream &operator<<(ostream &os, const Json &json)
{
	os << json.toString(true);
	return os;
}

string Json::operator[](string key)
{
	bool entrance;
	auto it = find_if(data.begin(), data.end(),
					  [&](const pair<string, string> &val) -> bool
					  {
						  entrance = (val.first == key);
						  return entrance;
					  });
	if (entrance)
		return it->second;
	else
		return "";
}