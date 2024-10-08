cmake_minimum_required(VERSION 3.12.0)
set(CMAKE_CXX_STANDARD 20)

project(ProjectName VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# globbing up sources
# - scaffold & non-cmake vendor sources
file(GLOB scaffold_sources CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/scaffold/source/*.cpp
    ${CMAKE_SOURCE_DIR}/scaffold/builtin/*.cpp
    ${CMAKE_SOURCE_DIR}/vendor/imgui/*.cpp
    ${CMAKE_SOURCE_DIR}/vendor/glad/src/gl.c
    ${CMAKE_SOURCE_DIR}/vendor/tinygltf/tinygltf_impl.cpp
)

# - project sources
#file(GLOB project_sources CONFIGURE_DEPENDS
#${CMAKE_SOURCE_DIR}/path/to/sources/*.cpp
#)

# register sources
add_executable(${CMAKE_PROJECT_NAME} 
    main.cpp 
    ${scaffold_sources}
#    ${project_sources}
)

# exposing includes
# - scaffold & vendor includes
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/scaffold/include
    ${CMAKE_SOURCE_DIR}/vendor/glfw-3.3.8/include/GLFW
    ${CMAKE_SOURCE_DIR}/vendor/glad/include
    ${CMAKE_SOURCE_DIR}/vendor/imgui
    ${CMAKE_SOURCE_DIR}/vendor/glm
)

# - project includes
#target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
#    ${CMAKE_SOURCE_DIR}path/to/includes
#)

# vendor specific
# - glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/glfw-3.3.8)
target_link_libraries(${CMAKE_PROJECT_NAME} glfw)
