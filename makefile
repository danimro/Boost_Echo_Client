CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system -lboost_thread -lboost_locale

all: BGSclient
	g++ -o bin/BGSclient bin/connectionHandler.o bin/IOTask.o bin/ConnectionServer.o bin/EncoderDecoder.o bin/echoClient.o $(LDFLAGS)

BGSclient: bin/connectionHandler.o bin/IOTask.o bin/ConnectionServer.o bin/EncoderDecoder.o bin/echoClient.o



bin/connectionHandler.o: src/connectionHandler.cpp
	g++ $(CFLAGS) -o bin/connectionHandler.o src/connectionHandler.cpp

bin/IOTask.o: src/IOTask.cpp
	g++ $(CFLAGS) -o bin/IOTask.o src/IOTask.cpp

bin/ConnectionServer.o: src/ConnectionServer.cpp
	g++ $(CFLAGS) -o bin/ConnectionServer.o src/ConnectionServer.cpp

bin/EncoderDecoder.o: src/EncoderDecoder.cpp
	g++ $(CFLAGS) -o bin/EncoderDecoder.o src/EncoderDecoder.cpp

bin/echoClient.o: src/echoClient.cpp
	g++ $(CFLAGS) -o bin/echoClient.o src/echoClient.cpp

.PHONY: clean
clean:
	rm -f bin/*