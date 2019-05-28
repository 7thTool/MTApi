#pyMTApi/CMakeLists.txt
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(pyMTApi)

#版本号，注意0会导致CONFIGURE_FILE替换@VAR@失败
SET(pyMTApi_VERSION_MAJOR 1)
SET(pyMTApi_VERSION_MINOR 1)
SET(pyMTApi_VERSION_BUILD 1)
SET(pyMTApi_VERSION "${pyMTApi_VERSION_MAJOR}.${pyMTApi_VERSION_MINOR}.${pyMTApi_VERSION_BUILD}")


IF(WIN32)
	#需要兼容XP时,定义_WIN32_WINNT 0x0501
	ADD_DEFINITIONS(-D_WIN32_WINNT=0x0601)
ELSE()
	ADD_DEFINITIONS(-std=c++11)
	#ADD_DEFINITIONS(-fpermissive)
	ADD_DEFINITIONS(-fPIC)
    INCLUDE_DIRECTORIES(/usr/local/include)
	LINK_DIRECTORIES(/usr/local/lib)
ENDIF()
CMAKE_POLICY(SET CMP0015 NEW)

#编译boost-python boost-numpy，需要先安装python、numpy环境，设置环境变量set PYTHON_ROOT=C:\Anaconda3 set PYTHON_VERSION=3.7
#.\bjam stage address-model=64 architecture=x86 --toolset=msvc-14.0 --with-python link=static runtime-link=static threading=multi debug release --build-type=complete
#https://svn.boost.org/trac10/ticket/13527  Cannot link to boost_pythonPY_MAJOR_VERSIONPY_MINOR_VERSION bug解决方法
FIND_PACKAGE(Python)
IF(Python_FOUND)
	MESSAGE(STATUS "Python library status:")
	MESSAGE(STATUS "     version: ${Python_VERSION}")
  	SET(Python_INCLUDE_DIRS "$ENV{PYTHON_ROOT}/include")
  	SET(Python_LIBRARY_DIRS "$ENV{PYTHON_ROOT}/libs")
ELSE()
	#MESSAGE(FATAL_ERROR "Python not found")
  	SET(Python_VERSION "3.7")
  	SET(Python_INCLUDE_DIRS "$ENV{PYTHON_ROOT}/include")
  	SET(Python_LIBRARY_DIRS "$ENV{PYTHON_ROOT}/libs")
ENDIF()
INCLUDE_DIRECTORIES(${Python_INCLUDE_DIRS})
LINK_DIRECTORIES(${Python_LIBRARY_DIRS})
MESSAGE(STATUS "python include path: ${Python_INCLUDE_DIRS}")
MESSAGE(STATUS "python library path: ${Python_LIBRARY_DIRS}")

FIND_PACKAGE(Boost)
IF(Boost_FOUND)
	MESSAGE(STATUS "Boost library status:")
	MESSAGE(STATUS "     version: ${Boost_VERSION}")
ELSE()
	if(BOOST_ROOT)
		set(Boost_INCLUDE_DIR ${BOOST_ROOT})
		IF(WIN32)
			SET(Boost_LIBRARY_DIRS "$ENV{BOOST_ROOT}/lib")
		ELSE()
			set(Boost_LIBRARY_DIRS $ENV{BOOST_ROOT}/output/stage/linux/lib)
		ENDIF()
	else()
		MESSAGE(FATAL_ERROR "BOOST library not found")
	endif()
ENDIF()
IF(WIN32)
	ADD_DEFINITIONS(-DBOOST_PYTHON_STATIC_LIB)
	ADD_DEFINITIONS(-DBOOST_NUMPY_STATIC_LIB)
ELSE()
	#
ENDIF()
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
MESSAGE(STATUS "boost include path: ${Boost_INCLUDE_DIR}")
MESSAGE(STATUS "boost library path: ${Boost_LIBRARY_DIRS}")

SET(SRCS 
./pyMTApi.cpp
)

#添加头文件搜索路径
INCLUDE_DIRECTORIES(../../local ../../local/include ../include)
#添加库文件搜索路径
LINK_DIRECTORIES(../../local/lib64 ../../local/lib ${CMAKE_BINARY_DIR}/bin)

IF(WIN32)
	SET (EXTRA_LIBS ${EXTRA_LIBS}  XUtil MDSet MTApi)
ELSE()
	SET (EXTRA_LIBS ${EXTRA_LIBS}  XUtil MDSet MTApi -pthread dl)
ENDIF()

#设置输出位置
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

ADD_LIBRARY(pyMTApi SHARED ${SRCS})
TARGET_LINK_LIBRARIES(pyMTApi ${EXTRA_LIBS})
set_target_properties(pyMTApi PROPERTIES SUFFIX .pyd)

