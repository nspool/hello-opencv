cmake_minimum_required(VERSION 3.9)
set(CMAKE_CXX_STANDARD 11)
project( Hello-OpenCV )
find_package( OpenCV REQUIRED )
file(GLOB SOURCES ${PROJECT_NAME}/*.cpp)
file(GLOB HEADERS ${PROJECT_NAME}/*.hpp)
add_executable(Hello-OpenCV ${SOURCES} ${HEADERS} ${RESOURCES}) 
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

