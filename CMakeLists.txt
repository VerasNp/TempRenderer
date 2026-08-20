cmake_minimum_required(VERSION 3.20)

project(
        TempRender
        VERSION 0.0.0
        DESCRIPTION "Render engine"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

option(TempRender_BUILD_TESTS "Compile with tests" ON)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Escolha o tipo de build: Debug ou Release" FORCE)
endif ()

include(FetchContent)

FetchContent_Declare(
        kwp
        GIT_REPOSITORY https://codeberg.org/VerasNp/KWP
        GIT_TAG main
)

FetchContent_MakeAvailable(kwp)

add_executable(
        TempRender
        main.cpp
)

target_link_libraries(TempRender
        PRIVATE
        kwp::kwp
)

if (TempRender_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif ()
