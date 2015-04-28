project(habanero3d)
cmake_minimum_required(VERSION 2.8)

#subdirs(src)
#subdirs(demo)

add_definitions(
	-std=c++11
)

add_subdirectory(src)
#add_subdirectory(demo)

find_package(PkgConfig)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

find_package(OpenGL REQUIRED)
find_package(ASSIMP REQUIRED)
find_package(SFML 2 COMPONENTS REQUIRED system window graphics network audio)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror") #warnings as errors

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
