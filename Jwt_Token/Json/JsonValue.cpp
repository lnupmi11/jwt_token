#include "../Include/JsonValue.h"

// Macros to free an array/object
#define FREE_ARRAY(x) do {for (auto&& item : (x)) delete item; } while(false)
#define FREE_OBJECT(x) do {for (auto iter = begin(x); iter != end(x); ++iter) delete (*iter).second; } while(false)

ulong countSubstr(const wstring& s, const wstring& sub)
{
	int count = 0;
	size_t nPos = s.find(sub, 0); // fist occurrence
	while (nPos != string::npos)
	{
		count++;
		nPos = s.find(sub, nPos + 1);
	}
	return nPos;
}


wstring JSONValue::removeWhitespace(const wstring& data)
{
	const wstring whitespace = L" \t\r\n\v\b\f";
	wstring result;
	bool quote_found = false;
	for (ulong i = 0; i < data.length(); ++i)
	{
		if (data[i] == L'"')
		{
			if (quoteClosed(data.substr(0, i + 1)))
			{
				quote_found = !quote_found;
			}
		}
		if (quote_found || whitespace.find(data[i]) == -1)
		{
			result += data[i];
		}
	}
	return result;
}

bool JSONValue::quoteClosed(const wstring& data)
{
	wstring last_item = data;
	ulong found_pos;
	found_pos = last_item.find(L"\\\\");
	while (found_pos != -1UL)
	{
		last_item.replace(found_pos, 2, L"..");
		found_pos = last_item.find(L"\\\\");
	}
	found_pos = last_item.find(L"\\\"");
	while (found_pos != -1UL)
	{
		last_item.replace(found_pos, 2, L"..");
		found_pos = last_item.find(L"\\\"");
	}
//	wcout << "last_item=`" << last_item << '`' << endl;
	found_pos = last_item.length() - 1;
	return last_item.empty() || last_item[found_pos] == L'"';
}

