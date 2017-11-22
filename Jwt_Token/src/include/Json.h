#ifndef JWT_TOKEN_JSON_H
#define JWT_TOKEN_JSON_H

#include <vector>
#include <string>
#include <map>
#include "JsonValue.h"

class JSON
{
	friend class JSONValue;

public:
	static JSONValue* Parse(const char *data);
	static JSONValue* Parse(const wchar_t *data);
	static JSONValue* Parse(wstring *data);
	static std::wstring Stringify(const JSONValue *value);
protected:
	static bool SkipWhitespace(const wchar_t **data);
	static bool ExtractString(const wchar_t **data, std::wstring &str);
	static double ParseInt(const wchar_t **data);
	static double ParseDecimal(const wchar_t **data);
private:
	JSON();
};


#endif
