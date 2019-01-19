cmake_minimum_required(VERSION 3.5)
project(MiniFender)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core HINTS ~/Qt/5.11.1/gcc_64/)
find_package(Qt5Sql HINTS ~/Qt/5.11.1/gcc_64)

add_executable(MiniFender main.cpp core.cpp enginehandler.cpp readconfiguration.cpp engine.cpp workerthread.cpp)

# linking
target_link_libraries(MiniFender Qt5::Core Qt5::Sql)