JSONValue* JSONValue::parse(wstring data)
{
	JSONValue* result;
	data = removeWhitespace(data);
//	wcout << "trimmed=`" << data << "`" << endl;
	if (data[0] == '"')
	{
		wstring str = extractString(data);
//		wcout << "string" << endl;
		result = new JSONValue(str);
	} else if (data == L"true" || data == L"false")
	{
//		wcout << "bool" << endl;
		result = new JSONValue(data == L"true");
	} else if (data == L"null")
	{
//		wcout << "null" << endl;
		result = new JSONValue();
	} else if (data[0] == L'-' || data[0] >= L'0' && data[0] <= L'9')
	{
//		wcout << "number" << endl;
		try
		{
			double num = stod(data);
			result = new JSONValue(num);
		} catch (const invalid_argument& e)
		{
			result = nullptr;
		}
	} else if (data[0] == L'[')
	{
//		wcout << "array" << endl;
		JSONArray array;
		ulong it = 1, start = 1;
		uint bracketOpened = 0;
		uint bracesOpened = 0;

		bool quote_found = false;

		bool error = false;

		while (it != data.length())
		{
//			wcout << "data[" << it << "]=`" << data[it] << '`'
//				  << (quote_found ? " (q)" : "")
//				  << (bracketOpened ? L" ([=" + to_wstring(bracketOpened) + L")" : L"")
//				  << (bracesOpened ? L" ({=" + to_wstring(bracesOpened) + L")" : L"")
//				  << endl;
			if (data[it] == L'"')
			{
				if (quoteClosed(data.substr(start, it - start + 1)))
					quote_found = !quote_found;
			} else if (!quote_found && data[it] == L'[')
			{
				++bracketOpened;
			} else if (!quote_found && data[it] == L'{')
			{
				++bracesOpened;
			} else if (!quote_found && data[it] == L'}')
			{
				--bracesOpened;
			} else if (!quote_found && data[it] == L']')
			{
				if (bracketOpened > 0)
				{
					--bracketOpened;
				} else if (bracesOpened == 0 && bracketOpened == 0 && data[it - 1] != L',' && it != 1)
				{
					wstring chunk = data.substr(start, it - start);
//					wcout << "chunk=" << chunk << endl;
					JSONValue* value = parse(chunk);
					if (value == nullptr)
					{
						FREE_ARRAY(array);
						error = true;
						break;
					} else
					{
						array.emplace_back(value);
					}
					break;
				}
			} else if (!quote_found && bracesOpened == 0 && bracketOpened == 0 && data[it] == L',')
			{
				if (data[it - 1] == L',')
				{
					FREE_ARRAY(array);
					error = true;
					break;
				}
				wstring chunk = data.substr(start, it - start);
//				wcout << "chunk=" << chunk << endl;
				start = it + 1;
				JSONValue* value = parse(chunk);
				if (value == nullptr)
				{
					FREE_ARRAY(array);
					error = true;
					break;
				} else
				{
					array.emplace_back(value);
				}
			}
			++it;
		}
		if (error)
			result = nullptr;
		else
			result = new JSONValue(array);
	} else if (data[0] == L'{')
	{
//		wcout << "object" << endl;
		JSONObject object;

		ulong it = 1, start = 1;
		uint bracketOpened = 0;
		uint bracesOpened = 0;
		bool quote_found = false;
		ulong colon_pos = 0;
		bool error = false;

		while (it != data.length())
		{
//			wcout << "data[" << it << "]=`" << data[it] << '`'
//				  << (quote_found ? " (q)" : "")
//				  << (bracketOpened ? L" ([=" + to_wstring(bracketOpened) + L")" : L"")
//				  << (bracesOpened ? L" ({=" + to_wstring(bracesOpened) + L")" : L"")
//				  << endl;
			if (data[it] == L'"')
			{
				if (quoteClosed(data.substr(start, it - start + 1)))
					quote_found = !quote_found;
			} else if (!quote_found && data[it] == L'{')
			{
				++bracesOpened;
			} else if (!quote_found && data[it] == L'[')
			{
				++bracketOpened;
			} else if (!quote_found && data[it] == L']')
			{
				--bracketOpened;
			} else if (!quote_found && data[it] == L':')
			{
				colon_pos = it;
			} else if (!quote_found && data[it] == L'}')
			{
//				wcout << "closing tag" << endl;
				if (bracesOpened > 0)
				{
					--bracesOpened;
				} else if (data[it - 1] != L',' && it != 1)
				{
//					wcout << "colon_pos=" << colon_pos << endl;
					if (colon_pos == 0 || data[colon_pos] != L':')
					{
						FREE_OBJECT(object);
						error = true;
						break;
					}
					wstring chunk_key = data.substr(start, colon_pos - start);
					wstring key = extractString(chunk_key);
					start = colon_pos + 1;
					wstring chunk_value = data.substr(start, it - start);

					JSONValue* value = parse(chunk_value);
					if (value == nullptr)
					{
						FREE_OBJECT(object);
						error = true;
						break;
					} else
					{
						object.emplace(key, value);
					}
					break;
				}
			} else if (!quote_found && (bracketOpened == 0 && bracesOpened == 0) && data[it] == L',')
			{
//				wcout << "colon_pos=" << colon_pos << endl;
				if (data[it - 1] == L',' || colon_pos == 0 || data[colon_pos] != L':')
				{
					FREE_OBJECT(object);
					error = true;
					break;
				}
				wstring chunk_key = data.substr(start, colon_pos - start);
				wstring key = extractString(chunk_key);
				start = colon_pos + 1;
				wstring chunk_value = data.substr(start, it - start);
				start = it + 1;

				JSONValue* value = parse(chunk_value);
				if (value == nullptr)
				{
					FREE_OBJECT(object);
					error = true;
					break;
				} else
				{
					object.emplace(key, value);
				}
			}

			++it;
		}
		if (error)
			result = nullptr;
		else
			result = new JSONValue(object);
	} else
	{
//		wcout << "error" << endl;
		result = nullptr;
	}
//	if (result == nullptr)
//		wcout << "error_data=\"" << data << '"' << endl;
	return result;
}

JSONValue* JSONValue::parse(string data)
{
	return parse(wstring(data.begin(), data.end()));
}

JSONValue::JSONValue(/*null*/)
{
	this->type = JSONType_Null;
}

JSONValue::JSONValue(const char* m_cstring_value)
{
	string bytes = string(m_cstring_value);
	type = JSONType_String;
	string_value = new wstring(bytes.begin(), bytes.end());
}

