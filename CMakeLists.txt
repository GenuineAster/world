cmake_minimum_required(VERSION 2.8)

project(world)

# Set options
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the build type ( Debug / Release )" FORCE)
endif()

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})

# Add a place to look for CMake modules in
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules;${CMAKE_MODULE_PATH}")

# Add source files
set(SRC_DIR "src")
file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false ${SRC_DIR}/*)


include_directories(ext/stb)

include_directories(${SRC_DIR})

find_package(GLFW)
include_directories(${GLFW_INCLUDE_DIRS})
link_libraries(${GLFW_LIBRARIES})

find_package(GLEW)
include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES})

find_package(OpenGL)
include_directories(${OPENGL_INCLUDE_DIRS})
link_libraries(${OPENGL_LIBRARIES})

find_package(Assimp)
include_directories(${ASSIMP_INCLUDE_DIRS})
link_libraries(${ASSIMP_LIBRARIES})

find_package(JsonCPP)
include_directories(${JSONCPP_INCLUDE_DIR})
link_libraries(${JSONCPP_LIBRARY})

include_directories(ext/imgui)
include_directories(ext/imgui-glfw)

add_library(imgui ext/imgui/imgui.h
                  ext/imgui/imgui.cpp
                  ext/imgui/imgui_draw.cpp
                  ext/imgui-glfw/imgui_impl_glfw_gl3.cpp
)
link_libraries(imgui)


# Enable C++14
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -pedantic -mfpmath=sse -msse3 -pthread")
endif()

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})



install(TARGETS ${CMAKE_PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_BINARY_DIR}/bin/)
install(DIRECTORY res DESTINATION ${CMAKE_BINARY_DIR})

# for whenever we do docs?
#find_package(Doxygen)
#if(DOXYGEN_FOUND)
#    configure_file(${CMAKE_SOURCE_DIR}/docs/Doxyfile.in ${CMAKE_BINARY_DIR}/docs/Doxyfile @ONLY)
#    add_custom_command(OUTPUT documentation COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/docs/Doxyfile BYPRODUCTS ${CMAKE_BINARY_DIR}/docs/html ${CMAKE_BINARY_DIR}/docs/Doxyfile COMMENT "Generating docs using Doxygen" VERBATIM)
#    add_custom_target(docs ALL DEPENDS documentation VERBATIM)
#    set_directory_properties(PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_BINARY_DIR}/docs)
#endif()
