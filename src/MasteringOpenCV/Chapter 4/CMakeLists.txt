project(ExploringSfMWithOpenCV)
cmake_minimum_required(VERSION 2.8)

find_package(PCL REQUIRED)

find_package(OpenCV REQUIRED)
find_package(OpenMP REQUIRED)

IF(APPLE)
	set( COCOA_LIBS ${CMAKE_OSX_SYSROOT}/System/Library/Frameworks/Cocoa.framework )
	set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -F/usr/local/lib -L/opt/local/lib")
ENDIF(APPLE)

include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

link_directories(${SSBA_LIBRARY_DIR})
include_directories(${SSBA_LIBRARY_DIR}/../)

set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D__SFM__DEBUG__" )

# Core functions of SfM
file(GLOB SFM_LIB_SOURCES	
	Distance.* 
	Triangulation.* 
	FeatureMatching.*
	FindCameraMatrices.*
	MultiCameraDistance.*
	IDistance.*
	MultiCameraPnP.*
	Common.*
	IFeatureMatcher.*
	RichFeatureMatcher.*
	OFFeatureMatcher.*
	BundleAdjuster.*
)

add_library( ExploringSfMLibrary ${SFM_LIB_SOURCES} )

# UI part
add_executable(ExploringSfMExec 
	Visualization.cpp
	Visualization.h
	main.cpp
	)


target_link_libraries(ExploringSfMExec 
			ExploringSfMLibrary
			${OpenCV_LIBS}
			${PCL_LIBRARIES}
			V3D
			colamd
			)