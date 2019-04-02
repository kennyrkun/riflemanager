cmake_minimum_required(VERSION 3.0)

project(riflemanager LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)

# Define sources and executable
set(PROJECT_NAME "riflemanager")
set(CMAKE_DEBUG_POSTFIX "-d")
add_executable(${PROJECT_NAME} ./src/AboutState.hpp ./src/AboutState.cpp ./src/AdminState.hpp ./src/AdminState.cpp ./src/AppEngine.hpp ./src/AppEngine.cpp ./src/AppState.hpp ./src/DebugState.hpp ./src/DebugState.cpp ./src/Logger.hpp ./src/Logger.cpp ./src/main.cpp ./src/MainMenuState.hpp ./src/MainMenuState.cpp ./src/Rifle.hpp ./src/Rifle.cpp ./src/RifleCheckoutState.hpp ./src/RifleCheckoutState.cpp ./src/RifleFileSystem.hpp ./src/RifleFileSystem.cpp ./src/RifleListState.hpp ./src/RifleListState.cpp ./src/RifleManager.hpp ./src/RifleManager.cpp ./src/SerialInputBox.hpp ./src/SerialInputBox.cpp ./src/SettingsParser.hpp ./src/SettingsParser.cpp ./src/SettingsState.hpp ./src/SettingsState.cpp ./src/Password.hpp ./src/Password.cpp ./src/PicoSHA256.hpp ./src/NewRifleState.hpp ./src/NewRifleState.cpp ./src/RifleInformationState.hpp ./src/RifleInformationState.cpp)

# Detect and add SFML
# only do this on unix, because I don't know if it works on other platforms and toolchains
# I know it doesn't work on Windows for Visual Studio
if(UNIX)
	set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
	find_package(SFML 2 REQUIRED network audio graphics window system)
	if(SFML_FOUND)
	  include_directories(${SFML_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/SFUI)
	  target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
	endif()

	link_directories(${CMAKE_SOURCE_DIR}/SFUI/)
	target_link_libraries(riflemanager ${CMAKE_SOURCE_DIR}/SFUI/SFUI.a)
	target_link_libraries(riflemanager stdc++fs)
endif(UNIX)

set(${CMAKE_BUILD_TYPE} Debug)

if(WIN32)
	target_include_directories(riflemanager PUBLIC ${CMAKE_SOURCE_DIR}/SFML/include)
	target_include_directories(riflemanager PUBLIC ${CMAKE_SOURCE_DIR}/SFUI/include)
	
	target_link_libraries(riflemanager ${CMAKE_SOURCE_DIR}/SFML/lib/sfml-graphics-s${CMAKE_DEBUG_POSTFIX}.lib)
	target_link_libraries(riflemanager ${CMAKE_SOURCE_DIR}/SFML/lib/sfml-window-s${CMAKE_DEBUG_POSTFIX}.lib)
	target_link_libraries(riflemanager ${CMAKE_SOURCE_DIR}/SFML/lib/sfml-system-s${CMAKE_DEBUG_POSTFIX}.lib)
	target_link_libraries(riflemanager opengl32.lib)
	target_link_libraries(riflemanager ${CMAKE_SOURCE_DIR}/SFML/lib/freetype.lib)
	target_link_libraries(riflemanager winmm.lib)
	target_link_libraries(riflemanager gdi32.lib)
	target_link_libraries(riflemanager ${CMAKE_SOURCE_DIR}/SFML/lib/sfml-audio-s${CMAKE_DEBUG_POSTFIX}.lib)
	target_link_libraries(riflemanager ${CMAKE_SOURCE_DIR}/SFML/lib/openal32.lib)
	target_link_libraries(riflemanager ${CMAKE_SOURCE_DIR}/SFML/lib/flac.lib)
	target_link_libraries(riflemanager ${CMAKE_SOURCE_DIR}/SFML/lib/vorbisenc.lib)
	target_link_libraries(riflemanager ${CMAKE_SOURCE_DIR}/SFML/lib/vorbisfile.lib)
	target_link_libraries(riflemanager ${CMAKE_SOURCE_DIR}/SFML/lib/vorbis.lib)
	target_link_libraries(riflemanager ${CMAKE_SOURCE_DIR}/SFML/lib/ogg.lib)
	target_link_libraries(riflemanager ${CMAKE_SOURCE_DIR}/SFML/lib/sfml-network-s${CMAKE_DEBUG_POSTFIX}.lib)
	target_link_libraries(riflemanager ws2_32.lib)
	target_link_libraries(riflemanager ${CMAKE_SOURCE_DIR}/SFUI/lib/SFUI${CMAKE_DEBUG_POSTFIX}.lib)
	
	target_compile_definitions(riflemanager PRIVATE SFML_STATIC)
endif()

# I don't know how to use CMake.
# If you want it in debug, create the config using -DCMAKE_BUILD_TYPE=Release
# I don't know why it's reversed, but it is, so fuck me I guess.
message("This is as close as I can get it:")
message("If you want to compile in Debug, set CMAKE_BUILD_TYPE to Release.")
message("If you want to compile in Release, set CMAKE_BUILD_TYPE to Debug.")

# This only shows up in release, for whatever reason, so there's no real reason to use it. It might be useful later, so I'll keep it here.
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DIS_DEBUG_ENABLED")
