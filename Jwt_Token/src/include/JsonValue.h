#ifndef JWT_TOKEN_JSONVALUE_H
#define JWT_TOKEN_JSONVALUE_H

#include <vector>
#include <string>
#include <map>
#include <cwctype>
#include <algorithm>
#include <sstream>

#include <iostream>

using namespace std;

//TODO implement JSON class
class JSON;

class JSONValue;

//custom types
typedef vector<JSONValue*> JSONArray;
typedef map<wstring, JSONValue*> JSONObject;

static inline bool isnan(double x)
{ return x != x; }

static inline bool isinf(double x)
{ return !isnan(x) && isnan(x - x); }

enum JSONType
{
	JSONType_Null, JSONType_String, JSONType_Bool, JSONType_Number, JSONType_Array, JSONType_Object
};

class JSONValue
{
	friend class JSON;

public:
	JSONValue(/*null*/);

	explicit JSONValue(const char* m_string_value);

	explicit JSONValue(const wchar_t* m_string_value);

	explicit JSONValue(const string& m_string_value);

	explicit JSONValue(const wstring& m_string_value);

	explicit JSONValue(bool m_bool_value);

	explicit JSONValue(double m_number_value);

	explicit JSONValue(int m_integer_value);

	explicit JSONValue(const JSONArray& m_array_value);

	explicit JSONValue(const JSONObject& m_object_value);

	JSONValue(const JSONValue& m_source);

	~JSONValue();

	bool isNull() const;

	bool isString() const;

	bool isBool() const;

	bool isNumber() const;

	bool isArray() const;

	bool isObject() const;

	const wstring& asString() const;

	bool asBool() const;

	double asNumber() const;

	const JSONArray& asArray() const;

	const JSONObject& asObject() const;

	size_t countChildren() const;

	bool hasChild(size_t index) const;

	JSONValue* child(size_t index);

	bool hasChild(const wchar_t* name) const;

	JSONValue* child(const wchar_t* name);

	vector<wstring> objectKeys() const;

	wstring stringify(const uint& indent_size = 0, const uint& indent_depth = 0) const;

	static wstring stringifyString(const wstring& str);

	static JSONValue* parse(wstring data);

	static JSONValue* parse(string data);

	template<typename T>
	T get()
	{
		switch (type)
		{
			case JSONType_Null:
				return 0;
			case JSONType_String:
				return asString();
			case JSONType_Bool:
				return asBool();
			case JSONType_Number:
				return asNumber();
			case JSONType_Array:
				return asArray();
			default:
				return 0;
		}
	}

	static wstring removeWhitespace(const wstring& data);

protected:
	static wstring extractString(const wstring& data);

private:

	JSONType type;

	union
	{
		bool bool_value;
		double number_value;
		wstring* string_value;
		JSONArray* array_value;
		JSONObject* object_value;
	};

	wstring getIndent(const uint& indent_size, const uint& indent_depth) const;

	static bool quoteClosed(const wstring& data);
};

#endif //JWT_TOKEN_JSONVALUE_H
