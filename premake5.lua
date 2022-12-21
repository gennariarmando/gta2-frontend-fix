workspace "gta2-frontend-fix"
	configurations { "ReleaseII", "DebugII" }
	location "project_files"
   
project "gta2-frontend-fix"
	kind "SharedLib"
	language "C++"
	targetdir "output/asi/"
	objdir ("output/obj")
	targetextension ".asi"
	characterset ("MBCS")
	linkoptions "/SAFESEH:NO"
	buildoptions { "-std:c++latest", "/permissive" }
	defines { "_CRT_SECURE_NO_WARNINGS", "_CRT_NON_CONFORMING_SWPRINTFS", "_USE_MATH_DEFINES" }
	disablewarnings { "4244", "4800", "4305", "4073", "4838", "4996", "4221", "4430", "26812", "26495", "6031" }

	files {
		"source/**.*",
		"vendor/pugixml/src/*.*"
	}
	
	includedirs { 
		"source/**",
		"vendor/**",
        "vendor/pugixml/src",
	}
	
	includedirs {
		"$(PLUGIN_SDK_DIR)/shared/",
		"$(PLUGIN_SDK_DIR)/shared/game/",
	}
		
	filter { "configurations:*II" }	
		defines { "GTA2", "PLUGIN_SGV_96EN" }	
		includedirs {
			"$(PLUGIN_SDK_DIR)/plugin_II/",
			"$(PLUGIN_SDK_DIR)/plugin_II/game_II/",
		}
		targetname "GTA2FrontendFix"
		debugdir "$(GTA_II_DIR)"
		debugcommand "$(GTA_II_DIR)/gta2.exe"
		postbuildcommands "copy /y \"$(TargetPath)\" \"$(GTA_II_DIR)\\scripts\\GTA2FrontendFix.asi\""

	filter { }
	
	libdirs { 
		"$(PLUGIN_SDK_DIR)/output/lib/",
	}
	
	filter "configurations:Debug*"		
		defines { "DEBUG" }
		symbols "on"
		staticruntime "on"

	filter "configurations:Release*"
		defines { "NDEBUG" }
		symbols "off"
		optimize "On"
		staticruntime "on"
		
	filter "configurations:ReleaseII"
		links { "plugin_ii" }
		
	filter "configurations:DebugII"
		links { "plugin_ii_d" }
			
	filter { }
