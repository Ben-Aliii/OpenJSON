project "OpenJSON"
	kind		    "StaticLib"
	language	    "C++"
    systemversion 	"latest"

	targetdir	("../bin/%{cfg.system}/%{cfg.architecture}/%{cfg.buildcfg}/%{prj.name}")
	objdir		("../bin/intermediate/%{cfg.system}/%{cfg.architecture}/%{cfg.buildcfg}/%{prj.name}")

	files
	{
		"include/**.h",
		"src/**.cpp"
	}
	
	includedirs
	{
		"include"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"JSON_ENABLE_ASSERT"
	}

	filter "system:linux"
		buildoptions 
		{
			"-Wall"
		}

    filter "configurations:Debug"
		symbols "On"
        optimize "Off"		
	filter "configurations:Release"
        symbols "Off"
		optimize "On"