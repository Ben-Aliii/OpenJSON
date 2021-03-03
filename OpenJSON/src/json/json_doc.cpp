/*

Copyright 2021 (C) Benali Louarrani <mtlm3014@gmail.com>

This file is part of OpenJSON.

OpenJSON is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenJSON is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with OpenJSON.  If not, see <https://www.gnu.org/licenses/>.

*/

#include <json/json_doc.h>

//////////////////////////////////////////////////////////////////////////
// json_token
//////////////////////////////////////////////////////////////////////////

enum class json_token_type : uint8_t
{
	unknown = 0,
	obj_start,		// {
	obj_end,		// }
	array_start,	// [
	array_end,		// ]
	comma,			// ,
	colon,			// :
	literal_true,
	literal_false,
	literal_null,
	value_number,
	value_string
};

struct json_token
{
	json_token_type type;
	std::string content;

	json_token(json_token_type t, const std::string& ctn = "")
		: type(t), content(ctn)
	{
	}
};

std::ostream& operator<<(std::ostream& stream, const json_token& tk)
{
	switch (tk.type)
	{
	case json_token_type::unknown:
		stream << "unknown        ";
		break;
	case json_token_type::obj_start:
		stream << "obj_start      ";
		break;
	case json_token_type::obj_end:
		stream << "obj_end        ";
		break;
	case json_token_type::array_start:
		stream << "array_start    ";
		break;
	case json_token_type::array_end:
		stream << "array_end      ";
		break;
	case json_token_type::comma:
		stream << "comma          ";
		break;
	case json_token_type::colon:
		stream << "colon          ";
		break;
	case json_token_type::literal_true:
		stream << "literal_true   ";
		break;
	case json_token_type::literal_false:
		stream << "literal_false  ";
		break;
	case json_token_type::literal_null:
		stream << "literal_null   ";
		break;
	case json_token_type::value_number:
		stream << "value_number   ";
		break;
	case json_token_type::value_string:
		stream << "value_string   ";
		break;
	}

	if (tk.content == "")
		stream << "<empty>";
	else
		stream << tk.content;
	return stream;
}

//////////////////////////////////////////////////////////////////////////
// globals
//////////////////////////////////////////////////////////////////////////

static parse_mode g_mode;
static std::vector<json_token> g_tokens;
static int g_index;

//////////////////////////////////////////////////////////////////////////
// lexical analysis
//////////////////////////////////////////////////////////////////////////

