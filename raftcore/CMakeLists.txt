cmake_minimum_required(VERSION 3.5)
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_COMPILER "g++")
SET(RAFRCORE_SRC raft.cpp raft_config.cpp raftlog.cpp memory_storage.cpp volatile_state.cpp logger.cpp)
INCLUDE_DIRECTORIES(./ spdlog/include msgpack-c/include)

add_library(raftcore STATIC ${RAFRCORE_SRC})
