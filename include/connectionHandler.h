#ifndef CONNECTION_HANDLER__
#define CONNECTION_HANDLER__
                                           
#include <string>
#include <map>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class ConnectionHandler {
private:
	const std::string host_;
	const short port_;
	boost::asio::io_service io_service_;   // Provides core I/O functionality
	tcp::socket socket_;
	std::map<std::string,short> commandDictionary;
	char delimiter = '\0';
 
public:
    ConnectionHandler(std::string host, short port);
    virtual ~ConnectionHandler();
 
    // Connect to the remote machine
    bool connect();
 
    // Read a fixed number of bytes from the server - blocking.
    // Returns false in case the connection is closed before bytesToRead bytes can be read.
    bool getBytes(char bytes[], unsigned int bytesToRead);
 
	// Send a fixed number of bytes from the client - blocking.
    // Returns false in case the connection is closed before all the data is sent.
    bool sendBytes(const char bytes[], int bytesToWrite);
	
    // Read an ascii line from the server
    // Returns false in case connection closed before a newline can be read.
    bool getLine(std::string& line);
	
	// Send an ascii line from the server
    // Returns false in case connection closed before all the data is sent.
    bool sendLine(std::string& line);
 
    // Get Ascii data from the server until the delimiter character
    // Returns false in case connection closed before null can be read.
    bool getFrameAscii(std::string& frame, char delimiter);
 
    // Send a message to the remote host.
    // Returns false in case connection is closed before all the data is sent.
    bool sendFrameAscii(const std::string& frame, char delimiter);
	
    // Close down the connection properly.
    void close();

    //convert bytes to short
    short bytesToShort(char* bytesArr);

    //convert short to bytes
    void shortToBytes(short num, char* bytesArr);

    char* stringToMessage(std::string input);

    char* registerAndLoginToMessage(std::string input, char *ch_Opcode);

    char* followToMessage(std::string input, char *ch_Opcode);

    void insertElementsToFollowInput(char *ch_Opcode, char &yesOrNo, std::vector<std::string> &names,
                                     char* output);

    char* postOrStatToMessage(std::string input, char *ch_Opcode);

    char* pmToMessage(std::string input, char *ch_Opcode);


}; //class ConnectionHandler
 
#endif