# CTest configuration
enable_testing()

add_executable(test_Vec2 test_Vec2.cpp ${SOURCE_DIR}/Vec2.cpp)
target_include_directories(test_Vec2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
target_link_libraries(test_Vec2 GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(test_Vec2)
# Add a test executable
# add_executable(tests test_file.cpp)

# Link the test executable with the library
# target_link_libraries(tests PRIVATE my_library)

# Add the test to CTest
# add_test(NAME MyTest COMMAND tests)