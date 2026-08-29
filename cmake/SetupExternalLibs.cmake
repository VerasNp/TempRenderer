macro(setup_external_libs)
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

  add_library(cl11 INTERFACE)
  target_include_directories(cl11 INTERFACE include/cli11)
endmacro()
