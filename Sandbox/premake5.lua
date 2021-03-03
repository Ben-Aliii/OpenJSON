project "Sandbox"
	kind		    "ConsoleApp"
	language	    "C++"
    systemversion 	"latest"

	targetdir	("../bin/%{cfg.system}/%{cfg.architecture}/%{cfg.buildcfg}/%{prj.name}")
	objdir		("../bin/intermediate/%{cfg.system}/%{cfg.architecture}/%{cfg.buildcfg}/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"../OpenJSON/include"
	}

	defines
	{
		"JSON_ENABLE_ASSERT"
	}

	links
	{
		"OpenJSON"
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