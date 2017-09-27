#include "../Include/Base64Encode.h"

inline char Base64Encode::EncodeChar(unsigned in)
{
	const char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
	return table[in];
}

string Base64Encode::EncodeUrl(const string &input)
{
	size_t num_encoded = EncodeBytesNeeded(input.size());
	std::unique_ptr<char[]> encoded(new char[num_encoded]);
	EncodeUrl(input.c_str(), input.size(), encoded.get(), &num_encoded);
	return string(encoded.get(), num_encoded - 1);
}

int Base64Encode::EncodeUrl(const char *encode, size_t num_encode, char *result, size_t *num_result)
{
	// No integer overflows please.
	if ((encode + num_encode) < encode || (result + *num_result) < result)
	{
		return 1;
	}

	if (EncodeBytesNeeded(num_encode) > *num_result)
	{
		return 1;
	}

	if (num_encode == 0)
	{
		*result = 0;
		return 0;
	}

	const char *start = result;
	size_t eLen = (num_encode / 3) * 3; // Length of even 24-bits.

										// Encode even 24-bits
	for (size_t s = 0; s < eLen; s += 3)
	{
		// Copy next three bytes into lower 24 bits of int, paying attension to
		// sign.
		int i = (*encode++ & 0xff) << 16;
		i = i | (*encode++ & 0xff) << 8;
		i = i | (*encode++ & 0xff);

		// Encode the int into four chars
		*result++ = EncodeChar((i >> 18) & 0x3f);
		*result++ = EncodeChar((i >> 12) & 0x3f);
		*result++ = EncodeChar((i >> 6) & 0x3f);
		*result++ = EncodeChar(i & 0x3f);
	}

	// Pad and encode last bits if source isn't an even 24 bits.
	size_t left = num_encode - eLen; // 0 - 2.
	if (left > 0)
	{
		// Prepare the int
		uint32_t i = ((*encode++ & 0xff) << 10);
		i = i | (left == 2 ? ((*encode & 0xff) << 2) : 0);

		// Set last four chars
		*result++ = EncodeChar(i >> 12);
		*result++ = EncodeChar((i >> 6) & 0x3f);
		if (left == 2)
		{
			*result++ = EncodeChar(i & 0x3f);
		}
	}
	*result++ = 0;
	*num_result = (result - start);
	return 0;
}

int Base64Encode::DecodeUrl(const char *decode, size_t num_decode, char *out, size_t *num_out) {
  if ((decode + num_decode) < decode || (out + *num_out) < out)
    return 1;

  if (*num_out < DecodeBytesNeeded(num_decode))
    return 1;

  const char *end = decode + num_decode;
  const char *out_start = out;
  char iter = 0;
  uint32_t buf = 0;

  while (decode < end) {
    uint8_t ch = *decode++;
    char c = DecodeChar(ch);

    switch (c) {
    case INVALID:
      return 1; // invalid input, return error
    default:
      buf = buf << 6 | c;
      iter++; // increment the number of iteration
      // If the buffer is full, split it into bytes
      if (iter == 4) {
        *(out++) = (buf >> 16) & 0xff;
        *(out++) = (buf >> 8) & 0xff;
        *(out++) = buf & 0xff;
        buf = 0;
        iter = 0;
      }
    }
  }

  if (iter == 3) {
    *(out++) = (buf >> 10) & 0xff;
    *(out++) = (buf >> 2) & 0xff;
  } else {
    if (iter == 2) {
      *(out++) = (buf >> 4) & 0xff;
    }
  }

  *num_out = (out - out_start);
  return 0;
}

string Base64Encode::DecodeUrl(const string &input) {
  size_t num_decoded = DecodeBytesNeeded(input.size());
  str_ptr decoded(new char[num_decoded]);
  if (DecodeUrl(input.c_str(), input.size(), decoded.get(), &num_decoded)) {
    return "";
  }
  return string(decoded.get(), num_decoded);
}

