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

#include <json/json_vars.h>

//////////////////////////////////////////////////////////////////////////
//	json_string
//////////////////////////////////////////////////////////////////////////

json_string::json_string()
{
	m_string = nullptr;
	m_length = 0;
}

json_string::json_string(const char *str)
{
	m_length = strlen(str);
	m_string = new char[m_length + 1];
	std::strcpy(m_string, str);
}

json_string::json_string(const std::string& str)
{
	m_length = str.size();
	m_string = new char[m_length + 1];
	std::strcpy(m_string, str.c_str());
}

json_string::json_string(const json_string& str)
{
	m_length = str.m_length;
	m_string = new char[m_length + 1];
	std::strcpy(m_string, str.m_string);
}

json_string::~json_string()
{
	if (m_string != nullptr)
		delete[] m_string;
}

void json_string::operator=(const char *str)
{
	m_length = strlen(str);
	m_string = new char[m_length + 1];
	std::strcpy(m_string, str);
}

void json_string::operator=(const std::string& str)
{
	m_length = str.size();
	m_string = new char[m_length + 1];
	std::strcpy(m_string, str.c_str());
}

void json_string::operator=(const json_string& str)
{
	m_length = str.m_length;
	m_string = new char[m_length + 1];
	std::strcpy(m_string, str.m_string);
}

bool json_string::operator==(const char *str)
{
	return std::strcmp(m_string, str) == 0;
}

bool json_string::operator==(const std::string& str)
{
	return operator==(str.c_str());
}

bool json_string::operator==(const json_string& str)
{
	return operator==(str.m_string);
}

bool json_string::operator!=(const char *str)
{
	return !operator==(str);
}

bool json_string::operator!=(const std::string& str)
{
	return !operator==(str);
}

bool json_string::operator!=(const json_string& str)
{
	return !operator==(str);
}

//////////////////////////////////////////////////////////////////////////
//	json_array
//////////////////////////////////////////////////////////////////////////

json_array::json_array(const std::initializer_list<json_var>& list)
	: m_data(list)
{
}

void json_array::add(const json_var& var)
{
	m_data.emplace_back(var);
}

void json_array::remove(size_t index)
{
	JSON_ASSERT(index >= 0 && index < count(), "json_array : index out of range");
	m_data.erase(m_data.begin() + index);
}

json_var& json_array::get(size_t index)
{
	JSON_ASSERT(index >= 0 && index < count(), "json_array : index out of range");
	return m_data[index];
}

json_var& json_array::operator[](size_t index)
{
	JSON_ASSERT(index >= 0 && index < count(), "json_array : index out of range");
	return m_data[index];
}

//////////////////////////////////////////////////////////////////////////
//	json_object
//////////////////////////////////////////////////////////////////////////

json_object::json_object()
{
}

json_var& json_object::get(const std::string& key)
{
	size_t i;
	for (i = 0; i < m_keys.size(); i++)
		if (m_keys[i] == key)
			break;
	if (i == m_keys.size())
	{
		m_keys.emplace_back(key);
		m_vars.emplace_back();
	}
	return m_vars[i];
}

const std::string& json_object::get_key(size_t index)
{
	JSON_ASSERT(index >= 0 && index < count(), "json_object : index out of range");
	return m_keys[index];
}

json_var& json_object::operator[](size_t index)
{
	JSON_ASSERT(index >= 0 && index < count(), "json_object : index out of range");
	return m_vars[index];
}

json_var& json_object::operator[](const std::string& key)
{
	return get(key);
}

//////////////////////////////////////////////////////////////////////////
//	helper functions
//////////////////////////////////////////////////////////////////////////

void clean(json_var& var)
{
	if (var.type == json_type::object && var.value.object != nullptr)
		delete var.value.object;
	else if (var.type == json_type::array && var.value.array != nullptr)
		delete var.value.array;
	else if (var.type == json_type::string && var.value.string != nullptr)
		delete var.value.string;
}

json_object* create_object(const json_object& obj)
{
	return new json_object(obj);
}

