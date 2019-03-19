#Change this if you need to target a specific CMake version
cmake_minimum_required(VERSION 2.6)

project(riflemanager LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)

# Define sources and executable
set(EXECUTABLE_NAME "riflemanager")
add_executable(${EXECUTABLE_NAME} ./src/AboutState.hpp ./src/AboutState.cpp ./src/AdminState.hpp ./src/AdminState.cpp ./src/AppEngine.hpp ./src/AppEngine.cpp ./src/AppState.hpp ./src/DebugState.hpp ./src/DebugState.cpp ./src/Logger.hpp ./src/Logger.cpp ./src/main.cpp ./src/MainMenuState.hpp ./src/MainMenuState.cpp ./src/Rifle.hpp ./src/Rifle.cpp ./src/RifleCheckoutState.hpp ./src/RifleCheckoutState.cpp ./src/RifleFileSystem.hpp ./src/RifleFileSystem.cpp ./src/RifleListState.hpp ./src/RifleListState.cpp ./src/RifleManager.hpp ./src/RifleManager.cpp ./src/SerialInputBox.hpp ./src/SerialInputBox.cpp ./src/SettingsParser.hpp ./src/SettingsParser.cpp ./src/SettingsState.hpp ./src/SettingsState.cpp ./src/Password.hpp ./src/Password.cpp ./src/PicoSHA256.hpp)

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED network audio graphics window system)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()

include_directories(${CMAKE_SOURCE_DIR}/SFUI/)
link_directories(${CMAKE_SOURCE_DIR}/SFUI/)
target_link_libraries(riflemanager ${CMAKE_SOURCE_DIR}/SFUI/SFUI.a)
target_link_libraries(riflemanager stdc++fs)

# I don't know how to use CMake.
# If you want it in debug, create the config using -DCMAKE_BUILD_TYPE=Release
# I don't know why it's reversed, but it is, so fuck me I guess.
message("This is as close as I can get it:")
message("If you want to compile in Debug, set CMAKE_BUILD_TYPE to Release.")
message("If you want to compile in Release, set CMAKE_BUILD_TYPE to Debug.")

# This only shows up in release, for whatever reason, so there's no real reason to use it. It might be useful later, so I'll keep it here.
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DIS_DEBUG_ENABLED")
