cmake_minimum_required (VERSION 2.6)

project(Brickware-Engine)

set(GL_SUPPORT TRUE)
set(D3D_SUPPORT FALSE)
set(BUILD_TEST TRUE)

set(BUILD_TEST "" CACHE BOOL "Whether or not to build the test")

if(WIN32)
	set(GLFW_INCLUDE "" CACHE PATH "GLFW Include Path")
	set(GLFW_LIB "" CACHE PATH "GLFW Lib Path")

	set(GLEW_INCLUDE "" CACHE PATH "GLFW Include Path")
	set(GLEW_LIB "" CACHE PATH "GLFW Lib Path")
	
	set(GL_SUPPORT "GL_SUPPORT" CACHE BOOL "")
	set(D3D_SUPPORT "D3D_SUPPORT" CACHE BOOL "")
	
	
	if(GL_SUPPORT AND D3D_SUPPORT)
		set(GL_SUPPORT FALSE)
	endif(GL_SUPPORT AND D3D_SUPPORT)
endif(WIN32)

if(D3D_SUPPORT)
	add_definitions(-DD3D_SUPPORT)
else(D3D_SUPPORT)
	add_definitions(-DGL_SUPPORT)
endif(D3D_SUPPORT)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(BUILD_DIR ${CMAKE_BINARY_DIR}\\TestBuild)

add_subdirectory(BrickwareMath)
add_subdirectory(BrickwareUtils)
add_subdirectory(BrickwareGraphics)
add_subdirectory(BrickwareCore)
if(BUILD_TEST)
add_subdirectory(BrickwareTest)
endif(BUILD_TEST)

if(MSVC)
	set_target_properties( BrickwareMath BrickwareUtils BrickwareGraphics BrickwareCore BrickwareTest
		PROPERTIES 
		ARCHIVE_OUTPUT_DIRECTORY ${BUILD_DIR}
		LIBRARY_OUTPUT_DIRECTORY ${BUILD_DIR}
		RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR})
		
    set_target_properties( BrickwareMath BrickwareUtils BrickwareGraphics BrickwareCore BrickwareTest
		PROPERTIES 
		ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${BUILD_DIR}
		LIBRARY_OUTPUT_DIRECTORY_DEBUG ${BUILD_DIR}
		RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BUILD_DIR})
		
    set_target_properties( BrickwareMath BrickwareUtils BrickwareGraphics BrickwareCore BrickwareTest
		PROPERTIES 
		ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${BUILD_DIR}
		LIBRARY_OUTPUT_DIRECTORY_RELEASE ${BUILD_DIR}
		RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BUILD_DIR})
else(MSVC)
	set_target_properties( BrickwareMath BrickwareUtils BrickwareGraphics BrickwareCore BrickwareTest
		PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY ${BUILD_DIR}
		LIBRARY_OUTPUT_DIRECTORY ${BUILD_DIR}
		RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR}
)
endif(MSVC)