json_array* create_array(const std::initializer_list<json_var>& list)
{
	return new json_array(list);
}

json_array* create_array(const json_array& arr)
{
	return new json_array(arr);
}

json_string* create_string(const char* str)
{
	return new json_string(str);
}

json_string* create_string(const std::string& str)
{
	return new json_string(str);
}

json_string* create_string(const json_string& str)
{
	return new json_string(str);
}

//////////////////////////////////////////////////////////////////////////
//	json_var
//////////////////////////////////////////////////////////////////////////

json_var::json_var()
{
	type = json_type::null;
}

json_var::json_var(const std::nullptr_t& t)
{
	type = json_type::null;
}

json_var::json_var(const json_var& var)
{
	clean(*this);
	type = var.type;
	if (type == json_type::object)
		value.object = create_object(*var.value.object);
	else if (type == json_type::array)
		value.array = create_array(*var.value.array);
	else if (type == json_type::string)
		value.string = create_string(*var.value.string);
	else if (type == json_type::boolean)
		value.boolean = var.value.boolean;
	else if (type == json_type::number)
		value.number = var.value.number;
}

json_var::json_var(const json_object& obj)
{
	type = json_type::object;
	value.object = create_object(obj);
}

json_var::json_var(const std::initializer_list<json_var>& list)
{
	type = json_type::array;
	value.array = create_array(list);
}

json_var::json_var(const json_array& arr)
{
	type = json_type::array;
	value.array = create_array(arr);
}

json_var::json_var(const char *str)
{
	type = json_type::string;
	value.string = create_string(str);
}

json_var::json_var(const std::string& str)
{
	type = json_type::string;
	value.string = create_string(str);
}

json_var::json_var(const json_string& str)
{
	type = json_type::string;
	value.string = create_string(str);
}

json_var::json_var(const json_boolean boolean)
{
	type = json_type::boolean;
	value.boolean = boolean;
}

json_var::json_var(const json_number number)
{
	type = json_type::number;
	value.number = number;
}

json_var& json_var::get(size_t index)
{
	JSON_ASSERT(is_array(), "json_var : not an array");
	return (*value.array)[index];
}

json_var& json_var::get(const std::string& key)
{
	JSON_ASSERT(is_object(), "json_var : not an object");
	return (*value.object).get(key);
}

const std::string& json_var::get_key(size_t index)
{
	JSON_ASSERT(is_object(), "json_var : not an object");
	return (*value.object).get_key(index);
}

void json_var::operator=(const std::nullptr_t & t)
{
	clean(*this);
	type = json_type::null;
}

void json_var::operator=(const json_var& var)
{
	clean(*this);
	type = var.type;
	if (type == json_type::object)
		value.object = create_object(*var.value.object);
	else if (type == json_type::array)
		value.array = create_array(*var.value.array);
	else if (type == json_type::string)
		value.string = create_string(*var.value.string);
	else if (type == json_type::boolean)
		value.boolean = var.value.boolean;
	else if (type == json_type::number)
		value.number = var.value.number;
}

void json_var::operator=(const json_object& obj)
{
	clean(*this);
	type = json_type::object;
	value.object = create_object(obj);
}

void json_var::operator=(const std::initializer_list<json_var>& list)
{
	clean(*this);
	type = json_type::array;
	value.array = create_array(list);
}

void json_var::operator=(const json_array& arr)
{
	clean(*this);
	type = json_type::array;
	value.array = create_array(arr);
}

void json_var::operator=(const char *str)
{
	clean(*this);
	type = json_type::string;
	value.string = create_string(str);
}

void json_var::operator=(const std::string& str)
{
	clean(*this);
	type = json_type::string;
	value.string = create_string(str);
}

void json_var::operator=(const json_string& str)
{
	clean(*this);
	type = json_type::string;
	value.string = create_string(str);
}

void json_var::operator=(json_boolean boolean)
{
	type = json_type::boolean;
	value.boolean = boolean;
}

