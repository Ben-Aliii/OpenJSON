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

#ifndef JSON_VARS_H_INCLUDED
#define JSON_VARS_H_INCLUDED

#include "json_types.h"

//////////////////////////////////////////////////////////////////////////
//	json_string
//////////////////////////////////////////////////////////////////////////

struct json_string
{
private:
	char *m_string;
	size_t m_length;

public:
	json_string();
	json_string(const char *str);
	json_string(const std::string& str);
	json_string(const json_string& str);
	~json_string();

	inline const char* get() const { return m_string; }
	inline size_t size() const { return m_length; }

	void operator=(const char *str);
	void operator=(const std::string& str);
	void operator=(const json_string& str);
	bool operator==(const char *str);
	bool operator==(const std::string& str);
	bool operator==(const json_string& str);
	bool operator!=(const char *str);
	bool operator!=(const std::string& str);
	bool operator!=(const json_string& str);

	operator std::string() { return std::string(m_string); }
};

//////////////////////////////////////////////////////////////////////////
//	json_array
//////////////////////////////////////////////////////////////////////////

struct json_array
{
private:
	std::vector<json_var> m_data;

public:
	json_array(const std::initializer_list<json_var>& list);

	void add(const json_var& var);
	void remove(size_t index);
	json_var& get(size_t index);

	inline size_t count() const { return m_data.size(); }

	json_var& operator[](size_t index);
};

//////////////////////////////////////////////////////////////////////////
//	json_object
//////////////////////////////////////////////////////////////////////////

struct json_object
{
private:
	std::vector<std::string> m_keys;
	std::vector<json_var> m_vars;

public:
	json_object();

	json_var& get(const std::string& key);
	const std::string& get_key(size_t index);

	inline size_t count() const { return m_keys.size(); }

	json_var& operator[](size_t index);
	json_var& operator[](const std::string& key);
};

//////////////////////////////////////////////////////////////////////////
//	json_var
//////////////////////////////////////////////////////////////////////////

struct json_var
{
	json_var();
	json_var(const std::nullptr_t& t);
	json_var(const json_var& var);
	json_var(const json_object& obj);
	json_var(const std::initializer_list<json_var>& list);
	json_var(const json_array& arr);
	json_var(const char *str);
	json_var(const std::string& str);
	json_var(const json_string& str);
	json_var(const json_boolean boolean);
	json_var(const json_number number);

	inline bool is_null()		const { return type == json_type::null; }
	inline bool is_object()		const { return type == json_type::object; }
	inline bool is_array()		const { return type == json_type::array; }
	inline bool is_string()		const { return type == json_type::string; }
	inline bool is_number()		const { return type == json_type::number; }
	inline bool is_boolean()	const { return type == json_type::boolean; }

	inline json_object&		to_object()		{ JSON_ASSERT(is_object(),	"json_var : not an object"); return *value.object; }
	inline json_array&		to_array()		{ JSON_ASSERT(is_array(),	"json_var : not an array");	 return *value.array; }
	inline json_string&		to_string()		{ JSON_ASSERT(is_string(),	"json_var : not a string");  return *value.string; }
	inline json_boolean		to_boolean()	{ JSON_ASSERT(is_boolean(), "json_var : not a bool");	 return value.boolean; }
	inline json_number		to_number()		{ JSON_ASSERT(is_number(),	"json_var : not a number");  return value.number; }

	json_var& get(size_t index);
	json_var& get(const std::string& key);
	const std::string& get_key(size_t index);

	void operator=(const std::nullptr_t& t);
	void operator=(const json_var& var);
	void operator=(const json_object& obj);
	void operator=(const std::initializer_list<json_var>& list);
	void operator=(const json_array& arr);
	void operator=(const char *str);
	void operator=(const std::string& str);
	void operator=(const json_string& str);
	void operator=(json_boolean boolean);
	void operator=(json_number number);

	operator json_object&();
	operator json_array&();
	operator json_string&();
	operator json_boolean();
	operator json_number();

	json_var& operator[](size_t index);
	json_var& operator[](const std::string& key);

	json_type type;
	json_value value;
};

//////////////////////////////////////////////////////////////////////////
//	operators
//////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& stream, json_string& str);
std::ostream& operator<<(std::ostream& stream, json_array& arr);
std::ostream& operator<<(std::ostream& stream, json_object& obj);
std::ostream& operator<<(std::ostream& stream, json_var& var);

#endif //JSON_VARS_H_INCLUDED