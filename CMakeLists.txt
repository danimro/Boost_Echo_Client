cmake_minimum_required(VERSION 3.12)
project(Boost_Echo_Client)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(Boost_Echo_Client
        include/connectionHandler.h
        src/connectionHandler.cpp
        src/echoClient.cpp
        makefile include/WielerConnectionHandler.h src/WielerConnectionHandler.cpp src/WielerEchoClient.cpp include/EncoderDecoder.h src/EncoderDecoder.cpp)
