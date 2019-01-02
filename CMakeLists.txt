cmake_minimum_required(VERSION 3.12)
project(Boost_Echo_Client)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_executable(Boost_Echo_Client
        include/connectionHandler.h
        src/connectionHandler.cpp
        src/echoClient.cpp
        makefile include/EncoderDecoder.h src/EncoderDecoder.cpp include/ConnectionServer.h src/ConnectionServer.cpp include/IOTask.h src/IOTask.cpp)
