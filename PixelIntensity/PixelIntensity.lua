-- sample_premake.lua
solution "PixelIntensity"
    configurations { "Debug", "Release", "Machine" }
 
    objdir "${HOME}/src/cpp/ImageProcessing/obj/%{cfg.buildcfg}"
    targetdir "${HOME}/src/cpp/ImageProcessing/bin/%{cfg.buildcfg}"

    configuration "Debug"
        defines { "_DEBUG" }
        flags { "Symbols" }
        targetsuffix "_g"
 
    configuration "Release"
        defines { "_REL" }
        flags { "Optimize" }

    configuration "gmake"
        buildoptions { "-Wall" }
        buildoptions { "-fPIC" }
        buildoptions { "-pthread" }
        buildoptions { "-std=c++14" }

    includedirs { "/usr/include", 
                 "/usr/local/include/opencv", 
                 "/usr/local/include/opencv2" }
    linkoptions { "-L/usr/local/opt/opencv3/lib/" }
    links { "opencv_core", 
           "opencv_imgproc", 
           "opencv_imgcodecs", 
           "opencv_highgui" }

    project "PixelIntensityBW"
        kind "ConsoleApp"
        language "C++"
        files { "PixelIntensityBW.C" }

    project "PixelIntensityColor"
        kind "ConsoleApp"
        language "C++"
        files { "PixelIntensityColor.C" }
