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

#ifndef CORE_H_INCLUDED
#define CORE_H_INCLUDED

//includes
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <initializer_list>
#include <cstdlib>


//platform
#if defined(_WIN64) || defined(_WIN32)
	#define JSON_PLATFORM_WIN
#elif defined(__linux__) && !defined(__ANDROID__)
	#define JSON_PLATFORM_LINUX
#else
	#define JSON_PLATFORM_UNKNOWN
#endif

//function
#if defined(JSON_PLATFORM_WIN)
	#define __FUNCTION_NAME__   __FUNCTION__
#elif defined(JSON_PLATFORM_LINUX)
	#define __FUNCTION_NAME__   __func__
#else
	#define __FUNCTION_NAME__
#endif

//assert
#if defined(JSON_ENABLE_ASSERT)
	#define JSON_ASSERT(x, y) if (!(x)) { std::cout << #x << "\nassertion failed : " << y << "\nfile : " << __FILE__  << "\nfunction : " << __FUNCTION_NAME__ << "\nline : " << __LINE__; abort(); }
#else 
	#define JSON_ASSERT(x, y)
#endif

//version
#define JSON_VERSION_MAJOR		0
#define JSON_VERSION_MINOR		1
#define JSON_VERSION_REVISION	0
#define JSON_VERSION_STATUS		"alpha"
#define JSON_VERSION_ID			(JSON_VERSION_MAJOR * 100000 + JSON_VERSION_MINOR * 100 + JSON_VERSION_REVISION)

#endif //CORE_H_INCLUDED