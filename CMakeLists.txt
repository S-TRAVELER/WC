cmake_minimum_required(VERSION 2.8)

project(WC_win)

OPTION(ENABLE_GCOV "Enable gcov (debug, Linux builds only)" OFF)

IF (ENABLE_GCOV AND NOT WIN32 AND NOT APPLE)
  set(GCOV_PREFIX ${PROJECT_BINARY_DIR}/bin)
  set(GCOV_PREFIX_STRIP 1)
  SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
  SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
  SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage -lgcov")
  set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)
ENDIF()


#设置库文件路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
#设置可执行文件路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(SUB_DIR_LIST "Cmd" "Util")

if(WIN32)
    list(APPEND SUB_DIR_LIST "win32")
    #防止Windows.h包含Winsock.h
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

#安装目录
if(WIN32)
    set(INSTALL_PATH_LIB $ENV{HOME}/${CMAKE_PROJECT_NAME}/lib)
    set(INSTALL_PATH_INCLUDE $ENV{HOME}/${CMAKE_PROJECT_NAME}/include)
else()
    set(INSTALL_PATH_LIB lib)
    set(INSTALL_PATH_INCLUDE include)
endif()

foreach(SUB_DIR ${SUB_DIR_LIST})
    #遍历源文件
    aux_source_directory(src/${SUB_DIR} SRC_LIST)
    #安装头文件至系统目录
    install(DIRECTORY src/${SUB_DIR} DESTINATION ${INSTALL_PATH_INCLUDE} FILES_MATCHING PATTERN "*.h")
endforeach()

if(WIN32)
    set(LINK_LIB_LIST WS2_32 Iphlpapi shlwapi)
else()
    set(LINK_LIB_LIST)
endif()

#打印库文件
message(STATUS "将链接依赖库:${LINK_LIB_LIST}")
#引用头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
#使能c++11
add_compile_options(-std=c++11)

if(NOT WIN32)
    add_compile_options(-Wno-deprecated-declarations)
    add_compile_options(-Wno-predefined-identifier-outside-function)
endif()

#编译动态库
add_library(${CMAKE_PROJECT_NAME}_shared SHARED ${SRC_LIST})
target_link_libraries(${CMAKE_PROJECT_NAME}_shared ${LINK_LIB_LIST})
set_target_properties(${CMAKE_PROJECT_NAME}_shared PROPERTIES OUTPUT_NAME "${CMAKE_PROJECT_NAME}")
install(TARGETS ${CMAKE_PROJECT_NAME}_shared  ARCHIVE DESTINATION ${INSTALL_PATH_LIB} LIBRARY DESTINATION ${INSTALL_PATH_LIB})



add_subdirectory(tests)
add_subdirectory(wc_gui)
