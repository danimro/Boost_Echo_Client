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


/**
 * Converting Char array to a Short number
 * @param bytesArr              Char arrayt to convert
 * @return        Short number that is the bytes value of waht was in the bytes array
 */
short ConnectionHandler::bytesToShort(char* bytesArr)
{
    short result = (short)((bytesArr[0] & 0xff) << 8);
    result += (short)(bytesArr[1] & 0xff);
    return result;
}

/**
 * Converting a short number to array of chars
 * @param num               short number to convert
 * @param bytesArr          Char array to put the number into
 */
void ConnectionHandler::shortToBytes(short num, char* bytesArr)
{
    bytesArr[0] = ((num >> 8) & 0xFF);
    bytesArr[1] = (num & 0xFF);
}

/**
 * Part of the StringToMessage function
 * when the message is identified as a Login or Register request -->
 * processing the rest of the string by the user as a Login or Register (according to the OpCode) Message Type.
 * @param input                 String represent the input that was entered by the user.
 * @param ch_Opcode             char array represents the Opcode of this message.
 * @return      Char Array that represents the final Login or Register message
 */
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

/**
 * Part of the StringToMessage function
 * when the message is identified as a Stat or Post request -->
 * processing the rest of the string by the user as a PM or Stat (according to the OpCode) Message Type.
 * @param input                 String represent the input that was entered by the user.
 * @param ch_Opcode             char array represents the Opcode of this message.
 * @return      Char Array that represents the final PM or Stat message
 */
char*  ConnectionHandler::postOrStatToMessage(std::string input, char *ch_Opcode){
    //creating the output array
    char* output[2 + input.length() + 1];
    //inserting the opcode to the array
    output[0] = &ch_Opcode[0];
    output[1] = &ch_Opcode[1];
    int index = 2;
    //inserting:
    //1. the useranme if it's a Stat message
    //2. the content if it's a post
    for (char &i : input) {
        output[index] = &i;
        index++;
    }
    //adding the '\0' delimiter in the end of the message
    output[index] = &this->delimiter;
    return *output;

}
/**
 * Part of the StringToMessage function
 * when the message is identified as a PM request --> processing the rest of the string by the user as a PM Message Type.
 * @param input                 String represent the input that was entered by the user.
 * @param ch_Opcode             char array represents the Opcode of this message.
 * @return      Char Array that represents the final PM message
 */
char* ConnectionHandler::pmToMessage(std::string input, char *ch_Opcode) {
    //getting the user name to search in the server
    std::string userName(input.substr(0,input.find_first_of(" ")));
    //creating the output array
    input = input.substr(input.find_first_of(" ") + 1);
    char* output[2 + userName.length() + input.length() + 2];
    //inserting the opcode to the output array
    output[0] = &ch_Opcode[0];
    output[1] = &ch_Opcode[1];

    int index = 2;
    //inserting the user name to the array
    for (char &i : userName) {
        output[index] = &i;
        index++;
    }
    //adding '\0' delimiter between the username to the content of the message
    output[index] = &this->delimiter;
    //adding all the content of the message to the output array
    for (char &i : input) {
        output[index] = &i;
        index++;
    }
    //adding the '\0' delimiter
    output[index] = &this->delimiter;
    return *output;

}
/**
 * Part of the StringToMessage function
 * when the message is identified as a follow request --> processing the rest of the string by the user as a Follow Message Type.
 * @param input                 String represent the input that was entered by the user.
 * @param ch_Opcode             char array represents the Opcode of this message.
 * @return      Char Array that represents the final follow message
 */
char* ConnectionHandler::followToMessage(std::string input, char *ch_Opcode) {

    char yesOrNo;
    //getting the "follow or not follow" from the string
    std::string followOrNot = input.substr(0,input.find_first_of(" "));
    input = input.substr(input.find_first_of(" ") + 1);
    if(followOrNot == "0"){
        //follow case
        yesOrNo = '0';
    }
    else{
        // unfollow case
        yesOrNo = '1';
    }
    //taking the number of users in the list from the user input
    short numberOfUsers = (short)std::stoi(input.substr(0,input.find_first_of(" ")));
    input = input.substr(input.find_first_of(" ") + 1);
    char* ch_numberOfUsers[2];
    this->shortToBytes(numberOfUsers,*ch_numberOfUsers);
    //creating a vector to hold the usernames to search in the server
    std::vector<string> names;
    int counter = 0;
    unsigned long end = input.find_first_of(" ");
    while (end != input.npos){
        //as long as there is still a user left to read --> adding it to the names vector
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
    //inserting all the elements in the right order to the output array
    insertElementsToFollowInput(ch_Opcode, yesOrNo, names, *output);
    return *output;
}

/**
 * Part of the FollowToMessage
 * combining all the char array that represents a Follow message,
 * and building one array that include all the information needed by the server.
 * @param ch_Opcode             Char array represent the first two bytes that are the Opcode of the message.
 * @param yesOrNo               Char that represent whether the the user want to follow or unfollow the people in the list.
 * @param names                 Vector of strings represent all the UserNames the current client want to follow or unfollow
 * @param output                Char array to bput all the given information in, in the correct order
 */
void ConnectionHandler::insertElementsToFollowInput(char *ch_Opcode, char &yesOrNo, std::vector<std::string> &names,
                                               char* output)  {
    //inserting the opCode
    output[0] = ch_Opcode[0];
    output[1] = ch_Opcode[1];
    //inserting the yesOrNo char
    output[2] = yesOrNo;
    int index = 3;
    for(int i = 0; i < names.size(); i++){
        //for each name in the vector
        for(int j = 0; j<names[i].length(); j++){
            //inserting all the letters of the user
            output[index] = names[i].at(j);
            index++;
        }
        //after each name --> putting the '\0' delimiter.
        output[index] = this->delimiter;
        index++;
    }
}

/**
 * Convert the input string from the client to a char array to send to the server.
 * @param input         String represent the user input
 * @return              Char* represent a bytes array to send to the server to process
 */
char* ConnectionHandler::stringToMessage(std::string input){

    char* ch_Opcode[2];
    std::locale loc;
    // taking the first word of the sentence to process which kind of request it is from the user
    std::string command = std::toupper(input.substr(0,input.find_first_of(" ")),loc);
    input = input.substr(input.find_first_of(" ") + 1);
    //translating the first word to a Opcode using the commandDictionary.
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