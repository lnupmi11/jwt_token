#ifndef JWT_TOKEN_JSON_H
#define JWT_TOKEN_JSON_H

#include <vector>
#include <string>
#include <map>
#include "JsonValue.h"

static inline bool simplejson_wcsnlen(const wchar_t *s, size_t n)
{
	if (s == 0)
	{
		return false;
	}

	const wchar_t *save = s;
	while (n-- > 0)
	{
		if (*(save++) == 0)
		{
			return false;
		}
	}

	return true;
}

class JSON
{
	friend class JSONValue;

public:
	static JSONValue* parse(const char *data);
	static JSONValue* parse(const wchar_t *data);
protected:
	static bool skipWhitespace(const wchar_t **data);
	static bool extractString(const wchar_t **data, std::wstring &str);
	static double parseInt(const wchar_t **data);
	static double parseDecimal(const wchar_t **data);
private:
	JSON();
};


#endif
