cmake_minimum_required(VERSION 3.0)

project(tinyrpc)


#配置编译选项
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)


#配置最终可执行文件输出的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)




# #指定搜索的子目录，工程有很多级目录，此时我们需要指定要搜索的子目录
# add_subdirectory(testmuduo)   #他会去这个目录下去找CMakeLists.txt然后在运行

#定义了一个SRC_LIST变量，包含了该目录下所有的源文件
aux_source_directory(. SRC_LIST)

aux_source_directory(./tinyrpc/common COM_LIST)
aux_source_directory(./tinyrpc/net NET_LIST)




#配置头文件的搜索路径
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/net)

# 定义生成 test_log 的源文件
set(TEST_LOG_SRC ${SRC_LIST} ${COM_LIST} ${NET_LIST} ${PROJECT_SOURCE_DIR}/testcases/test_log.cc)

# 定义生成 test_eventloop 的源文件
set(TEST_EVENTLOOP_SRC ${SRC_LIST} ${COM_LIST} ${NET_LIST} ${PROJECT_SOURCE_DIR}/testcases/test_eventloop.cc)

#指定生成可执行文件
add_executable(test_log ${TEST_LOG_SRC})
add_executable(test_eventloop ${TEST_EVENTLOOP_SRC})



#指定可执行文件链接时所需要依赖的库文件
target_link_libraries(test_log  pthread)
target_link_libraries(test_eventloop  pthread)
