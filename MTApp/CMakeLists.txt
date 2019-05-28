# CMake build script for nanomsg example
cmake_minimum_required(VERSION "2.8.1")

set(Projects
	App
)

# add location of platform.hpp for Windows builds
if(WIN32)
  add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)
  add_definitions(-DWIN32 -D_WINDOWS)
  # Same name on 64bit systems
  link_libraries(ws2_32.lib)
endif()

ADD_DEFINITIONS(
	-Wshift-count-overflow 
	#-ferror-limit 
	-DXSERVER_PROTOTYPE=XSERVER_HTTP
)

IF(WIN32)
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

IF(WIN32)
#设置需要的boost 组件
#SET(BOOST_COMPONENTS log_setup log regex date_time chrono system filesystem thread)
#使用cmake find_package 查找boost库位置
#FIND_PACKAGE(Boost REQUIRED COMPONENTS ${BOOST_COMPONENTS})
FIND_PACKAGE(Boost REQUIRED)
IF(Boost_FOUND)
	MESSAGE(STATUS "Boost library status:")
	MESSAGE(STATUS "     version: ${Boost_VERSION}")
	MESSAGE(STATUS "     include path: ${Boost_INCLUDE_DIRS}")
  	#SET(Boost_LIBRARY_DIRS "$ENV{BOOST_ROOT}/lib")
	MESSAGE(STATUS "     library path: ${Boost_LIBRARY_DIRS}")
	IF(WIN32)
	ELSE()
  		ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)
  	ENDIF()
	INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
	LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
ELSE()
	MESSAGE(FATAL_ERROR "BOOST library not found")
ENDIF()
ENDIF()

IF(WIN32)
	#添加头文件搜索路径
	INCLUDE_DIRECTORIES(../../local/x86-windows-static/include ../..)
	#添加库文件搜索路径
	LINK_DIRECTORIES(../../local/x86-windows-static/lib ${CMAKE_BINARY_DIR}/bin)
ELSE()
	#添加头文件搜索路径
	INCLUDE_DIRECTORIES(../../local/include ../.. 
	#/Users/allen/vcpkg/installed/x64-osx/include
	../boost/bcpout
	)
	#添加库文件搜索路径
	LINK_DIRECTORIES(../../local/lib64 ../../local/lib  
	#/Users/allen/vcpkg/installed/x64-osx/lib 
	${CMAKE_BINARY_DIR}/boost
	${CMAKE_BINARY_DIR}/bin)

	IF(LINUX)
	#FIND_LIBRARY(BOOST_LOG_SETUP boost_log_setup ${Boost_LIBRARY_DIRS})
	#FIND_LIBRARY(BOOST_LOG boost_log ${Boost_LIBRARY_DIRS})
	#FIND_LIBRARY(BOOST_REGEX boost_regex ${Boost_LIBRARY_DIRS})
	#FIND_LIBRARY(BOOST_DATE_TIME boost_date_time ${Boost_LIBRARY_DIRS})
	#FIND_LIBRARY(BOOST_CHRONO boost_chrono ${Boost_LIBRARY_DIRS})
	SET (EXTRA_LIBS ${EXTRA_LIBS} 
	#${BOOST_LOG_SETUP} ${BOOST_LOG} ${BOOST_REGEX} ${BOOST_DATE_TIME} ${BOOST_CHRONO}
	#boost_locale boost_system boost_filesystem boost_serialization boost_thread 
	boost -pthread dl)
	ELSE()
	SET (EXTRA_LIBS ${EXTRA_LIBS} 
	#boost_log_setup boost_log boost_regex boost_date_time boost_chrono
	#boost_locale boost_system boost_filesystem boost_serialization boost_thread 
	boost -pthread dl)
	ENDIF()
ENDIF()

foreach(prj ${Projects})

# 添加可执行文件
ADD_EXECUTABLE(${prj} ${prj}.cpp)
TARGET_LINK_LIBRARIES(${prj} ${EXTRA_LIBS})

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

endforeach()


