solution "WSImageServer"
    configurations { "Debug", "Release", "Machine" }

    objdir "${HOME}/build/WSImageServer/obj/%{cfg.buildcfg}"
    targetdir "${HOME}/build/WSImageServer/bin/%{cfg.buildcfg}"

    configuration "Debug"
        defines      { "_DEBUG" }
        flags        { symbols("On") }
        targetsuffix "_g"
        buildoptions { "-g -O0" }
 
    configuration "Release"
        defines      { "_REL" }
        flags        { "Optimize" }

    configuration "gmake"
        buildoptions { "-Wall" }
        buildoptions { "-fPIC" }
        buildoptions { "-pthread" }
        buildoptions { "-std=c++14" }

    includedirs      { "/usr/include",
                       "/usr/local/include",
                       "${PWD}/../include",
                       "/usr/local/opt/opencv3/include" }

    libdirs          { "/usr/lib", 
                       "/usr/local/lib",
                       "/usr/local/opt/opencv3/lib" }

    links            { "PocoFoundation",
                       "PocoNet",
                       "PocoUtil",
                       "opencv_core", 
                       "opencv_imgproc", 
                       "opencv_imgcodecs", 
                       "opencv_highgui" }

    project "WSImageServer"
        kind "ConsoleApp"
        language "C++"
        files { "../src/*.cpp" }
