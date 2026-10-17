include(FetchContent)

FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://github.com/doctest/doctest.git
        GIT_TAG v2.4.11
        SYSTEM
)

FetchContent_MakeAvailable(doctest)

add_executable(
        temprenderer_tests
        test_main.cpp
        "${CMAKE_CURRENT_SOURCE_DIR}/core/config/utils_test.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/math/image_texture_test.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/math/metal_texture_test.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/math/wood_texture_test.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/renderer/material_render_test.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/renderer/texture_coordinates_test.cpp"
#        "${CMAKE_CURRENT_SOURCE_DIR}/core/config/render_config_test.cpp"
#        "${CMAKE_CURRENT_SOURCE_DIR}/renderer/camera_test.cpp"
)

target_link_libraries(
        temprenderer_tests
        PRIVATE
        doctest::doctest
        kwp::kwp
        temprenderer_core
)

list(APPEND CMAKE_MODULE_PATH ${doctest_SOURCE_DIR}/scripts/cmake)

include(CTest)
include(doctest)

doctest_discover_tests(temprenderer_tests)
