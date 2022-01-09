solution "HoughTransform"
    configurations { "Debug", "Release", "macosx" }
 
    objdir "${HOME}/build/ImageProcessing/obj/%{cfg.buildcfg}"
    targetdir "${HOME}/build/ImageProcessing/bin/%{cfg.buildcfg}"

    configuration "Debug"
        defines { "_DEBUG" }
        flags   { symbols("On") }
        targetsuffix "_g"
 
    configuration "Release"
        defines { "_REL" }
        flags   { "Optimize" }

    configuration "gmake"
        buildoptions { "-Wall" }
        buildoptions { "-fPIC" }
        buildoptions { "-pthread" }
        buildoptions { "-std=c++14" }

    includedirs { "/usr/include", 
                 "/usr/local/opt/opencv3/include",
                 "/usr/local/opt/qt5/include" }
    linkoptions { "-L/usr/local/opt/opencv3/lib" }
    links       { "opencv_core", 
                 "opencv_imgproc", 
                 "opencv_imgcodecs", 
                 "opencv_highgui" }

    project "CoinDetection"
        kind "ConsoleApp"
        language "C++"
        files { "CoinDetection.C" }
