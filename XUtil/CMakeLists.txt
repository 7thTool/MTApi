# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

MESSAGE(STATUS "${CMAKE_SYSTEM_NAME} ${CMAKE_CL_64}")

MESSAGE(STATUS "This is XUtil PROJECT BINARY dir " ${PROJECT_BINARY_DIR})
MESSAGE(STATUS "This is XUtil CMAKE BINARY dir " ${CMAKE_BINARY_DIR})
MESSAGE(STATUS "This is XUtil PROJECT SOURCE dir " ${PROJECT_SOURCE_DIR})
MESSAGE(STATUS "This is XUtil CMAKE SOURCE dir " ${CMAKE_SOURCE_DIR})

MESSAGE(STATUS "This is XUtil current BINARY dir " ${CMAKE_CURRENT_BINARY_DIR})
MESSAGE(STATUS "This is XUtil current SOURCE dir " ${CMAKE_CURRENT_SOURCE_DIR})
MESSAGE(STATUS "This is XUtil current CMAKE SOURCE dir " ${CMAKE_CURRENT_SOURCE_DIR})
MESSAGE(STATUS "This is XUtil current CMakeList.txt dir " ${CMAKE_CURRENT_LIST_DIR})
MESSAGE(STATUS "This is XUtil current CMakeList.txt line " ${CMAKE_CURRENT_LIST_LINE})

MESSAGE(STATUS "This is XUtil library output " ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

IF(WIN32)
	ADD_DEFINITIONS(-std=c++11)
        add_definitions(-DWIN32 -D_WINDOWS)
	#需要兼容XP时,定义_WIN32_WINNT 0x0501
	ADD_DEFINITIONS(-D_WIN32_WINNT=0x0602)
	#SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
	#SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	#SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MT")
	#SET(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /MT")
ELSE()
	ADD_DEFINITIONS(-std=c++11)
	ADD_DEFINITIONS(-fpermissive)
	ADD_DEFINITIONS(-fPIC)
	#-w的意思是关闭编译时的警告，也就是编译后不显示任何warning
	#-Wall选项意思是编译后显示所有警告
	#-W选项类似-Wall，会显示警告，但是只显示编译器认为会出现错误的警告
	#调试信息格式主要有下面几种：stabs，COFF，PE-COFF，OMF，IEEE-695和DWARF
	#其中DWARF在Linux中被普遍使用，dwarf2对dwarf1的改变很大，dwarf3大多是对dwarf2的扩充，可以支持C、C++、JAVA、Fortran等语言
	#使用readelf –w* transfer命令，*是调试节名的第一个字母，如-wi就是查看.debug_info节的内容，-wl就是查看.debug_line节的内容
	#-g、-ggdb、-g3和-ggdb3，-g产生OS native format的debug信息，GDB可以使用之。而-ggdb产生的debug信息更倾向于给GDB使用的
	#如果你用的GDB调试器，那么使用-ggdb选项。如果是其他调试器，则使用-g。3只是debug信息级别，3这个级别可以调试宏。
	#SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -w -gdwarf-2 -ggdb3")
	#SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -Wall -DNODEBUG -gdwarf-2 -ggdb")
	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -W -g3")
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -W")
	SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2 -W -gdwarf-3 -g")
	SET(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -Os -W")
  	INCLUDE_DIRECTORIES(/usr/local/include)
	LINK_DIRECTORIES(/usr/local/lib)
ENDIF()
CMAKE_POLICY(SET CMP0015 NEW)

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

INCLUDE_DIRECTORIES(../include ../../local ../../local/XUtil)
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/bin)

aux_source_directory(. libSRC_LIST)
set(libSRC_LIST ${libSRC_LIST} 
../../local/XUtil/XController.cpp 
../../local/XUtil/XLogger.cpp
)

#设置输出位置
add_library( # Sets the name of the library.
        XUtil

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ${libSRC_LIST}
        )
TARGET_LINK_LIBRARIES(XUtil ${EXTRA_LIBS})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

