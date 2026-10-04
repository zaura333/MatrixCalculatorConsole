include(FetchContent)
FetchContent_Declare(googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(matrix_tests "SmokeTests.cpp" "MatrixTests.cpp")
target_link_libraries(matrix_tests PRIVATE matrix GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(matrix_tests)