static inline bool is_whitespace(char c)
{
	return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

static inline bool is_ctrl(char c)
{
	return (c >= 0x00 && c <= 0x1f) || c == 0x7f;
}

static inline bool is_hex(char c)
{
	return isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

static bool lexical_analysis(const char *str)
{
	g_tokens.clear();
	char c;
	size_t i = 0;
	size_t state = 0;
	bool success = true;
	std::string ctn;
	while ((c = str[i++]) != '\0' && success)
	{
		switch (state)
		{
		case 0:
			if (c == '{')
				g_tokens.emplace_back(json_token_type::obj_start);
			else if (c == '}')
				g_tokens.emplace_back(json_token_type::obj_end);
			else if (c == '[')
				g_tokens.emplace_back(json_token_type::array_start);
			else if (c == ']')
				g_tokens.emplace_back(json_token_type::array_end);
			else if (c == ':')
				g_tokens.emplace_back(json_token_type::colon);
			else if (c == ',')
				g_tokens.emplace_back(json_token_type::comma);
			else if (c == '\"')
				state = 1;
			else if (c == '-' || (isdigit(c) /*&& c != '0'*/))
			{
				ctn += c;
				state = 7;
			}
			else if (isalpha(c))
			{
				ctn += c;
				state = 11;
			}
			else if (g_mode == parse_mode::permissive && c == '/') // permissive mode
				state = 12;
			else if (g_mode == parse_mode::permissive && c == '\'') // permissive mode
				state = 16;
			else if (!is_whitespace(c))
			{
				std::cout << c << " here\n";
				g_tokens.emplace_back(json_token_type::unknown, "unexpected character");
				success = false;
			}
			break;
		case 1:
			if (c == '\"')
			{
				g_tokens.emplace_back(json_token_type::value_string, ctn);
				ctn = "";
				state = 0;
			}
			else if (c == '\\')
				state = 2;
			else if (is_ctrl(c))
			{
				g_tokens.emplace_back(json_token_type::unknown, "control character in string value");
				success = false;
			}
			else
				ctn += c;
			break;
		case 2:
			if (c == '\"')
			{
				ctn += '\"';
				state = 1;
			}
			else if (c == '\\')
			{
				ctn += '\\';
				state = 1;
			}
			else if (c == '/')
			{
				ctn += '/';
				state = 1;
			}
			else if (c == 'b')
			{
				ctn += '\b';
				state = 1;
			}
			else if (c == 'f')
			{
				ctn += '\f';
				state = 1;
			}
			else if (c == 'n')
			{
				ctn += '\n';
				state = 1;
			}
			else if (c == 'r')
			{
				ctn += '\r';
				state = 1;
			}
			else if (c == 't')
			{
				ctn += '\t';
				state = 1;
			}
			else if (c == 'u')
				state = 3;
			else
			{
				g_tokens.emplace_back(json_token_type::unknown, "unknown character");
				state = 1;
			}
			break;
		case 3:
			if (is_hex(c))
			{
				ctn += c;
				state = 4;
			}
			else
			{
				g_tokens.emplace_back(json_token_type::unknown, "unexpected character");
				state = 1;
			}
			break;
		case 4:
			if (is_hex(c))
			{
				ctn += c;
				state = 5;
			}
			else
			{
				g_tokens.emplace_back(json_token_type::unknown, "unexpected character");
				state = 1;
			}
			break;
		case 5:
			if (is_hex(c))
			{
				ctn += c;
				state = 6;
			}
			else
			{
				g_tokens.emplace_back(json_token_type::unknown, "unexpected character");
				state = 1;
			}
			break;
		case 6:
			if (is_hex(c))
			{
				ctn += c;
				state = 1;
			}
			else
			{
				g_tokens.emplace_back(json_token_type::unknown, "unexpected character");
				state = 1;
			}
			break;
		case 7:
			if (isdigit(c))
				ctn += c;
			else if (c == '.')
			{
				ctn += c;
				state = 8;
			}
			else
			{
				g_tokens.emplace_back(json_token_type::value_number, ctn);
				ctn = "";
				state = 0;
				i--;
			}
			break;
		case 8:
			if (isdigit(c))
				ctn += c;
			else if (c == 'e' || c == 'E')
			{
				ctn += c;
				state = 9;
			}
			else
			{
				g_tokens.emplace_back(json_token_type::value_number, ctn);
				ctn = "";
				state = 0;
				i--;
			}
			break;
		case 9:
			if (isdigit(c))
			{
				ctn += c;
				state = 10;
			}
			else if (c == '-' || c == '+')
			{
				ctn += c;
				state = 10;
			}
			else
			{
				g_tokens.emplace_back(json_token_type::unknown, "unexpected character");
				state = 0;
			}
			break;
		case 10:
			if (isdigit(c))
				ctn += c;
			else
			{
				g_tokens.emplace_back(json_token_type::value_number, ctn);
				ctn = "";
				state = 0;
				i--;
			}
			break;
		case 11:
			if (!isalpha(c))
			{
				if (ctn == "true")
					g_tokens.emplace_back(json_token_type::literal_true);
				else if (ctn == "false")
					g_tokens.emplace_back(json_token_type::literal_false);
				else if (ctn == "null")
					g_tokens.emplace_back(json_token_type::literal_null);
				else
					g_tokens.emplace_back(json_token_type::unknown, ctn);
				ctn = "";
				state = 0;
				i--;
			}
			else
				ctn += c;
			break;
		case 12:
			if (c == '/')
				state = 13;
			else if (c == '*')
				state = 14;
			else
				success = false;
			break;
		case 13: //single line comments
			if (c == '\n' || c == '\r')
				state = 0;
			break;
		case 14: // multiline comments
			if (c == '*')
				state = 15;
			break;
		case 15:
			if (c == '/')
				state = 0;
			else
				state = 14;
			break;
		case 16: //single quoted strings
			if (c == '\'')
			{
				g_tokens.emplace_back(json_token_type::value_string, ctn);
				ctn = "";
				state = 0;
			}
			else if (c == '\\')
				state = 17;
			else if (is_ctrl(c))
			{
				g_tokens.emplace_back(json_token_type::unknown, "control character in string value");
				success = false;
			}
			else
				ctn += c;
			break;
		case 17:
			if (c == '\"')
			{
				ctn += '\"';
				state = 16;
			}
			else if (c == '\\')
			{
				ctn += '\\';
				state = 16;
			}
			else if (c == '/')
			{
				ctn += '/';
				state = 16;
			}
			else if (c == 'b')
			{
				ctn += '\b';
				state = 16;
			}
			else if (c == 'f')
			{
				ctn += '\f';
				state = 16;
			}
			else if (c == 'n')
			{
				ctn += '\n';
				state = 16;
			}
			else if (c == 'r')
			{
				ctn += '\r';
				state = 16;
			}
			else if (c == 't')
			{
				ctn += '\t';
				state = 16;
			}
			else if (c == 'u')
				state = 18;
			else
			{
				g_tokens.emplace_back(json_token_type::unknown, "unknown character");
				state = 16;
			}
			break;
		case 18:
			if (is_hex(c))
			{
				ctn += c;
				state = 19;
			}
			else
			{
				g_tokens.emplace_back(json_token_type::unknown, "unexpected character");
				state = 16;
			}
			break;
		case 19:
			if (is_hex(c))
			{
				ctn += c;
				state = 20;
			}
			else
			{
				g_tokens.emplace_back(json_token_type::unknown, "unexpected character");
				state = 16;
			}
			break;
		case 20:
			if (is_hex(c))
			{
				ctn += c;
				state = 21;
			}
			else
			{
				g_tokens.emplace_back(json_token_type::unknown, "unexpected character");
				state = 16;
			}
			break;
		case 21:
			if (is_hex(c))
			{
				ctn += c;
				state = 16;
			}
			else
			{
				g_tokens.emplace_back(json_token_type::unknown, "unexpected character");
				state = 16;
			}
			break;
		}
	}
	return success && (state == 0 || state == 13);
}

//////////////////////////////////////////////////////////////////////////
// syntax analysis
//////////////////////////////////////////////////////////////////////////

static bool parse_array(json_array& arr);
static bool parse_object(json_object& obj);

static json_token& next()
{
	return g_tokens[g_index++];
}

static bool parse_array(json_array& arr)
{
	if (next().type != json_token_type::array_start)
		return false;

	if (next().type == json_token_type::array_end)
		return true;
	else
		g_index--;

	do
	{
		json_token& _t = next();
		if (_t.type == json_token_type::value_number)
			arr.add(std::stof(_t.content));
		else if (_t.type == json_token_type::value_string)
			arr.add(_t.content);
		else if (_t.type == json_token_type::literal_true)
			arr.add(true);
		else if (_t.type == json_token_type::literal_false)
			arr.add(false);
		else if (_t.type == json_token_type::literal_null)
			arr.add(json_var());
		else if (_t.type == json_token_type::obj_start)
		{
			g_index--;
			arr.add(json_object());
			if (!parse_object(arr[arr.count() - 1]))
				return false;
		}
		else if (_t.type == json_token_type::array_start)
		{
			g_index--;
			json_array _a = {};
			arr.add(_a);
			if (!parse_array(arr[arr.count() - 1]))
				return false;
		}
		else
			return false;
	} while (next().type == json_token_type::comma);

	g_index--;
	if (next().type != json_token_type::array_end)
		return false;
	return true;
}

static bool parse_object(json_object& obj)
{
	if (next().type != json_token_type::obj_start)
		return false;

	if (next().type == json_token_type::obj_end)
		return true;
	else
		g_index--;

	do
	{
		json_token& _t = next();
		if (_t.type != json_token_type::value_string)
			return false;

		std::string _key = _t.content;
		_t = next();
		if (_t.type != json_token_type::colon)
			return false;

		_t = next();
		if (_t.type == json_token_type::value_number)
			obj[_key] = std::stof(_t.content);
		else if (_t.type == json_token_type::value_string)
			obj[_key] = _t.content;
		else if (_t.type == json_token_type::literal_true)
			obj[_key] = true;
		else if (_t.type == json_token_type::literal_false)
			obj[_key] = false;
		else if (_t.type == json_token_type::literal_null)
			obj[_key] = json_var();
		else if (_t.type == json_token_type::obj_start)
		{
			g_index--;
			obj[_key] = json_object();
			if (!parse_object(obj[_key]))
				return false;
		}
		else if (_t.type == json_token_type::array_start)
		{
			g_index--;
			obj[_key] = {};
			if (!parse_array(obj[_key]))
				return false;
		}
		else
			return false;
	} while (next().type == json_token_type::comma);
	g_index--;
	if (next().type != json_token_type::obj_end)
		return false;
	return true;
}

static bool syntax_analysis(json_object& obj)
{
	g_index = 0;
	if (!parse_object(obj))
	{
		obj = json_object();
		return false;
	}
	return true;
}

static bool syntax_analysis(json_var& var)
{
	var = json_object();
	return syntax_analysis(var.to_object());
}

//////////////////////////////////////////////////////////////////////////
// json_doc
//////////////////////////////////////////////////////////////////////////

parse_mode json_doc::mode;

void json_doc::save(json_object& obj, const char *file)
{
	std::ofstream s(file);
	JSON_ASSERT(s.is_open(), "cannot open file");
	s << obj;
	s.close();
}

void json_doc::save(json_object& obj, const std::string& file)
{
	save(obj, file.c_str());
}

bool json_doc::load_file(const char *file, json_object& obj)
{
	std::string content;
	std::ifstream s(file);

	JSON_ASSERT(s.is_open(), "cannot open file");

	s.seekg(0, std::ios::end);
	content.reserve((unsigned int)s.tellg());
	s.seekg(0, std::ios::beg);
	content.assign(std::istreambuf_iterator<char>(s), std::istreambuf_iterator<char>());

	return load(content, obj);
}

bool json_doc::load_file(const std::string& file, json_object& obj)
{
	return load_file(file.c_str(), obj);
}

bool json_doc::load(const char *str, json_object& obj)
{
	g_mode = mode;

	bool success = false;
	success = lexical_analysis(str);

	if (!success)
	{
		std::cout << "lexical error(s).\n";
		return false;
	}

	success = syntax_analysis(obj);

	//std::cout << "tokens.size() = " << tokens.size() << "\n";
	//for (int i = 0; i < tokens.size(); i++)
	//	std::cout << tokens[i] << "\n";

	if (!success)
	{
		std::cout << "syntax error(s).\n";
		return false;
	}

	return success;
}

bool json_doc::load(const std::string& str, json_object& obj)
{
	return load(str.c_str(), obj);
}

bool json_doc::load_file(const char *file, json_var& var)
{
	var = json_object();
	return load_file(file, var.to_object());
}

bool json_doc::load_file(const std::string& file, json_var& var)
{
	var = json_object();
	return load_file(file, var.to_object());
}

bool json_doc::load(const char *str, json_var& var)
{
	var = json_object();
	return load(str, var.to_object());
}

bool json_doc::load(const std::string& str, json_var& var)
{
	var = json_object();
	return load(str, var.to_object());
}

json_object operator""_json(const char *str, size_t size)
{
	json_object _obj;
	return json_doc::load(str, _obj) ? _obj : json_object();
}