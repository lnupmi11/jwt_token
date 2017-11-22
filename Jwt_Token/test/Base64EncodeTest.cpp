#include "Base64Encode.h"
#include "gtest/gtest.h"

using namespace std;

#define MANY_TIMES 5000


TEST(Base64UncodeTest, simple_sentence)
{
	string input = "Hello!May I introduce you a JWT token.";
	string expected = "SGVsbG8hTWF5IEkgaW50cm9kdWNlIHlvdSBhIEpXVCB0b2tlbi4";

	EXPECT_EQ(expected, Base64Encode::EncodeUrl(input));
	EXPECT_EQ(input, Base64Encode::DecodeUrl(expected));
}


TEST(Base64UncodeTest, buffer_overflows)
{
	string str_dec = "SGVsbG8hTWF5IEkgaW50cm9kdWNlIHlvdSBhIEpXVCB0b2tlbi4";
	char buffer[4096];

	for (size_t num_dec = 0; num_dec < str_dec.size(); num_dec++)
	{
		for (size_t num_buffer = 0; num_buffer < Base64Encode::DecodeBytesNeeded(num_dec) / 2; num_buffer++)
		{
			EXPECT_EQ(1, Base64Encode::DecodeUrl(str_dec.c_str(), num_dec, buffer, &num_buffer));
		}
	}
}


TEST(Base64UncodeTest, buffer_underflows)
{
	string str_enc = "Hello!May I introduce you a JWT token.";
	char buffer[4096];

	for (size_t num_enc = 6; num_enc < str_enc.size(); num_enc++)
	{
		for (size_t num_buffer = 0; num_buffer < Base64Encode::EncodeBytesNeeded(num_enc) / 2; num_buffer++)
		{
			EXPECT_EQ(1, Base64Encode::EncodeUrl(str_enc.c_str(), num_enc, buffer, &num_buffer));
		}
	}
}


TEST(Base64UncodeTest, partial)
{
	const char* simpleLine = "c2ltcGxlTGluZQ";
	char buf[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	size_t cBuf = 9;

	Base64Encode::DecodeUrl(simpleLine, 8, buf, &cBuf);

	EXPECT_EQ(6, cBuf);
	EXPECT_STREQ("simple", buf);
}


TEST(Base64UncodeTest, len)
{
	string hello;

	for (int i = 0; i < 9; i++)
	{
		hello.append("x");
		string enc = Base64Encode::EncodeUrl(hello);

		EXPECT_EQ(enc.size() + 1, Base64Encode::EncodeBytesNeeded(hello.size()));
	}
}


TEST(Base64UncodeTest, spec)
{
	EXPECT_STREQ("", Base64Encode::DecodeUrl("").c_str());
	EXPECT_STREQ("s", Base64Encode::DecodeUrl("cw").c_str());
	EXPECT_STREQ("si", Base64Encode::DecodeUrl("c2k").c_str());
	EXPECT_STREQ("sim", Base64Encode::DecodeUrl("c2lt").c_str());
	EXPECT_STREQ("simp", Base64Encode::DecodeUrl("c2ltcA").c_str());
	EXPECT_STREQ("simpl", Base64Encode::DecodeUrl("c2ltcGw").c_str());
	EXPECT_STREQ("simple", Base64Encode::DecodeUrl("c2ltcGxl").c_str());
}


TEST(Base64UncodeTest, spec_inv)
{
	EXPECT_STREQ("", Base64Encode::EncodeUrl("").c_str());
	EXPECT_STREQ("cw", Base64Encode::EncodeUrl("s").c_str());
	EXPECT_STREQ("c2k", Base64Encode::EncodeUrl("si").c_str());
	EXPECT_STREQ("c2lt", Base64Encode::EncodeUrl("sim").c_str());
	EXPECT_STREQ("c2ltcA", Base64Encode::EncodeUrl("simp").c_str());
	EXPECT_STREQ("c2ltcGw", Base64Encode::EncodeUrl("simpl").c_str());
	EXPECT_STREQ("c2ltcGxl", Base64Encode::EncodeUrl("simple").c_str());
}


TEST(Base64UncodeTest, bad)
{
	EXPECT_STREQ("", Base64Encode::DecodeUrl("Zg=").c_str());
	EXPECT_STREQ("", Base64Encode::DecodeUrl("Zg#").c_str());
	EXPECT_STREQ("", Base64Encode::DecodeUrl("Zm9vYmE@").c_str());
	EXPECT_STREQ("", Base64Encode::DecodeUrl(
			"VGhlIHF1aWNrIGJy\nb3duIGZveCBqdW1wcyBvdmVyIHRoZSBsYXp5IGRvZyBhbmQgc29tZSBleHR").c_str());
}


TEST(Base64UncodeTest, invers)
{
	string res = "dCm-Fpm5S8VCg0Mi8LPdMNwrZhQWtox7hFsH6oG-yf4";
	string out = Base64Encode::DecodeUrl(res);

	ASSERT_EQ(32, out.size());
	ASSERT_EQ(res, Base64Encode::EncodeUrl(Base64Encode::DecodeUrl(res)));
}


TEST(Base64UncodeTest, many_times_encode_to_another_array)
{
	string encode = "Simple text";
	size_t buf = 4096;
	char res[4096];

	for (int i = 0; i < MANY_TIMES; i++)
	{
		Base64Encode::EncodeUrl(encode.c_str(), encode.size(), res, &buf);
	}
}


TEST(Base64UncodeTest, many_times_encode)
{
	string encode = "Simple text";

	for (int i = 0; i < MANY_TIMES; i++)
	{
		Base64Encode::EncodeUrl(encode);
	}
}


TEST(Base64UncodeTest, many_times_decode_to_another_array)
{
	string encode = "U2ltcGxlIHRleHQ";
	char res[4096];
	size_t cRes = 4096;

	for (int i = 0; i < MANY_TIMES; i++)
	{
		Base64Encode::DecodeUrl(encode.c_str(), encode.size(), res, &cRes);
	}
}


TEST(Base64UncodeTest, many_times_decode)
{
	string encode = "U2ltcGxlIHRleHQ";

	for (int i = 0; i < MANY_TIMES; i++)
	{
		Base64Encode::DecodeUrl(encode);
	}
}