#include "../include/Json.h"

JSON::JSON()
{

}

JSONValue *JSON::parse(const char *data)
{
	size_t length = strlen(data) + 1;
	wchar_t *w_data = (wchar_t*)malloc(length * sizeof(wchar_t));
	size_t ret_value = 0;
	if (mbstowcs_s(&ret_value, w_data, length, data, length) != 0)
	{
		free(w_data);
		return nullptr;
	}
	JSONValue *value = JSON::parse(w_data);
	free(w_data);
	return value;
}

JSONValue *JSON::parse(const wchar_t *data)
{
	if (!skipWhitespace(&data))
	{
		return nullptr;
	}

	JSONValue *value = JSONValue::parse(data);
	if (value == nullptr)
	{
		return nullptr;
	}

	if (skipWhitespace(&data))
	{
		delete value;
		return nullptr;
	}

	return value;
}

bool JSON::skipWhitespace(const wchar_t **data)
{
	while (**data != 0 && (**data == L' ' || **data == L'\t' || **data == L'\r' || **data == L'\n'))
	{
		(*data)++;
	}
	return **data != 0;
}

bool JSON::extractString(const wchar_t **data, std::wstring &str)
{
	str = L"";

	while (**data != 0)
	{
		wchar_t next_char = **data;

		if (next_char == L'\\')
		{
			(*data)++;

			switch (**data)
			{
				case L'"': next_char = L'"'; break;
				case L'\\': next_char = L'\\'; break;
				case L'/': next_char = L'/'; break;
				case L'b': next_char = L'\b'; break;
				case L'f': next_char = L'\f'; break;
				case L'n': next_char = L'\n'; break;
				case L'r': next_char = L'\r'; break;
				case L't': next_char = L'\t'; break;
				case L'u':
				{
					if (!simplejson_wcsnlen(*data, 5))
					{
						return false;
					}

					next_char = 0;
					for (int i = 0; i < 4; i++)
					{
						(*data)++;

						next_char <<= 4;

						if (**data >= '0' && **data <= '9')
							next_char |= (**data - '0');
						else if (**data >= 'A' && **data <= 'F')
							next_char |= (10 + (**data - 'A'));
						else if (**data >= 'a' && **data <= 'f')
							next_char |= (10 + (**data - 'a'));
						else
						{
							return false;
						}
					}
					break;
				}

				default:
					return false;
			}
		}

		else if (next_char == L'"')
		{
			(*data)++;
			str.reserve();
			return true;
		}

		else if (next_char < L' ' && next_char != L'\t')
		{
			return false;
		}

		str += next_char;

		(*data)++;
	}
	return false;
}

double JSON::parseInt(const wchar_t **data)
{
	double integer = 0;
	while (**data != 0 && **data >= '0' && **data <= '9')
	{
		integer = integer * 10 + (*(*data)++ - '0');
	}

	return integer;
}

double JSON::parseDecimal(const wchar_t **data)
{
	double decimal = 0.0;
	double factor = 0.1;
	while (**data != 0 && **data >= '0' && **data <= '9')
	{
		int digit = (*(*data)++ - '0');
		decimal = decimal + digit * factor;
		factor *= 0.1;
	}
	return decimal;
}