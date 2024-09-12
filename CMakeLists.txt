cmake_minimum_required(VERSION 3.0)
project(ZX)
#set(LIB_SRC code/Log/Log_CC)#设置变量
file(GLOB LOG code/Log/Log_CC/*.cc)#将code/Log/Log_CC/*.cc所有文件封装成环境变量LOG
file(GLOB CONFIG code/Config/Config_CC/*.cc)
file(GLOB THREAD code/Thread/Thread_CC/*.cc)
#生成动态库
add_library(util SHARED code/util.cc)
add_library(log SHARED ${LOG})
add_library(config SHARED ${CONFIG})
add_library(thread SHARED ${THREAD})
target_link_libraries(log yaml-cpp pthread)#log生成动态库时需要yaml支持
#引入头文件
include_directories(/home/zhangxuan/apps/yaml-cpp/include)
#add_library(log SHARED ${LIB_SRC}/LogAppender.cc ${LIB_SRC}/LogEvent.cc ${LIB_SRC}/LogFormatter.cc ${LIB_SRC}/Logger.cc ${LIB_SRC}/LogLevel.cc)
#可执行文件由哪些生成
add_executable(test tests/test.cc)#生成test可执行文件
add_executable(test_config tests/test_config.cc)#生成test_config可执行文件
add_executable(test_thread tests/test_thread.cc)#生成test_config可执行文件
#可执行文件依赖的
set(count util log config thread )
add_dependencies(test ${count})#main文件对动态库的依赖
add_dependencies(test_config  ${count})#main文件对动态库的依赖
add_dependencies(test_thread  ${count})
#生成可执行文件时链接的
target_link_libraries(test  ${count})
target_link_libraries(test_config ${count} yaml-cpp)
target_link_libraries(test_thread  ${count} yaml-cpp pthread)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)