void json_var::operator=(json_number number)
{
	type = json_type::number;
	value.number = number;
}

json_var::operator json_object&()
{
	return to_object();
}

json_var::operator json_array&()
{
	return to_array();
}

json_var::operator json_string&()
{
	return to_string();
}

json_var::operator json_boolean()
{
	return to_boolean();
}

json_var::operator json_number()
{
	return to_number();
}

json_var& json_var::operator[](size_t index)
{
	JSON_ASSERT(is_array() || is_object(), "json_var : not an array nor an object");
	if (is_array())
		return (*value.array)[index];
	else
		return (*value.object)[index];
}

json_var& json_var::operator[](const std::string& key)
{
	if (is_null())
		*this = json_object();
	JSON_ASSERT(is_object(), "json_var : not an object");
	return (*value.object)[key];
}

//////////////////////////////////////////////////////////////////////////
// helper functions
//////////////////////////////////////////////////////////////////////////

void write_object(std::ostream& stream, json_object& obj, int indent = 1)
{
	if (obj.count() <= 0)
	{
		//stream << "{}";
		return;
	}
	size_t i;
	for (i = 0; i < obj.count() - 1; i++)
	{
		for (int j = 0; j < indent; j++)
			stream << "\t";
		if (obj[i].is_string())
			stream << "\"" << obj.get_key(i) << "\" : \"" << obj[i] << "\",\n";
		else if (!obj[i].is_object())
			stream << "\"" << obj.get_key(i) << "\" : " << obj[i] << ",\n";
		else
		{
			stream << "\"" << obj.get_key(i) << "\" : \n";
			for (int j = 0; j < indent; j++)
				stream << "\t";
			stream << "{\n";
			write_object(stream, obj[i], indent + 1);
			stream << "\n";
			for (int j = 0; j < indent; j++)
				stream << "\t";
			stream << "},\n";
		}
	}

	for (int j = 0; j < indent; j++)
		stream << "\t";

	if (obj[i].is_string())
		stream << "\"" << obj.get_key(i) << "\" : \"" << obj[i] << "\"\n";
	else if (!obj[i].is_object())
		stream << "\"" << obj.get_key(i) << "\" : " << obj[i];
	else
	{
		stream << "\"" << obj.get_key(i) << "\" : \n";
		for (int j = 0; j < indent; j++)
			stream << "\t";
		stream << "{\n";
		write_object(stream, obj[i], indent + 1);
		stream << "\n";
		for (int j = 0; j < indent; j++)
			stream << "\t";
		stream << "}";
	}
}

//////////////////////////////////////////////////////////////////////////
//	operators
//////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& stream, json_string& str)
{
	stream << str.get();
	return stream;
}

std::ostream& operator<<(std::ostream& stream, json_array& arr)
{
	if (arr.count() == 0)
	{
		stream << "[]";
		return stream;
	}

	stream << "[ ";
	for (size_t i = 0; i < arr.count() - 1; i++)
	{
		if (arr[i].is_string())
			stream << "\"" << arr[i] << "\", ";
		else
			stream << arr[i] << ", ";
	}
	if (arr[arr.count() - 1].is_string())
		stream << "\"" << arr[arr.count() - 1] << "\" ]";
	else
		stream << arr[arr.count() - 1] << " ]";
	return stream;
}

std::ostream& operator<<(std::ostream& stream, json_object& obj)
{
	stream << "{\n";
	write_object(stream, obj);
	stream << "\n}";
	return stream;
}

std::ostream& operator<<(std::ostream& stream, json_var& var)
{
	if (var.type == json_type::array)
		stream << *var.value.array;
	else if (var.type == json_type::string)
		stream << *var.value.string;
	else if (var.type == json_type::number)
		stream << var.value.number;
	else if (var.type == json_type::boolean)
		stream << (var.value.boolean ? "true" : "false");
	else if (var.type == json_type::null)
		stream << "null";
	else if (var.type == json_type::object)
		stream << *var.value.object;
	return stream;
}