cmake_minimum_required(VERSION 3.22)
project(labyrinth VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_SOURCE_DIR}/include)
aux_source_directory(${CMAKE_SOURCE_DIR}/source LIBS) 
add_library(lib STATIC ${LIBS})
message(STATUS "include: ${CMAKE_SOURCE_DIR}/include")

set(GLM_INCLUDE_DIRS /opt/homebrew/Cellar/glm/0.9.9.8/include)
message(STATUS "GLM found: ${GLM_INCLUDE_DIRS}")

find_package(GLUT REQUIRED)
message(STATUS "GLUT found: ${GLUT_INCLUDE_DIRS}")

find_package(OpenGL REQUIRED)
message(STATUS "OpenGL found: ${OPENGL_LIBRARIES}")

include_directories(${GLUT_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/include)

add_executable(labyrinth main.cpp)

target_link_libraries(labyrinth ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLM_LIBRARIES} lib)

add_custom_target(CopyShaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
    "${CMAKE_SOURCE_DIR}/resource" "${CMAKE_BINARY_DIR}/resource"
    COMMENT "Copy resource directory to build tree" VERBATIM)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations -std=c++17 -stdlib=libc++ -framework OpenGL -framework GLUT -I${GLUT_INCLUDE_DIRS} -I${GLM_INCLUDE_DIRS} -I${OPENGL_INCLUDE_DIRS} -I${CMAKE_SOURCE_DIR}/include")