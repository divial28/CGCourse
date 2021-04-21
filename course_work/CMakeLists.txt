cmake_minimum_required(VERSION 3.0.0)

project(course_work)

set(CMAKE_CXX_STANDARD 11)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(ASSIMP REQUIRED)

file(GLOB SOURCES *.h *.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(course_work sfml-graphics sfml-audio SOIL assimp ${OPENGL_LIBRARY} ${GLEW_LIBRARY})