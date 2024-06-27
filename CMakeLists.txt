cmake_minimum_required(VERSION 3.0)



project(tinyrpc)


# # 或者通过查找程序找到 Clang 可执行文件
# find_program(CLANG_EXECUTABLE NAMES clang clang++)
# if(NOT CLANG_EXECUTABLE)
#     message(FATAL_ERROR "Clang compiler not found! Please install Clang or specify its location.")
# endif()

# # 设置 CMake 使用 Clang 编译器
# set(CMAKE_CXX_COMPILER "${CLANG_EXECUTABLE}")

# set(CMAKE_CXX_STANDARD 11)  # 设置 C++ 标准为 C++11
# set(CMAKE_CXX_STANDARD_REQUIRED ON)  # 设置为必须使用指定的 C++ 标准
# set(CMAKE_CXX_EXTENSIONS OFF)  # 禁用编译器特定的扩展特性

#配置头文件的搜索路径
# include_directories(/usr/include/c++/11)
# include_directories(/usr/include/x86_64-linux-gnu/c++/11)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/common)
include_directories(${PROJECT_SOURCE_DIR}/include/net)
include_directories(${PROJECT_SOURCE_DIR}/include/tcp)
include_directories(${PROJECT_SOURCE_DIR}/include/coder)
include_directories(${PROJECT_SOURCE_DIR}/include/rpc)
include_directories(${PROJECT_SOURCE_DIR}/include/proto)


#配置编译选项
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)


#配置最终可执行文件输出的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)




# #指定搜索的子目录，工程有很多级目录，此时我们需要指定要搜索的子目录
# add_subdirectory(testmuduo)   #他会去这个目录下去找CMakeLists.txt然后在运行

#定义了一个SRC_LIST变量，包含了该目录下所有的源文件
aux_source_directory(./src SRC_LIST)

aux_source_directory(./src/common COM_LIST)
aux_source_directory(./src/net NET_LIST)
aux_source_directory(./src/tcp TCP_LIST)
aux_source_directory(./src/coder CODER_LIST)
aux_source_directory(./src/rpc RPC_LIST)
aux_source_directory(./src/proto PROTO_LIST)






# # 定义生成 test_log 的源文件
# set(TEST_LOG_SRC ${SRC_LIST} ${COM_LIST} ${NET_LIST} ${TCP_LIST} ${PROJECT_SOURCE_DIR}/testcases/test_log.cc)

# 定义生成 test_eventloop 的源文件
# set(TEST_EVENTLOOP_SRC ${SRC_LIST} ${COM_LIST} ${NET_LIST} ${TCP_LIST} ${PROJECT_SOURCE_DIR}/testcases/test_eventloop.cpp)

# # 定义生成 test_tcp 的源文件
# set(TEST_TCP_SRC ${SRC_LIST} ${COM_LIST} ${NET_LIST} ${TCP_LIST} ${CODER_LIST} ${RPC_LIST} ${PROJECT_SOURCE_DIR}/testcases/test_tcp.cpp)

# #
# set(TEST_CLIENT_SRC ${SRC_LIST} ${COM_LIST} ${NET_LIST} ${TCP_LIST} ${CODER_LIST} ${RPC_LIST} ${PROJECT_SOURCE_DIR}/testcases/test_client.cpp)

# 定义生成 test_tcp 的源文件
set(TEST_TCP_SRC ${SRC_LIST} ${COM_LIST} ${NET_LIST} ${TCP_LIST} ${CODER_LIST} ${RPC_LIST} ${PROTO_LIST} ${PROJECT_SOURCE_DIR}/testcases/test_rpc_server.cpp)

#
set(TEST_CLIENT_SRC ${SRC_LIST} ${COM_LIST} ${NET_LIST} ${TCP_LIST} ${CODER_LIST} ${RPC_LIST} ${PROTO_LIST} ${PROJECT_SOURCE_DIR}/testcases/test_rpc_client.cpp)

#指定生成可执行文件
# add_executable(test_log ${TEST_LOG_SRC})
# add_executable(test_eventloop ${TEST_EVENTLOOP_SRC})
add_executable(test_rpc_server ${TEST_TCP_SRC})
add_executable(test_rpc_client ${TEST_CLIENT_SRC})



#指定可执行文件链接时所需要依赖的库文件
# target_link_libraries(test_log  pthread)
# target_link_libraries(test_eventloop  pthread)
# 查找 Protocol Buffers 库
# find_package(Protobuf REQUIRED)
target_link_libraries(test_rpc_server pthread protobuf)
target_link_libraries(test_rpc_client pthread  protobuf)