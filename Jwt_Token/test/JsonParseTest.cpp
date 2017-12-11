//
// Created by orest on 22/11/17.
//

#include "gtest/gtest.h"

#include "../src/include/JsonValue.h"

TEST(JSON, json_parse)
{
	const wstring test1 = LR"(
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
	
	auto value = JSONValue::parse(test1);
	ASSERT_NE(nullptr, value);
	ASSERT_TRUE(value->isArray());
	
}

TEST(JSON, null)
{
	const wstring test;
	auto value = JSONValue::parse(test);
	ASSERT_EQ(nullptr, value);
	//ASSERT_TRUE(value->isNull());
}

TEST(JSON, brackets)
{
	const wstring test = L"[]";
	auto value = JSONValue::parse(test);
	ASSERT_NE(nullptr, value);
}

TEST(JSON, number)
{
	const wstring test = L"[1]";
	auto value = JSONValue::parse(test);
	ASSERT_NE(nullptr, value);
}

TEST(JSON, symbol_without_quotation_marks)
{
	const wstring test = L"[a]";
	auto value = JSONValue::parse(test);
	ASSERT_EQ(nullptr, value);
}

TEST(JSON, symbol_with_quotation_marks)
{
	const wstring test = LR"(["a"])";
	auto value = JSONValue::parse(test);
	ASSERT_NE(nullptr, value);
}

TEST(JSON, many_numbers)
{
	const wstring test = L"[ 1, 2 ,  3,4,5]";
	auto value = JSONValue::parse(test);
	ASSERT_NE(nullptr, value);
	ASSERT_TRUE(value->isArray());
}

TEST(JSON, many_symbols)
{
	const wstring test = LR"([1, "abc" , 3.14, -4])";
	auto value = JSONValue::parse(test);
	ASSERT_NE(nullptr, value);
	ASSERT_TRUE(value->isArray());
}

TEST(JSON, array_of_arrays_of_symbols)
{
	const wstring test = LR"([ "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 
"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb",
"ccccccccccccccccccccccc",
"dddddddddddddddddddddddddddddddddddddddddddddddddddd" ])";
	auto value = JSONValue::parse(test);
	ASSERT_NE(nullptr, value);
}

TEST(JSON, comment)
{
	const wstring test = LR"(
// Comment for array
[
   // Comment within array
   "one-element"
])";
	auto value = JSONValue::parse(test);
	ASSERT_NE(nullptr, value);
}

TEST(JSON, simple_json)
{
	const wstring test = LR"({ 
	"count" : 1234,
	"name" : { "aka" : "T.E.S.T.", "id" : 123987 },
	"attribute" : [ 
		"random", 
		"short", 
		"bold", 
		12, 
		{ "height" : 7, "width" : 64 } 
		],
	"test": { "1" : 
		{ "2" : 
			{ "3" :  { "coord" : [ 1,2] } 
			} 
		}
	}
})";
	auto value = JSONValue::parse(test);
	ASSERT_NE(nullptr, value);
	ASSERT_TRUE(value->isObject());
}

TEST(JSON, integer)
{
	const wstring test = L"-2147483648";
	auto value = JSONValue::parse(test);
	ASSERT_NE(nullptr, value);
	ASSERT_TRUE(value->isNumber());
}

TEST(JSON, object)
{
	const wstring test = L"{}";
	auto value = JSONValue::parse(test);
	ASSERT_NE(nullptr, value);
	ASSERT_TRUE(value->isObject());
}

TEST(JSON, out_of_range)
{
	const wstring test = L"8589934592";
	auto value = JSONValue::parse(test);
	ASSERT_NE(nullptr, value);
	ASSERT_TRUE(value->isNumber());
}

TEST(JSON, string)
{
	const wstring test = LR"("\"abc\\def\"")";
	auto value = JSONValue::parse(test);
	ASSERT_NE(nullptr, value);
	ASSERT_TRUE(value->isString());
}

TEST(JSON, random_symbols_string)
{
	const wstring test = LR"("!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]
^_`abcdefghijklmnopqrstuvwxyz{|}~!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]
^_`abcdefghijklmnopqrstuvwxyz{|}~!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]
^_`abcdefghijklmnopqrstuvwxyz{|}~!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]
^_`abcdefghijklmnopqrstuvwxyz{|}~!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]
^_`abcdefghijklmnopqrstuvwxyz{|}~!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]
^_`abcdefghijklmnopqrstuvwxyz{|}~!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]
^_`abcdefghijklmnopqrstuvwxyz{|}~!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]
^_`abcdefghijklmnopqrstuvwxyz{|}~!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]
^_`abcdefghijklmnopqrstuvwxyz{|}~!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]
^_`abcdefghijklmnopqrstuvwxyz{|}~!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]
^_`abcdefghijklmnopqrstuvwxyz{|}~!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]
^_`abcdefghijklmnopqrstuvwxyz{|}~!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]
^_`abcdefghijklmnopqrstuvwxyz{|}~!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]
^_`abcdefghijklmnopqrstuvwxyz{|}~!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]
^_`abcdefghijklmnopqrstuvwxyz{|}~!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]
^_`abcdefghijklmnopqrstuvwxyz{|}~!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]
^_`abcdefghijklmnopqrstuvwxyz{|}~!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]
^_`abcdefghijklmnopqrstuvwxyz{|}~!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]
^_`abcdefghijklmnopqrstuvwxyz{|}~!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]
^_`abcdefghijklmnopqrstuvwxyz{|}~")";
	auto value = JSONValue::parse(test);
	ASSERT_NE(nullptr, value);
	ASSERT_TRUE(value->isString());
}

TEST(JSON, unicode)
{
	const wstring test = LR"("\u00A2")";
	auto value = JSONValue::parse(test);
	ASSERT_NE(nullptr, value);
	ASSERT_TRUE(value->isString());
}


