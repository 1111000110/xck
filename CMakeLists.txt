cmake_minimum_required(VERSION 3.0)
project(ZX)
set(LIB_SRC code/Log/Log_CC)#设置变量
file(GLOB LOG code/Log/Log_CC/*.cc)#将code/Log/Log_CC/*.cc所有文件封装成环境变量LOG
file(GLOB CONFIG code/Config/Config_CC/*.cc)
file(GLOB THREAD code/Thread/Thread_CC/*.cc)
file(GLOB FIBER code/Fiber/Fiber_CC/*.cc)
#生成动态库
add_library(util SHARED code/util.cc)
add_library(fiber SHARED ${FIBER})
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
add_executable(test_util tests/test_util.cc)#生成test_config可执行文件
add_executable(test_fiber tests/test_fiber.cc)#
add_executable(test_scheduler tests/test_scheduler.cc )

set(count   fiber util   log thread  config )
set(count1 util  log thread  config fiber)
#生成可执行文件时链接的
target_link_libraries(test_config  ${count1} ) 
target_link_libraries(test_thread  ${count1} )
target_link_libraries(test_util  ${count1} )
target_link_libraries(test_fiber ${count1} )
target_link_libraries(test_scheduler ${count})
target_link_libraries(test ${count1})
#可执行文件依赖的
add_dependencies(test_config  ${count})#main文件对动态库的依赖
add_dependencies(test_thread  ${count})
add_dependencies(test_util  ${count})
add_dependencies(test_fiber ${count})
add_dependencies(test_scheduler ${count})
add_dependencies(test ${count})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)