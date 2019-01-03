#include <connectionHandler.h>
 
using boost::asio::ip::tcp;

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;



/**
 * Default constructor
 */
ConnectionHandler::ConnectionHandler(string host, short port): host_(host), port_(port), io_service_(), socket_(io_service_),endDec(){
    //initialising the values of the encoder decoder
    this->endDec.init();
}
/**
* Default destructor
*/
ConnectionHandler::~ConnectionHandler() {
    close();
}
 
bool ConnectionHandler::connect() {
    std::cout << "Starting connect to " 
        << host_ << ":" << port_ << std::endl;
    try {
		tcp::endpoint endpoint(boost::asio::ip::address::from_string(host_), port_); // the server endpoint
		boost::system::error_code error;
		socket_.connect(endpoint, error);
		if (error)
			throw boost::system::system_error(error);
    }
    catch (std::exception& e) {
        std::cerr << "Connection failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}
 
bool ConnectionHandler::getBytes(char bytes[], unsigned int bytesToRead) {
    size_t tmp = 0;
	boost::system::error_code error;
    try {
        while (!error && bytesToRead > tmp ) {
			tmp += socket_.read_some(boost::asio::buffer(bytes+tmp, bytesToRead-tmp), error);			
        }
		if(error)
			throw boost::system::system_error(error);
    } catch (std::exception& e) {
        std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}

bool ConnectionHandler::sendBytes(const char bytes[], int bytesToWrite) {
    int tmp = 0;
	boost::system::error_code error;
    try {
        while (!error && bytesToWrite > tmp ) {
			tmp += socket_.write_some(boost::asio::buffer(bytes + tmp, bytesToWrite - tmp), error);
        }
		if(error)
			throw boost::system::system_error(error);
    } catch (std::exception& e) {
        std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}
 
/*bool ConnectionHandler::getLine(std::string& line) {
    return getFrameAscii(line, '\n');
}*/

bool ConnectionHandler::sendLine(std::string& line) {
    return sendFrameAscii(line, '\n');
}
 
bool ConnectionHandler::getFrameAscii(std::string& frame, char delimiter) {
    char ch;
    // Stop when we encounter the null character.
    // Notice that the null character is not appended to the frame string.
    try {
		do{
			getBytes(&ch, 1);
            //frame.append(1, ch);
        }while (delimiter != ch);
    } catch (std::exception& e) {
        std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}

std::string ConnectionHandler::translateMessage() {
    std::string output;
    char ch;
    std::vector<char> message;
    for(int i = 0;i < 2; i++){
        getBytes(&ch,1);
        message.push_back(ch);
    }
    char ch_tempArray[2] = {message[0],message[1]};
    short opcode = this->endDec.bytesToShort(ch_tempArray);
    //could be only notification, ack, error
    switch (opcode){
        case ACK:
            //need to check the message that was resolved
            for(int i = 0; i < 2; i++){
                getBytes(&ch, 1);
                message.push_back(ch);
            }
            //getting resolved opcode
            ch_tempArray[0] = message[2];
            ch_tempArray[1] = message[3];
            opcode = this->endDec.bytesToShort(ch_tempArray);
            if(opcode == FOLLOW || opcode ==USERLIST){
                //building the string
                //adding the ack
                output.append("ACK ");
                if(opcode == FOLLOW){
                    output.append("4 ");
                }
                else{
                    output.append("7 ");
                }
                //getting the next two bytes to see how many names to read
                for(int i = 0; i < 2; i++){
                    getBytes(&ch, 1);
                    message.push_back(ch);
                }
                ch_tempArray[0] = message[4];
                ch_tempArray[1] = message[5];
                short numberOfUsers = this->endDec.bytesToShort(ch_tempArray);
                //adding number of users
                output.append(std::to_string(numberOfUsers));
                //getting all the users
                for(int i = 0; i < numberOfUsers; i++){
                    output.append(" ");
                    std::string currentName;
                    this->getFrameAscii(currentName,'\0');
                    output.append(currentName);
                }
                return output;
            }
            else if(opcode == STAT){
                output.append("ACK 8 ");
                //getting the next two bytes --> number of posts
                for(int i = 0; i < 2; i++){
                    getBytes(&ch, 1);
                    message.push_back(ch);
                }
                ch_tempArray[0] = message[4];
                ch_tempArray[1] = message[5];
                output.append(std::to_string(this->endDec.bytesToShort(ch_tempArray)));
                output.append(" ");
                //getting the next two bytes --> number of followers
                for(int i = 0; i < 2; i++){
                    getBytes(&ch, 1);
                    message.push_back(ch);
                }
                ch_tempArray[0] = message[6];
                ch_tempArray[1] = message[7];
                output.append(std::to_string(this->endDec.bytesToShort(ch_tempArray)));
                output.append(" ");
                //getting the next two bytes --> number of following
                for(int i = 0; i < 2; i++){
                    getBytes(&ch, 1);
                    message.push_back(ch);
                }
                ch_tempArray[0] = message[8];
                ch_tempArray[1] = message[9];
                output.append(std::to_string(this->endDec.bytesToShort(ch_tempArray)));
                return output;
            }
            else{
                //in case it's one of the following:
                // 1.Register
                // 2.Login
                // 3.Logout
                // 4.Post
                // 5.Pm
                output.append("ACK ");
                output.append(std::to_string(opcode));
                return output;
            }

        case ERROR:
            output.append("ERROR ");
            for(int i = 0; i < 2; i++){
                getBytes(&ch, 1);
                message.push_back(ch);
            }
            //getting resolved opcode
            ch_tempArray[0] = message[2];
            ch_tempArray[1] = message[3];
            opcode = this->endDec.bytesToShort(ch_tempArray);
            output.append(std::to_string(opcode));
            return output;

        default:
            //notification case
            output.append("NOTIFICATION ");
            getBytes(&ch, 1);
            if(ch == '0'){
                output.append("PM ");
            }
            else{
                output.append("Public ");
            }
            std::string postingUser;
            this->getFrameAscii(postingUser,'\0');
            output.append(postingUser);
            output.append(" ");
            std::string content;
            this->getFrameAscii(content,'\0');
            output.append(content);
            return output;
    }
}

char* ConnectionHandler::convertToArray(std::vector<char> &message, int index){
    char* messageArray[message.size()];
    for(char &current:message){
        messageArray[index] = &current;
        index++;
    }
    return *messageArray;
}

bool ConnectionHandler::sendFrameAscii(const std::string& frame, char delimiter) {
    //string is converted to a char array the server can understand.
    std::vector<char> toConvert = this->endDec.stringToMessage(frame);
    toConvert.shrink_to_fit();
    char* toSend = toConvert.data();
	bool result = sendBytes(toSend, frame.length());
	if(!result) return false;
	return sendBytes(&delimiter,1);
}
 
// Close down the connection properly.
void ConnectionHandler::close() {
    try{
        socket_.close();
    } catch (...) {
        std::cout << "closing failed: connection already closed" << std::endl;
    }
}






