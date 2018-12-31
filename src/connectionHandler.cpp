#include <connectionHandler.h>
 
using boost::asio::ip::tcp;

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
 
ConnectionHandler::ConnectionHandler(string host, short port): host_(host), port_(port), io_service_(), socket_(io_service_){
    this->commandDictionary.insert(std::pair<string,short>("REGISTER",1));
    this->commandDictionary.insert(std::pair<string,short>("LOGIN",2));
    this->commandDictionary.insert(std::pair<string,short>("LOGOUT",3));
    this->commandDictionary.insert(std::pair<string,short>("FOLLOW",4));
    this->commandDictionary.insert(std::pair<string,short>("POST",5));
    this->commandDictionary.insert(std::pair<string,short>("PM",6));
    this->commandDictionary.insert(std::pair<string,short>("USERLIST",7));
    this->commandDictionary.insert(std::pair<string,short>("STAT",8));

}
    
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
 
bool ConnectionHandler::getLine(std::string& line) {
    return getFrameAscii(line, '\n');
}

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
            frame.append(1, ch);
        }while (delimiter != ch);
    } catch (std::exception& e) {
        std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}
 
bool ConnectionHandler::sendFrameAscii(const std::string& frame, char delimiter) {
    //string is converted to a char array the server can understand.
    char* toSend = stringToMessage(frame);
	bool result = sendBytes(toSend,frame.length());
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



short ConnectionHandler::bytesToShort(char* bytesArr)
{
    short result = (short)((bytesArr[0] & 0xff) << 8);
    result += (short)(bytesArr[1] & 0xff);
    return result;
}

void ConnectionHandler::shortToBytes(short num, char* bytesArr)
{
    bytesArr[0] = ((num >> 8) & 0xFF);
    bytesArr[1] = (num & 0xFF);
}

char* ConnectionHandler::registerAndLoginToMessage(std::string input, char *ch_Opcode){
    //register case
    std::string userName(input.substr(0,input.find_first_of(" ")));
    input = input.substr(input.find_first_of(" ") + 1);
    std::string password(input.substr(0,input.find_first_of(" ")));
    //creating output char* in the size of the opcode,username and password
    char* output[userName.length() + password.length() + 4];
    output[0] = &ch_Opcode[0];
    output[1] = &ch_Opcode[1];
    int index = 2;
    for (char &i : userName) {
        output[index] = &i;
        index++;
    }
    output[index] = &this->delimiter;
    index++;
    for (char &i : password) {
        output[index] = &i;
        index++;
    }
    output[index] = &this->delimiter;
    return *output;
}

char*  ConnectionHandler::postOrStatToMessage(std::string input, char *ch_Opcode){
    char* output[2 + input.length() + 1];
    output[0] = &ch_Opcode[0];
    output[1] = &ch_Opcode[1];
    int index = 2;
    for (char &i : input) {
        output[index] = &i;
        index++;
    }
    output[index] = &this->delimiter;
    return *output;

}

char* ConnectionHandler::pmToMessage(std::string input, char *ch_Opcode) {
    std::string userName(input.substr(0,input.find_first_of(" ")));
    input = input.substr(input.find_first_of(" ") + 1);
    char* output[2 + userName.length() + input.length() + 2];
    output[0] = &ch_Opcode[0];
    output[1] = &ch_Opcode[1];
    int index = 2;
    for (char &i : userName) {
        output[index] = &i;
        index++;
    }
    output[index] = &this->delimiter;
    for (char &i : input) {
        output[index] = &i;
        index++;
    }
    output[index] = &this->delimiter;
    return *output;

}

char* ConnectionHandler::followToMessage(std::string input, char *ch_Opcode) {
    //follow case
    std::locale loc;
    char yesOrNo;
    std::string followOrNot = std::toupper(input.substr(0,input.find_first_of(" ")),loc);
    input = input.substr(input.find_first_of(" ") + 1);
    if(followOrNot=="FOLLOW"){
        yesOrNo = '0';
    }
    else{
        yesOrNo = '1';
    }
    short numberOfUsers = (short)std::stoi(input.substr(0,input.find_first_of(" ")));
    input = input.substr(input.find_first_of(" ") + 1);
    char* ch_numberOfUsers[2];
    this->shortToBytes(numberOfUsers,*ch_numberOfUsers);
    std::vector<string> names;
    int counter = 0;
    unsigned long end = input.find_first_of(" ");
    while (end != input.npos){
        std::string current = input.substr(0,input.find_first_of(" "));
        input = input.substr(input.find_first_of(" ") + 1);
        end = input.find_first_of(" ");
        counter +=current.length();
        names.push_back(current);
    }
    //the output needs to contains:
    //1.the opcode
    //2.the follow\unfollow
    //3.the names (names) and 0 between them (names.size)
    char* output[2 + 1 + 2 + counter+names.size()];
    insertElementsToFollowInput(ch_Opcode, yesOrNo, names, *output);
    return *output;
}

void ConnectionHandler::insertElementsToFollowInput(char *ch_Opcode, char &yesOrNo, std::vector<std::string> &names,
                                               char* output)  {
    output[0] = ch_Opcode[0];
    output[1] = ch_Opcode[1];
    output[2] = yesOrNo;
    int index = 3;
    for(int i = 0; i < names.size(); i++){
        for(int j = 0; j<names[i].length(); j++){
            output[index] = names[i].at(j);
            index++;
        }
        output[index] = this->delimiter;
        index++;
    }
}

char* ConnectionHandler::stringToMessage(std::string input){

    char* ch_Opcode[2];
    std::locale loc;
    std::string command = std::toupper(input.substr(0,input.find_first_of(" ")),loc);
    input = input.substr(input.find_first_of(" ") + 1);
    short opcode = this->commandDictionary.at(command);
    this->shortToBytes(opcode, *ch_Opcode);
    switch (opcode){
        case 1:
            //register case
           return registerAndLoginToMessage(input, *ch_Opcode);

        case 2:
            //login case
            return registerAndLoginToMessage(input, *ch_Opcode);
        case 3:
            //logout case
            return *ch_Opcode;
        case 4:
            return followToMessage(input,*ch_Opcode);
        case 5:
            //post case
            return postOrStatToMessage(input, *ch_Opcode);
        case 6:
            //pm case
            return pmToMessage(input,*ch_Opcode);
        case 7:
            //User list case
            return *ch_Opcode;
        default:
            //stat case
            return postOrStatToMessage(input, *ch_Opcode);
    }


}