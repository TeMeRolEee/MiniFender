cmake_minimum_required(VERSION 3.5)
project(MiniFender)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC)
set(CMAKE_AUTORCC)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

configure_file(settings.ini settings/settings.ini COPYONLY)

find_package(Qt5Core REQUIRED)
find_package(Qt5Sql REQUIRED)
find_package(Qt5Network REQUIRED)

add_executable(MiniFender main.cpp core.cpp enginehandler.cpp engine.cpp workerthread.cpp dbmanager.cpp clihandler.cpp webserver.cpp)

# linking
target_link_libraries(MiniFender Qt5::Core Qt5::Sql Qt5::Network)
