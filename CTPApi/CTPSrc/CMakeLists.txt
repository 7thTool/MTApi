#CTPSrc/CMakeLists.txt
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#CMAKE_SIZEOF_VOID_P 表示 void* 的大小（例如为 4 或者 8），可以使用其来判断当前构建为 32 位还是 64 位

ADD_DEFINITIONS(-DXSERVER_PROTOTYPE=XSERVER_HTTP)

#CMAKE_C_COMPILER：指定C编译器
#CMAKE_CXX_COMPILER：指定C++编译器
IF(WIN32)
	#需要兼容XP时,定义_WIN32_WINNT 0x0501
	ADD_DEFINITIONS(-D_WIN32_WINNT=0x0601)
  	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
	#不用加EXPORTS定义，cmake会自动加CTPSrc_EXPORTS
	ADD_DEFINITIONS(-DMTApi_EXPORTS)
	#SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
	#SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	#SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MT")
	#SET(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /MT")
ELSE()
	ADD_DEFINITIONS(-std=c++11)
	#ADD_DEFINITIONS(-fpermissive)
	ADD_DEFINITIONS(-fPIC)
	#-w的意思是关闭编译时的警告，也就是编译后不显示任何warning
	#-Wall选项意思是编译后显示所有警告
	#-W选项类似-Wall，会显示警告，但是只显示编译器认为会出现错误的警告
	#调试信息格式主要有下面几种：stabs，COFF，PE-COFF，OMF，IEEE-695和DWARF
	#其中DWARF在Linux中被普遍使用，dwarf2对dwarf1的改变很大，dwarf3大多是对dwarf2的扩充，可以支持C、C++、JAVA、Fortran等语言
	#使用readelf –w* CTPSrc命令，*是调试节名的第一个字母，如-wi就是查看.debug_info节的内容，-wl就是查看.debug_line节的内容
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

#添加头文件搜索路径
INCLUDE_DIRECTORIES(../../../local ../../../local/include ../../include ../include ../include/${CTP_PREFIX})
#添加库文件搜索路径
LINK_DIRECTORIES(../../../local/lib64 ../../../local/lib ${CMAKE_BINARY_DIR}/bin ../lib/${CTP_PREFIX})

IF(WIN32)
	SET (EXTRA_LIBS ${EXTRA_LIBS} XUtil MDSet thostmduserapi thosttraderapi)
ELSE()
	#要将thostmduserapi.so和thostmduserapi.so改为libthostmduserapi.so和libthostmduserapi.so
	SET (EXTRA_LIBS ${EXTRA_LIBS} XUtil MDSet thostmduserapi thosttraderapi)
ENDIF()

#设置输出位置
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

#同时生产动态库和静态库
ADD_LIBRARY(CTPApi SHARED 
	../src/CTPCommon.cpp
	CTPMarketApi.cpp
	CTPTraderApi.cpp
	CTPApi.cpp
)
TARGET_LINK_LIBRARIES(CTPApi ${EXTRA_LIBS})

#按照规则，动态库是应该包含一个版本号的，为了实现动态库版本号，我们仍然需要使用 SET_TARGET_PROPERTIES 指令。
#VERSION指代动态库版本，SOVERSION 指代 API 版本。
#SET_TARGET_PROPERTIES(CTPSrc PROPERTIES VERSION "${CTPAPI_VERSION_MAJOR}.${CTPAPI_VERSION_MINOR}" SOVERSION ${CTPAPI_VERSION_BUILD})

# 添加可执行文件
ADD_EXECUTABLE(CTPSrc
	../src/CTPCommon.cpp
	CTPMarketApi.cpp
	CTPTraderApi.cpp
	CTPSrc.cpp
)
TARGET_LINK_LIBRARIES(CTPSrc ${EXTRA_LIBS})
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)