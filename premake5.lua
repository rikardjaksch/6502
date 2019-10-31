workspace "6502_emulation"
    location "build_gen"
    language "C"
    architecture "x86_64"
    configurations { "Debug", "Release" }
    flags { "FatalWarnings", "MultiProcessorCompile" }
    targetdir ("bin/")
    objdir ("obj/%{prj.name}/%{cfg.shortname}")
    startproject "6502_tests"

    filter { "configurations:Debug" }
        symbols "On"

    filter { "configurations:Release" }
        optimize "On"

    filter "system:windows"
        platforms { "Win64" }

    filter "platforms:Win64"
        defines { "_CRT_SECURE_NO_WARNINGS" }
        staticruntime "On"
        architecture "x64"

    project "6502"
        kind "StaticLib"
        files { "6502/include/*.h", "6502/source/*.c", "6502/source/*.h" }
        includedirs { "6502/include", "6502/source" }

    project "6502_tests"
        kind "ConsoleApp"
        files { "6502_tests/*.h", "6502_tests/*.c" }
        links { "6502" }
        includedirs "6502/include"