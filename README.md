# OpenJSON

A JSON library written in C++. It is fast and lightweight. It has no dependencies other than STL.

## Build Notes

Build scripts are provided for [premake](https://premake.github.io).

## Integration

Add include path and link against the static library and include the header file in your code.
```cpp
#include <openjson.h>
```

## Usage

The API is simple and easy to use. To make this JSON object

```json
{
    "language" : "C++",
    "pi" : 3.14159,
    "colors" : [ "Red", "Green", "Blue" ],
    "nothing" : null,
    "enabled" : true,
    "vector3" : 
    {
        "x" : 0.0,
        "y" : 1.0,
        "z" : 3.0
    }
}
```

you can write

```cpp
// make an empty json variable (null)
json_var var;

// add a new key-value pair (var will implicitly converted to an object)
var["language"] = "C++";

// add a number
var["pi"] = 3.14159f;

// add an array of elements
var["colours"] = { "Red", "Green", "Blue" }

// add a null value
var["nothing"] = json_var();

// add a boolean
var["enabled"] = true;

// add an object
var["vector3"]["x"] = 0.0f;
var["vector3"]["y"] = 1.0f;
var["vector3"]["z"] = 3.0f;
```

To save your object to a file you can write:
```cpp
// this methods takes the object to save and file path
json_doc::save(var, "var.json");
```
It will be saved with proper indetation.

You can load JSON from a file in the disk or a string.
To load from file you can use:
```cpp
// this methods takes a variable to save to and file path
json_doc::load_file(var, "var.json");
```
Or you can make an object directly form a string:
```cpp
// load an object form string
json_doc::load(var, "{ \"key\":\"value\" }");

// or you can use this istead
var = "{ \"key\":\"value\" }"_json;
```
Before loading your object form file or string you can change parsing mode. This librray provides 2 modes, strict and permissive. Setting it to strict mode makes it accept properly formated objects only. The permissive mode makes it accept properly formated objects and C style single line and multi-line comments and single quoted string for keys or values.
You can change the mode like so:

```cpp
// set it to strict parsing
json_doc::mode = parse_mode::strict;
// set it to permissive parsing
json_doc::mode = parse_mode::permissive;
```

*NOTE:* you must change the parsing mode before you actually load the object either from a file or a string.

The 'json_var' is flexible. It can be an object or a value.
```cpp
json_var var;
var = "This is a test string";
var = 123;
var = true;
var = { "car", 3.14f, nullptr };
```