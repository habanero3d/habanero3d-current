project(habanero3d)
cmake_minimum_required(VERSION 2.8)

add_definitions(
	-std=c++11
)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(ASSIMP REQUIRED)
find_package(SFML 2 COMPONENTS REQUIRED system window graphics network audio)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror") #warnings as errors

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

include_directories(src)

file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)
add_executable(foo ${SRC_FILES} ${INC_FILES})