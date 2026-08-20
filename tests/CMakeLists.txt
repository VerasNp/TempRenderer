include(FetchContent)

FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://github.com/doctest/doctest.git
        GIT_TAG v2.4.11
        SYSTEM
)

FetchContent_MakeAvailable(doctest)

add_executable(
        TempRender_tests
        test_main.cpp
        factorial_test.cpp
)

target_link_libraries(TempRender_tests PRIVATE doctest::doctest)

list(APPEND CMAKE_MODULE_PATH ${doctest_SOURCE_DIR}/scripts/cmake)

include(CTest)
include(doctest)

doctest_discover_tests(TempRender_tests)
