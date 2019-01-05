CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-pthread -lboost_system

all: BGSclient
	g++ -o bin/BGSclient bin/ConnectionHandler.o bin/ClientRequestTask.o bin/ServerListenerTask.o bin/EncoderDecoder.o bin/echoClient.o $(LDFLAGS)
	@echo 'finished building'
BGSclient: bin/ConnectionHandler.o bin/ClientRequestTask.o bin/ServerListenerTask.o bin/EncoderDecoder.o bin/echoClient.o



bin/ConnectionHandler.o: src/ConnectionHandler.cpp
	g++ $(CFLAGS) -o bin/ConnectionHandler.o src/ConnectionHandler.cpp

bin/ClientRequestTask.o: src/ClientRequestTask.cpp
	g++ $(CFLAGS) -o bin/ClientRequestTask.o src/ClientRequestTask.cpp

bin/ServerListenerTask.o: src/ServerListenerTask.cpp
	g++ $(CFLAGS) -o bin/ServerListenerTask.o src/ServerListenerTask.cpp

bin/EncoderDecoder.o: src/EncoderDecoder.cpp
	g++ $(CFLAGS) -o bin/EncoderDecoder.o src/EncoderDecoder.cpp

bin/echoClient.o: src/echoClient.cpp
	g++ $(CFLAGS) -o bin/echoClient.o src/echoClient.cpp

.PHONY: clean
clean:
	rm -f bin/*
	@echo 'finished clean'
