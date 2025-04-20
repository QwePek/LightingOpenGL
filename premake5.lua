workspace "LightingOpenGL"
   architecture "x86"
   
   configurations { "Debug", "Release" }
   startproject "LightingOpenGL"

project "LightingOpenGL"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   
   targetdir "bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
   objdir "bin-int/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
   
   pchheader "pch.h"
   pchsource "src/pch.cpp"

   files { 
       "src/**.h", 
       "src/**.cpp",
       "res/libraries/**.h",
       "res/libraries/glm/detail/glm.cpp",
       "res/libraries/imgui/imgui.cpp",
       "res/libraries/imgui/imgui_demo.cpp",
       "res/libraries/imgui/imgui_draw.cpp",
       "res/libraries/imgui/imgui_impl_glfw.cpp",
       "res/libraries/imgui/imgui_impl_opengl3.cpp",
       "res/libraries/imgui/imgui_tables.cpp",
       "res/libraries/imgui/imgui_widgets.cpp",
       "res/libraries/stb_image/*.h",
       "res/libraries/stb_image/*.cpp",
   }

    includedirs {
        "res/libraries", 
        "Dependencies/GLFW/include",
        "Dependencies/GLEW/include",
        "Dependencies/assimp/include"
    }

   libdirs { 
       "Dependencies/GLFW/lib-vc2022",
       "Dependencies/GLEW/lib/Release/Win32" 
   }
   
   links { "glfw3", "glew32s" }
   defines { "GLEW_STATIC" }

   filter "system:windows"
       defines { "WIN32", "_CONSOLE" }
       links { "opengl32", "User32", "Gdi32", "Shell32" }

   filter "configurations:Debug"
        defines { "_DEBUG" }
        symbols "On"
        postbuildcommands {
            '{COPY} "Dependencies/assimp/lib/Debug/Win32/assimp-vc143-mtd.dll" "%{cfg.targetdir}"' 
        }

   filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
        postbuildcommands {
            '{COPY} "Dependencies/assimp/lib/Release/Win32/assimp-vc143-mt.dll" "%{cfg.targetdir}"' 
        }

   filter { "configurations:Debug", "architecture:x86" }
       libdirs { "Dependencies/assimp/lib/Debug/Win32" }
       links { "assimp-vc143-mtd" }

   filter { "configurations:Release", "architecture:x86" }
       libdirs { "Dependencies/assimp/lib/Release/Win32" }
       links { "assimp-vc143-mt" }

   filter {}