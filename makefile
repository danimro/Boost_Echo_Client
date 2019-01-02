#CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
#LDFLAGS:=-lboost_system
# All Targets
all: BGSclient


BGSclient: bin/echoClient.o bin/EncoderDecoder.o bin/connectionHandler.o
    @echo 'Building target: BGSclient'
    @echo 'Invoking: C++ Linker'
    g++ -o bin/BGSclient bin/echoClient.o bin/EncoderDecoder.o bin/connectionHandler.o -lboost_system
    @echo 'Finished building target: rest'
    @echo ' '


bin/echoClient.o: src/echoClient.cpp
	g++ -c -Wall -Weffc++ -g -std=c++11 -Iinclude -o bin/echoClient.o src/echoClient.cpp


bin/connectionHandler.o: src/connectionHandler.cpp
	g++ -c -Wall -Weffc++ -g -std=c++11 -Iinclude -o bin/connectionHandler.o src/connectionHandler.cpp


bin/EncoderDecoder.o: src/EncoderDecoder.cpp
    g++ -c -Wall -Weffc++ -g -std=c++11 -Iinclude -o bin/connectionHandler.o src/connectionHandler.cpp

	
.PHONY: clean
clean:
	rm -f bin/*
