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

include("cmake/APIGraphicsDetection.cmake")
set_api_graphics_detection_vars()

set(source_dir "${PROJECT_SOURCE_DIR}/src/")
file(
        GLOB
        source_files
        "${source_dir}/temprenderer/main.cpp"
        "${source_dir}/temprenderer/core/cli/CliOptions.cpp"
        "${source_dir}/temprenderer/core/logging/LoggerManager.cpp"
        "${source_dir}/temprenderer/core/config/ApplicationConfig.cpp"
        "${source_dir}/temprenderer/core/config/CameraConfig.cpp"
        "${source_dir}/temprenderer/core/config/RenderConfig.cpp"
        "${source_dir}/temprenderer/core/config/SceneConfig.cpp"
        "${source_dir}/temprenderer/core/config/WindowConfig.cpp"
        "${source_dir}/temprenderer/pd/editor/EditorManager.cpp"
        "${source_dir}/temprenderer/platform/graphics/WindowManager.cpp"
        "${source_dir}/temprenderer/renderer/Camera.cpp"
        "${source_dir}/temprenderer/renderer/Canvas.cpp"
        "${source_dir}/temprenderer/renderer/PhongIntegrator.cpp"
        "${source_dir}/temprenderer/renderer/RayCastIntegrator.cpp"
        "${source_dir}/temprenderer/renderer/RendererManager.cpp"
        "${source_dir}/temprenderer/renderer/Sphere.cpp"
        "${source_dir}/temprenderer/scene/SceneComposer.cpp"
        "${source_dir}/temprenderer/ApplicationManager.cpp"
)

set(include_dir "${PROJECT_SOURCE_DIR}/include/temprenderer")

option(temprenderer_BUILD_TESTS "Compile with tests" ON)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Escolha o tipo de build: Debug ou Release" FORCE)
endif ()

include("cmake/SetupExternalLibs.cmake")
setup_external_libs()

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
        ${CMAKE_SOURCE_DIR}/config.toml
        $<TARGET_FILE_DIR:${PROJECT_NAME}>

        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
)


if (temprenderer_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif ()
