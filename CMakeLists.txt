cmake_minimum_required(VERSION 3.12)
project(Boost_Echo_Client)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost COMPONENTS system filesystem REQUIRED)
IF(Boost_FOUND)
    include_directories(${Boost_LIBRARY_DIR})
endif()
#link_directories(${Boost_LIBRARY_DIR})
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}-std=c++11 -pthread -lboost_system")

include_directories(include)

add_executable(Boost_Echo_Client
        include/connectionHandler.h
        src/connectionHandler.cpp
        src/echoClient.cpp
        makefile src/IOTask.cpp include/IOTask.h src/ConnectionServer.cpp include/ConnectionServer.h src/EncoderDecoder.cpp include/EncoderDecoder.h)
target_link_libraries(Boost_Echo_Client LINK_PUBLIC ${Boost_LIBRARIES})