JSONValue::JSONValue(const wchar_t* m_wcstring_value)
{
	type = JSONType_String;
	string_value = new wstring(m_wcstring_value);
}

JSONValue::JSONValue(const string& m_string_value)
{
	type = JSONType_String;
	string_value = new wstring(m_string_value.begin(), m_string_value.end());
}

JSONValue::JSONValue(const wstring& m_string_value)
{
	type = JSONType_String;
	string_value = new wstring(m_string_value);
}

JSONValue::JSONValue(bool m_bool_value)
{
	type = JSONType_Bool;
	bool_value = m_bool_value;
}

JSONValue::JSONValue(double m_number_value)
{
	type = JSONType_Number;
	number_value = m_number_value;
}

JSONValue::JSONValue(int m_integer_value)
{
	type = JSONType_Number;
	number_value = (double) m_integer_value;
}

JSONValue::JSONValue(const JSONArray& m_array_value)
{
	type = JSONType_Array;
	array_value = new JSONArray(m_array_value);
}

JSONValue::JSONValue(const JSONObject& m_object_value)
{
	type = JSONType_Object;
	object_value = new JSONObject(m_object_value);
}

JSONValue::JSONValue(const JSONValue& source)
{
	this->type = source.type;
}

JSONValue::~JSONValue()
{
	if (isString())
	{
		delete string_value;
	} else if (isArray())
	{
		FREE_ARRAY(*array_value);
		delete array_value;
	} else if (isObject())
	{
		FREE_OBJECT(*object_value);
		delete object_value;
	}
}

bool JSONValue::isNull() const
{
	return type == JSONType_Null;
}

bool JSONValue::isString() const
{
	return type == JSONType_String;
}

bool JSONValue::isBool() const
{
	return type == JSONType_Bool;
}

bool JSONValue::isNumber() const
{
	return type == JSONType_Number;
}

bool JSONValue::isArray() const
{
	return type == JSONType_Array;
}

bool JSONValue::isObject() const
{
	return type == JSONType_Object;
}

const std::wstring& JSONValue::asString() const
{
	return *this->string_value;
}

bool JSONValue::asBool() const
{
	return this->bool_value;
}

double JSONValue::asNumber() const
{
	return this->number_value;
}

const JSONArray& JSONValue::asArray() const
{
	return *this->array_value;
}

const JSONObject& JSONValue::asObject() const
{
	return *this->object_value;
}

std::size_t JSONValue::countChildren() const
{
	return 0;
}

bool JSONValue::hasChild(std::size_t index) const
{
	return false;
}

JSONValue* JSONValue::child(std::size_t index)
{
	return nullptr;
}

bool JSONValue::hasChild(const wchar_t* name) const
{
	return false;
}

JSONValue* JSONValue::child(const wchar_t* name)
{
	return nullptr;
}

vector<wstring> JSONValue::objectKeys() const
{
	return vector<wstring>();
}

wstring JSONValue::stringify(const uint& indent_size, const uint& indent_depth) const
{
	wstring result;
	wstring indent;

	indent = getIndent(indent_size, indent_depth);

	switch (type)
	{
		case JSONType_Null:
			result += L"null";
			break;

		case JSONType_String:
			result += stringifyString(*string_value);
			break;

		case JSONType_Bool:
			result += bool_value ? L"true" : L"false";
			break;
		case JSONType_Number:
		{
			if (isinf(number_value) || isnan(number_value))
				result += L"null";
			else
			{
				wstringstream ss;
				ss.precision(15);
				ss << number_value;
				result += ss.str();
			}
			break;
		}
		case JSONType_Array:
		{
			result += L'[';
			if (array_value->empty())
			{
				result += L']';
				break;
			}
			wstring shift_indent;
			if (indent_size > 0)
				shift_indent = getIndent(indent_size, indent_depth + 1);

			auto iter = begin(*array_value);
			while (iter != end(*array_value))
			{
				if (indent_size > 0)
				{
					result += shift_indent;
				}
				result += (*iter)->stringify(indent_size, indent_depth + 1);

				if (next(iter) != array_value->end())
					result += L",";

				++iter;
			}

			if (indent_size > 0)
				result += (indent_depth > 0 ? indent : L"\n");

			result += L"]";
			break;
		}
		case JSONType_Object:
		{
			result += L'{';
			if (object_value->empty())
			{
				result += L'}';
				break;
			}

			wstring shift_indent;
			if (indent_size > 0)
				shift_indent = getIndent(indent_size, indent_depth + 1);
			for (auto iter = begin(*object_value); iter != end(*object_value); ++iter)
			{
				if (indent_size > 0)
				{
					result += shift_indent;
				}

				result += stringifyString(iter->first);
				result += indent_size > 0 ? L": " : L":";
				result += iter->second->stringify(indent_size, indent_depth + 1);
				if (next(iter) != end(*object_value))
					result += L",";
			}

			if (indent_size > 0)
				result += (indent_depth > 0 ? indent : L"\n");

			result += L"}";
			break;
		}
		default:
			break;
	}
	return result;
}

