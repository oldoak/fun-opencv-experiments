solution "WSImageServerTest"
    configurations { "Debug", "Release", "Machine", "Test" }
 
    objdir "${HOME}/build/WSImageServer/obj/%{cfg.buildcfg}"
    targetdir "${HOME}/build/WSImageServer/bin/%{cfg.buildcfg}"

    configuration "Debug"
        defines      { "_DEBUG" }
        flags        { symbols("On") }
        targetsuffix "_g"
        buildoptions { "-g -O0" }
 
    configuration "Test"
        defines      { "_TEST" }
        flags        { "Symbols" }
 
    configuration "Release"
        defines      { "_REL" }
        flags        { "Optimize" }

    configuration "gmake"
        buildoptions { "-Wall" }
        buildoptions { "-fPIC" }
        buildoptions { "-pthread" }
        buildoptions { "-std=c++14" }

        linkoptions  { "${GTEST_DIR}/lib/libgtest.a",
                       "${GMOCK_DIR}/lib/libgmock.a" }

    includedirs      { "/usr/include",
                       "${GTEST_DIR}/include",
                       "${GMOCK_DIR}/include",
                       "${PWD}/../include" }
    libdirs          { "/usr/lib", 
                       "/usr/local/lib",
                       "${GTEST_ROOT}/googletest/lib" }
    links            { "PocoFoundation",
                       "PocoNet",
                       "PocoUtil" }

    project "WSImageServerTest"
        kind "ConsoleApp"
        language "C++"
        files { "../test/*.c" }
