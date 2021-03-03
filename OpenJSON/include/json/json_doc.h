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

#ifndef JSON_DOC_H_INCLUDED
#define JSON_DOC_H_INCLUDED

#include "json_vars.h"

enum class parse_mode
{
	strict,
	permissive
};

class json_doc
{
private:
	json_doc() {}
	json_doc(const json_doc&) = delete;
	~json_doc() {}

public:
	static parse_mode mode;

	static void save(json_object& obj, const char *file);
	static void save(json_object& obj, const std::string& file);
	static bool load_file(const char *file, json_object& obj);
	static bool load_file(const std::string& file, json_object& obj);
	static bool load(const char *str, json_object& obj);
	static bool load(const std::string& str, json_object& obj);

	static bool load_file(const char *file, json_var& var);
	static bool load_file(const std::string& file, json_var& var);
	static bool load(const char *str, json_var& var);
	static bool load(const std::string& str, json_var& var);
};

json_object operator""_json(const char *str, size_t size);

#endif //JSON_DOC_H_INCLUDED