wstring JSONValue::getIndent(const uint& indent_size, const uint& indent_depth) const
{
	wstring indent;
	if (indent_size > 0 && indent_depth > 0)
	{
		indent = L"\n";
		for (int i = 0; i < indent_size * indent_depth; ++i)
		{
			indent += L' ';
		}
	}
	return indent;
}

wstring JSONValue::stringifyString(const wstring& str)
{
	wstring result = L"\"";

	auto iter = str.begin();
	while (iter != str.end())
	{
		wchar_t chr = *iter;

		switch (chr)
		{
			case L'"':
			case L'\\':
				result += L'\\';
				result += chr;
				break;
			case L'\b':
				result += L"\\b";
				break;
			case L'\f':
				result += L"\\f";
				break;
			case L'\n':
				result += L"\\n";
				break;
			case L'\r':
				result += L"\\r";
				break;
			case L'\t':
				result += L"\\t";
				break;
			default:
				if (chr < L' ' || chr > 126)
				{
					result += L"\\u";
					for (int i = 0; i < 4; i++)
					{
						int value = (chr >> 12) & 0xf;
						if (value >= 0 && value <= 9)
							result += (wchar_t) ('0' + value);
						else if (value >= 10 && value <= 15)
							result += (wchar_t) ('A' + (value - 10));
						chr <<= 4;
					}
				} else
				{
					result += chr;
				}
		}

		iter++;
	}

	result += L"\"";
	return result;
}

wstring JSONValue::extractString(const wstring& data)
{
	wstring result;
	if (data[0] != L'"')
	{
		return L"";
	}
	ulong it = 1;
	while (it != data.length())
	{
		// Save the char so we can change it if need be
		wchar_t next_char = data[it];
		// Escaping something?
		if (next_char == L'\\')
		{
			// Move over the escape char
			++it;
			// Deal with the escaped char
			switch (data[it])
			{
				case L'"':
					next_char = L'"';
					break;
				case L'\\':
					next_char = L'\\';
					break;
				case L'b':
					next_char = L'\b';
					break;
				case L'f':
					next_char = L'\f';
					break;
				case L'n':
					next_char = L'\n';
					break;
				case L'r':
					next_char = L'\r';
					break;
				case L't':
					next_char = L'\t';
					break;
				case L'u':
				{
					// We need 5 chars (4 hex + the 'u') or its not valid
					if (data.length() - it < 5)
						return L"";

					// Deal with the chars
					next_char = 0;
					for (int i = 0; i < 4; i++)
					{
						// Do it first to move off the 'u' and leave us on the
						// final hex digit as we move on by one later on
						++it;

						next_char <<= 4;

						// Parse the hex digit
						if (data[it] >= '0' && data[it] <= '9')
							next_char |= (data[it] - '0');
						else if (data[it] >= 'A' && data[it] <= 'F')
							next_char |= (10 + (data[it] - 'A'));
						else if (data[it] >= 'a' && data[it] <= 'f')
							next_char |= (10 + (data[it] - 'a'));
						else
						{
							// Invalid hex digit = invalid JSON
							return L"";
						}
					}
					break;
				}
				default:
					next_char = data[it];
					break;
			}
		}
			// End of the string?
		else if (next_char == L'"')
		{
			result.reserve(); // Remove unused capacity
			break;
		}
			// Disallowed char?
		else if (next_char < L' ' && next_char != L'\t')
		{
			result = L"";
			break;
		}
		// Add the next char
		result += next_char;

		// Move on
		++it;
	}
//	wcout << '`' << data << "` --> `" << result << '`' << endl;
	return result;
}

