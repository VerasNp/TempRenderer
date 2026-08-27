cmake_minimum_required(VERSION 3.20)

project(
        temprenderer
        VERSION 0.0.0
        DESCRIPTION "Render engine"
        LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

if (NOT API_GRAPHIC)
    set(API_GRAPHIC "opengl" CACHE STRING "Choose the Graphic API" FORCE)
endif ()

include(cmake/APIGraphicsDetection.cmake)
set_api_graphics_detection_vars()

set(source_dir "${PROJECT_SOURCE_DIR}/src/")
file(
        GLOB
        source_files
        "${source_dir}/temprenderer/main.cpp"
        "${source_dir}/temprenderer/core/logging/LoggerManager.cpp"
        "${source_dir}/temprenderer/core/config/ApplicationConfig.cpp"
        "${source_dir}/temprenderer/platform/graphics/WindowManager.cpp"
        "${source_dir}/temprenderer/renderer/RendererManager.cpp"
        "${source_dir}/temprenderer/renderer/Canvas.cpp"
        "${source_dir}/temprenderer/renderer/RayCastIntegrator.cpp"
        "${source_dir}/temprenderer/Application.cpp"
        "${source_dir}/temprenderer/pd/editor/EditorManager.cpp"
)

set(include_dir "${PROJECT_SOURCE_DIR}/include")

option(temprenderer_BUILD_TESTS "Compile with tests" ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Escolha o tipo de build: Debug ou Release" FORCE)
endif ()

include(FetchContent)

FetchContent_Declare(
        kwp
        GIT_REPOSITORY https://codeberg.org/VerasNp/KWP
        GIT_TAG main
        GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(kwp)

FetchContent_Declare(
        tomlplusplus
        GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
        GIT_TAG v3.4.0
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(tomlplusplus)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw
        GIT_TAG 3.5.1
        GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(glfw)

find_package(OpenGL REQUIRED)

FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_TAG v1.91.5
        GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(imgui)

FetchContent_GetProperties(imgui)

add_library(imgui STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)
target_link_libraries(imgui PUBLIC glfw OpenGL::GL)

add_library(glad STATIC include/glad/src/glad.c)
target_include_directories(glad PUBLIC include/glad/include)

add_executable(${PROJECT_NAME} ${source_files})

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${include_dir}
        ${source_dir}
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        kwp::kwp
        tomlplusplus::tomlplusplus
        glad
        glfw
        imgui
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/engine_config.toml
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)


if (temprenderer_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif ()
