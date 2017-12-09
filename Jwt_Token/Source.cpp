//#include "Base64Encode.h"
//#include "Json.h"
//#include "RSA.h"

#include "JsonValue.h"

int main()
{
	const wstring data = LR"(
[
	"JSON test Pattern pass1",
	{"object with 1 member":["array with 1 element"]},
	{},
	[],
	-42,
	true,
	false,
	null,
	{
		"integer": 1234567890,
		"real": -9876.543210,
		"e": 0.123456789e-12,
		"E": 1.234567890E+34,
		"":  23456789012E66,
		"zero": 0,
		"one": 1,
		"space": " ",
		"quote": "\"",
		"backslash": "\\",
		"controls": "\b\f\n\r\t",
		"slash": "/ & \/",
		"alpha": "abcdefghijklmnopqrstuvwyz",
		"ALPHA": "ABCDEFGHIJKLMNOPQRSTUVWYZ",
		"digit": "0123456789",
		"0123456789": "digit",
		"special": "`1~!@#$%^&*()_+-={':[,]}|;.</>?",
		"true": true,
		"false": false,
		"null": null,
		"array":[  ],
		"object":{  },
		"address": "50 St. James Street",
		"url": "http://www.JSON.org/",
		"comment": "// /* <!-- --",
		"# -- --> */": " ",
		" s p a c e d " :[1,2 , 3

,

4 , 5        ,          6           ,7        ],"compact":[1,2,3,4,5,6,7],
		"jsontext": "{\"object with 1 member\":[\"array with 1 element\"]}",
		"quotes": "&#34; \u0022 %22 0x22 034 &#x22;",
		"\/\\\"\b\f\n\r\t`1~!@#$%^&*()_+-=[]{}|;:',./<>?"
: "A key can be any string"
	},
	0.5 ,98.6
,
99.44
,

1066,
1e1,
0.1e1,
1e-1,
1e00,2e+00,2e-00
,"rosebud"]
)";

	/*const wstring data = LR"(
{
	"quote": "\"",
	"jsontext": "{\"object with 1 member\":[\"array with 1 element\"]}",
	"quotes": "&#34; \u0022 %22 0x22 034 &#x22;",
	"\/\\\"\b\f\n\r\t`1~!@#$%^&*()_+-=[]{}|;:',./<>?"
: "A key can be any string"
}
)";*/
	wcout << "data=" << data << endl;
//	wcout << "trimmed=" << JSONValue::removeWhitespace(data) << endl;
	auto value = JSONValue::parse(data);
	if (value)
	{
		wcout << "parsed=" << value->stringify(2) << endl;
		return 0;
	} else
	{
		return 1;
	}
}
