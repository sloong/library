cmake_minimum_required(VERSION 3.2)

project(univ)

include_directories(
    "$(PROJECT_BINARY_DIR)"
    "/usr/include/glib-2.0"
    "/usr/include/lua5.3"
	# ubuntu path
    "/usr/lib/x86_64-linux-gnu/glib-2.0/include"
	# centos path
	"/usr/lib64/glib-2.0/include"
)

#允许gdb调试
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -g -pg -ggdb -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -DNODEBUG -DRELASE")

add_definitions(-DLUA_USE_READLINE)

# 代码覆盖信息
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftest-coverage -fprofile-arcs")

# 废弃API检测
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")

# 警告信息选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-switch -Wno-return-type -Wno-misleading-indentation")

# C++ 版本
set(CMAKE_CXX_FLAGS "-std=c++17 ${CMAKE_CXX_FLAGS}")

#当前目录下所有源文件赋给SRC_LIST
#AUX_SOURCE_DIRECTORY(. SRC_LIST) 
file(GLOB_RECURSE SRC_LIST "*.c*")

#生成可执行文件
add_library(${PROJECT_NAME} SHARED ${SRC_LIST}) 

target_link_libraries(${PROJECT_NAME} crypto)
target_link_libraries(${PROJECT_NAME} lua5.3)
target_link_libraries(${PROJECT_NAME} boost_serialization)