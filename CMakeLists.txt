cmake_minimum_required(VERSION 3.5)
project(MiniFender)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS ON)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC)
set(CMAKE_AUTORCC)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

configure_file(settings.ini settings/settings.ini COPYONLY)
configure_file(auth.ini settings/auth.ini COPYONLY)

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Sql REQUIRED)

add_subdirectory(TestEngines)
include_directories(TestEngines)

add_subdirectory(AuthServer)
include_directories(AuthServer)

add_executable(MiniFender main.cpp core.cpp enginehandler.cpp engine.cpp workerthread.cpp dbmanager.cpp clihandler.cpp authclient.cpp)

install(TARGETS MiniFender
		CONFIGURATIONS Release
		DESTINATION ${CMAKE_BINARY_DIR}/MiniFender)
install(FILES settings.ini auth.ini
		CONFIGURATIONS Release
		DESTINATION ${CMAKE_BINARY_DIR}/MiniFender/settings)

install(TARGETS MiniFender_auth_server
		CONFIGURATIONS Release
		DESTINATION ${CMAKE_BINARY_DIR}/MiniFender)
install(FILES AuthServer/auth_settings.ini
		CONFIGURATIONS Release
		DESTINATION ${CMAKE_BINARY_DIR}/MiniFender/settings)

install(TARGETS TestEngines
		CONFIGURATIONS Release
		DESTINATION ${CMAKE_BINARY_DIR}/MiniFender/engines)



# linking
target_link_libraries(MiniFender Qt5::Core Qt5::Sql Qt5::Network)
