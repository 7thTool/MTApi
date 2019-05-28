# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)


MESSAGE(STATUS "This is MTApi PROJECT BINARY dir " ${PROJECT_BINARY_DIR})
MESSAGE(STATUS "This is MTApi CMAKE BINARY dir " ${CMAKE_BINARY_DIR})
MESSAGE(STATUS "This is MTApi PROJECT SOURCE dir " ${PROJECT_SOURCE_DIR})
MESSAGE(STATUS "This is MTApi CMAKE SOURCE dir " ${CMAKE_SOURCE_DIR})

MESSAGE(STATUS "This is MTApi current BINARY dir " ${CMAKE_CURRENT_BINARY_DIR})
MESSAGE(STATUS "This is MTApi current SOURCE dir " ${CMAKE_CURRENT_SOURCE_DIR})
MESSAGE(STATUS "This is MTApi current CMAKE SOURCE dir " ${CMAKE_CURRENT_SOURCE_DIR})
MESSAGE(STATUS "This is MTApi current CMakeList.txt dir " ${CMAKE_CURRENT_LIST_DIR})
MESSAGE(STATUS "This is MTApi current CMakeList.txt line " ${CMAKE_CURRENT_LIST_LINE})

ADD_DEFINITIONS(-std=c++11)
ADD_DEFINITIONS(-DXSERVER_PROTOTYPE=XSERVER_HTTP)

# add location of platform.hpp for Windows builds
if(WIN32)
  #需要兼容XP时,定义_WIN32_WINNT 0x0501
  ADD_DEFINITIONS(-D_WIN32_WINNT=0x0601)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
  add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)
  add_definitions(-DWIN32 -D_WINDOWS)
  # Same name on 64bit systems
  link_libraries(ws2_32.lib)
endif()

#设置需要的Boost组件
#SET(BOOST_COMPONENTS log_setup log regex date_time chrono system filesystem thread)
#使用cmake find_package 查找boost库位置
#FIND_PACKAGE(Boost REQUIRED COMPONENTS ${BOOST_COMPONENTS})
FIND_PACKAGE(Boost REQUIRED)
IF(Boost_FOUND)
	MESSAGE(STATUS "Boost library status:")
	MESSAGE(STATUS "     version: ${Boost_VERSION}")
ELSE()
	#MESSAGE(FATAL_ERROR "BOOST library not found")
	if(BOOST_ROOT)
		set(Boost_INCLUDE_DIRS ${BOOST_ROOT})
	else()
		set(Boost_INCLUDE_DIRS ../../boost_1_66_0)
	endif()
ENDIF()
IF(WIN32)
  	SET(Boost_LIBRARY_DIRS "$ENV{BOOST_ROOT}/lib")
ELSE()
	set(Boost_LIBRARY_DIRS $ENV{BOOST_ROOT}/output/stage/linux/lib)
	#使用log动态库需要定义BOOST_LOG_DYN_LINK
  	#ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)
	FIND_LIBRARY(BOOST_LOG_SETUP boost_log_setup ${Boost_LIBRARY_DIRS})
	FIND_LIBRARY(BOOST_LOG boost_log ${Boost_LIBRARY_DIRS})
	FIND_LIBRARY(BOOST_REGEX boost_regex ${Boost_LIBRARY_DIRS})
	FIND_LIBRARY(BOOST_DATE_TIME boost_date_time ${Boost_LIBRARY_DIRS})
	FIND_LIBRARY(BOOST_CHRONO boost_chrono ${Boost_LIBRARY_DIRS})
	SET (EXTRA_LIBS ${EXTRA_LIBS} ${BOOST_LOG_SETUP} ${BOOST_LOG} ${BOOST_REGEX} ${BOOST_DATE_TIME} ${BOOST_CHRONO}
		boost_locale boost_system boost_filesystem boost_serialization boost_thread -pthread dl)
ENDIF()
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
MESSAGE(STATUS "boost include path: ${Boost_INCLUDE_DIRS}")
MESSAGE(STATUS "boost library path: ${Boost_LIBRARY_DIRS}")

#添加头文件搜索路径
INCLUDE_DIRECTORIES(../../local ../../local/include ../include)
#添加库文件搜索路径
LINK_DIRECTORIES(../../local/lib64 ../../local/lib ${CMAKE_BINARY_DIR}/bin)

IF(WIN32)
	SET (EXTRA_LIBS ${EXTRA_LIBS} XUtil MDSet TDSet Indicator)
ELSE()
	SET (EXTRA_LIBS ${EXTRA_LIBS} XUtil MDSet TDSet Indicator)
ENDIF()

#aux_source_directory(./src libSRC_LIST)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        MTApi

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
		./MTApi.cpp
        ./MDApi.cpp
        ./TDApi.cpp
        )
TARGET_LINK_LIBRARIES(MTApi ${EXTRA_LIBS})
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

#按照规则，动态库是应该包含一个版本号的，为了实现动态库版本号，我们仍然需要使用 SET_TARGET_PROPERTIES 指令。
#VERSION指代动态库版本，SOVERSION 指代 API 版本。
#SET_TARGET_PROPERTIES(CTPMarket PROPERTIES VERSION "${CTPAPI_VERSION_MAJOR}.${CTPAPI_VERSION_MINOR}" SOVERSION ${CTPAPI_VERSION_BUILD})
