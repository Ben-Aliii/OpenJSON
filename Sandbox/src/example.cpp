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

#include <iostream>
#include <openjson.h>

int main()
{
	// make an empty json variable (null)
	json_var var;

	// add a new key-value pair (var will implicitly converted to an object)
	var["language"] = "C++";

	// add a number
	var["pi"] = 3.14159f;

	// add an array of elements
	var["colours"] = { "Red", "Green", "Blue" };

	// add another element to existing array
	var["colours"].to_array().add("Purple");

	// add a null value
	var["nothing"] = nullptr;

	// add a boolean
	var["enabled"] = true;

	// add an object
	var["vector3"]["x"] = 0.0f;
	var["vector3"]["y"] = 1.0f;
	var["vector3"]["z"] = 3.0f;

	std::cin.get();
	return 0;
}