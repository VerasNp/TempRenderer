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

option(temprenderer_BUILD_TESTS "Compile with tests" ON)

if (NOT API_GRAPHIC)
  set(API_GRAPHIC "opengl" CACHE STRING "Choose the Graphic API" FORCE)
endif ()

include("cmake/APIGraphicsDetection.cmake")
set_api_graphics_detection_vars()

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Escolha o tipo de build: Debug ou Release" FORCE)
endif ()

include("cmake/SetupExternalLibs.cmake")
setup_external_libs()

set(temprenderer_core_source_dir "${PROJECT_SOURCE_DIR}/src/")
set(temprenderer_core_source_files
        "${temprenderer_core_source_dir}/temprenderer/core/cli/CliOptions.cpp"
        "${temprenderer_core_source_dir}/temprenderer/core/logging/LoggerManager.cpp"
        "${temprenderer_core_source_dir}/temprenderer/core/config/ApplicationConfig.cpp"
        "${temprenderer_core_source_dir}/temprenderer/core/config/CameraConfig.cpp"
        "${temprenderer_core_source_dir}/temprenderer/core/config/RenderConfig.cpp"
        "${temprenderer_core_source_dir}/temprenderer/core/config/SceneConfig.cpp"
        "${temprenderer_core_source_dir}/temprenderer/core/config/WindowConfig.cpp"
        "${temprenderer_core_source_dir}/temprenderer/pd/editor/EditorManager.cpp"
        "${temprenderer_core_source_dir}/temprenderer/platform/graphics/WindowManager.cpp"
        "${temprenderer_core_source_dir}/temprenderer/renderer/Camera.cpp"
        "${temprenderer_core_source_dir}/temprenderer/renderer/Canvas.cpp"
        "${temprenderer_core_source_dir}/temprenderer/renderer/PhongIntegrator.cpp"
        "${temprenderer_core_source_dir}/temprenderer/renderer/RayCastIntegrator.cpp"
        "${temprenderer_core_source_dir}/temprenderer/renderer/RendererManager.cpp"
        "${temprenderer_core_source_dir}/temprenderer/renderer/Sphere.cpp"
        "${temprenderer_core_source_dir}/temprenderer/renderer/Cone.cpp"
        "${temprenderer_core_source_dir}/temprenderer/renderer/Plane.cpp"
        "${temprenderer_core_source_dir}/temprenderer/scene/SceneComposer.cpp"
        "${temprenderer_core_source_dir}/temprenderer/ApplicationManager.cpp"
)

add_library(temprenderer_core STATIC ${temprenderer_core_source_files})

set(temprenderer_core_include_dir "${PROJECT_SOURCE_DIR}/include/temprenderer")

target_include_directories(temprenderer_core
        PUBLIC
        ${temprenderer_core_include_dir}
)

target_link_libraries(temprenderer_core
        PUBLIC
        kwp::kwp
        tomlplusplus::tomlplusplus
        imgui
        glad
        glfw
        nlohmann.json
)

add_executable(${PROJECT_NAME} "src/temprenderer/main.cpp")

target_link_libraries(${PROJECT_NAME} PRIVATE temprenderer_core)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/config.toml
        $<TARGET_FILE_DIR:${PROJECT_NAME}>

        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/proj1.json
        $<TARGET_FILE_DIR:${PROJECT_NAME}>

        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
)


if (temprenderer_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif ()
