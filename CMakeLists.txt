cmake_minimum_required(VERSION 3.27.9)
project(megamario LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG main
    GIT_SHALLOW TRUE)

if (WIN32)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()
FetchContent_MakeAvailable(googletest)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)
enable_testing()

add_subdirectory(src)
add_subdirectory(tests)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(megamario ${SOURCES})

target_include_directories(megamario PRIVATE ${SOURCE_DIR})
target_link_libraries(megamario PRIVATE sfml-graphics)
target_link_libraries(megamario PRIVATE sfml-audio)
target_compile_features(megamario PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET megamario
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:megamario>
        VERBATIM)
endif()

install(TARGETS megamario)

# file(COPY assets/ DESTINATION ${CMAKE_BINARY_DIR}/bin)
add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/bin
    COMMENT "Copy assets" VERBATIM
)
add_dependencies(megamario copy_assets)
