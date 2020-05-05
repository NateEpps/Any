cmake_minimum_required(VERSION 3.0)

project(Any)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE ${CMAKE_SOURCE_DIR}/main.cpp
	${CMAKE_SOURCE_DIR}/Any.hpp
	${CMAKE_SOURCE_DIR}/Any.cpp)

add_executable(Main ${SOURCE})