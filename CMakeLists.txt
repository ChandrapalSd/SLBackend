cmake_minimum_required(VERSION 3.22.0)
project(SLBackend VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)


include_directories(include)
	
file(GLOB SOURCES src/*.cpp)
file(GLOB HEADERS include/*.hpp)

set(BUILD_SHARED_LIBS OFF)

#if (MSVC)
#  add_compile_options(/bigobj)
#else ()
#  add_compile_options(-Wa,-mbig-obj)
#endif ()

#define target
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

#pch
#target_precompile_headers(${PROJECT_NAME} PRIVATE include/pch.hpp)

#dependencies

#target_link_libraries(${PROJECT_NAME} PUBLIC)							   

target_compile_definitions(${PROJECT_NAME} PUBLIC)

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

