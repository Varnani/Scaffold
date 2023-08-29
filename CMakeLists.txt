cmake_minimum_required(VERSION 3.12.0)
set(CMAKE_CXX_STANDARD 20)

project(Scaffold VERSION 0.1.0 LANGUAGES C CXX)

# glob up all scaffold & non-cmake vendor sources
file(GLOB scaffold_sources 
    ${CMAKE_SOURCE_DIR}/scaffold/source/*.cpp
    ${CMAKE_SOURCE_DIR}/scaffold/builtin/*.cpp
    ${CMAKE_SOURCE_DIR}/vendor/imgui/*.cpp
    ${CMAKE_SOURCE_DIR}/vendor/glad/src/gl.c
)

add_executable(${CMAKE_PROJECT_NAME} 
    main.cpp 
    ${scaffold_sources})

# expose scaffold & vendor includes
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/scaffold/include
    ${CMAKE_SOURCE_DIR}/vendor/glfw-3.3.8/include/GLFW
    ${CMAKE_SOURCE_DIR}/vendor/glad/include
    ${CMAKE_SOURCE_DIR}/vendor/imgui
    ${CMAKE_SOURCE_DIR}/vendor/glm
)

# glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/glfw-3.3.8)

target_link_libraries(${CMAKE_PROJECT_NAME} glfw)