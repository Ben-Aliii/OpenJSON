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

#ifndef JSON_TYPES_H_INCLUDED
#define JSON_TYPES_H_INCLUDED

#include "core.h"

enum class json_type : uint8_t
{
	null,
	object,
	array,
	string,
	boolean,
	number
};

struct json_object;
struct json_array;
struct json_string;
typedef bool json_boolean;
typedef float json_number;

struct json_var;

union json_value
{
	json_object *object;
	json_array *array;
	json_string *string;
	json_number number;
	json_boolean boolean;
};

#endif //JSON_TYPES_H_INCLUDED