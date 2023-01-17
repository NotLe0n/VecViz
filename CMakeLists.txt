cmake_minimum_required(VERSION 3.11)
project(VecViz)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

# Dependencies
include(FetchContent)
set(RAYLIB_VERSION 4.2.0)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    FetchContent_Declare(
            raylib
            URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
endif()

# Our Project
add_executable(${PROJECT_NAME} main.cpp UI/UIWindow.cpp UI/UIWindow.h FontManager.cpp FontManager.h utils.h utils.cpp VectorSpaces/VectorSpace.h VectorSpaces/VectorSpace2D.cpp VectorSpaces/VectorSpace2D.h VectorSpaces/DrawVector.cpp VectorSpaces/DrawVector.h Drawing.h lib/rlImGui/rlImGui.cpp lib/rlImGui/rlImGui.h lib/imgui/imgui.h lib/imgui/imgui.cpp lib/imgui/imgui_internal.h lib/imgui/imgui_draw.cpp lib/imgui/imgui_tables.cpp lib/imgui/imgui_demo.cpp lib/imgui/imgui_widgets.cpp VectorSpaces/VectorSpace.cpp)
#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib)
include_directories(lib/imgui lib/rlImGui)


file(COPY ${CMAKE_SOURCE_DIR}/Fonts DESTINATION ${CMAKE_BINARY_DIR})

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    # Tell Emscripten to build an example.html file.
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()