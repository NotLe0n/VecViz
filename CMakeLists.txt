cmake_minimum_required(VERSION 3.24)
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

# -- ImGui --
set(IMGUI_VERSION 1.89.2)
find_package(imgui ${IMGUI_VERSION} QUIET) # QUIET or REQUIRED
if (NOT imgui_FOUND) # If there's none, fetch and build raylib
    FetchContent_Declare(
            imgui
            URL https://github.com/ocornut/imgui/archive/refs/tags/v${IMGUI_VERSION}.tar.gz
    )
    FetchContent_MakeAvailable(imgui)
endif()

# -- rlImGui --
FetchContent_Declare(
        rlImGui
        URL https://github.com/raylib-extras/rlImGui
)
FetchContent_MakeAvailable(rlImGui)

# Our Project
add_executable(${PROJECT_NAME} main.cpp UI/UIWindow.cpp UI/UIWindow.h FontManager.cpp FontManager.h utils.h utils.cpp VectorSpaces/VectorSpace.h VectorSpaces/VectorSpace2D.cpp VectorSpaces/VectorSpace2D.h VectorSpaces/DrawVector.cpp VectorSpaces/DrawVector.h Drawing.h VectorSpaces/VectorSpace.cpp ${rlImGui_SOURCE_DIR}/rlImGui.cpp ${rlImGui_SOURCE_DIR}/rlImGui.h)
#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib)
target_include_directories(${PROJECT_NAME} ${imgui_SOURCE_DIR} ${rlImGui_SOURCE_DIR})

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