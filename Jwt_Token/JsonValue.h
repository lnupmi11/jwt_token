#ifndef JWT_TOKEN_JSONVALUE_H
#define JWT_TOKEN_JSONVALUE_H

#include <vector>
#include <string>

//TODO implement JSON class
class JSON;

//custom types
typedef std::vector<JSONValue*> JSONArray;
typedef std::map<std::wstring, JSONValue*> JSONObject;


enum JSONType { JSONType_Null, JSONType_String, JSONType_Bool, JSONType_Number, JSONType_Array, JSONType_Object };

class JSONValue
{
	friend class JSON;

public:
	JSONValue(/*null*/);
	JSONValue(const wchar_t *m_char_value);
	JSONValue(const std::wstring &m_string_value);
	JSONValue(bool m_bool_value);
	JSONValue(double m_number_value);
	JSONValue(int m_integer_value);
	JSONValue(const JSONArray &m_array_value);
	JSONValue(const JSONObject &m_object_value);
	JSONValue(const JSONValue &m_source);
	~JSONValue();

	bool isNull() const;
	bool isString() const;
	bool isBool() const;
	bool isNumber() const;
	bool isArray() const;
	bool isObject() const;

	const std::wstring &asString() const;
	bool asBool() const;
	double asNumber() const;
	const JSONArray &asArray() const;
	const JSONObject &asObject() const;

	std::size_t countChildren() const;
	bool hasChild(std::size_t index) const;
	JSONValue *child(std::size_t index);
	bool hasChild(const wchar_t* name) const;
	JSONValue *child(const wchar_t* name);
	std::vector<std::wstring> objectKeys() const;

	std::wstring stringify(bool const prettyprint = false) const;
protected:
	static JSONValue *parse(const wchar_t **data);

private:

	JSONType type;

	union
	{
		bool bool_value;
		double number_value;
		std::wstring *string_value;
		JSONArray *array_value;
		JSONObject *object_value;
	};

};

#endif //JWT_TOKEN_JSONVALUE_H
