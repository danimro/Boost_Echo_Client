//
// Created by tomer on 02/01/19.
//


#include <EncoderDecoder.h>
#include <locale>

using namespace std;

/**
 * Initialising the delimiter, and the values of the Messages's opcodes
 */
void EncoderDecoder::init() {
    //Register request = 1
    this->commandDictionary.insert(std::pair<string,short>("REGISTER",1));
    //Login request = 2
    this->commandDictionary.insert(std::pair<string,short>("LOGIN",2));
    //Logout request = 3
    this->commandDictionary.insert(std::pair<string,short>("LOGOUT",3));
    //Follow request = 4
    this->commandDictionary.insert(std::pair<string,short>("FOLLOW",4));
    //Post request = 5
    this->commandDictionary.insert(std::pair<string,short>("POST",5));
    //PM request = 6
    this->commandDictionary.insert(std::pair<string,short>("PM",6));
    //UserList request = 7
    this->commandDictionary.insert(std::pair<string,short>("USERLIST",7));
    //Stat request = 8
    this->commandDictionary.insert(std::pair<string,short>("STAT",8));
    this->zeroDelimiter = '\0';
}

/**
 * Converting Char array to a Short number
 * @param bytesArr              Char array to convert
 * @return        Short number that is the bytes value of what was in the bytes array
 */
short EncoderDecoder::bytesToShort(char *bytesArr) {
    short result = (short)((bytesArr[0] & 0xff) << 8);
    result += (short)(bytesArr[1] & 0xff);
    return result;
}

/**
 * Converting a short number to array of chars
 * @param num               short number to convert
 * @param bytesArr          Char array to put the number into
 */
void EncoderDecoder::shortToBytes(short num, char *bytesArr) {
    bytesArr[0] = ((num >> 8) & 0xFF);
    bytesArr[1] = (num & 0xFF);
}

/**
 * Convert the input string from the client to a char array to send to the server.
 * @param input         String represent the user input
 * @return              Char* represent a bytes array to send to the server to process
 */
char* EncoderDecoder::stringToMessage(std::string input) {
    char *ch_Opcode[2];
    std::locale loc;
    // taking the first word of the sentence to process which kind of request it is from the user
    std::string command = std::toupper(input.substr(0, input.find_first_of(" ")), loc);
    input = input.substr(input.find_first_of(" ") + 1);
    //translating the first word to a Opcode using the commandDictionary.
    short opcode = this->commandDictionary.at(command);
    this->shortToBytes(opcode, *ch_Opcode);
    switch (opcode) {
        case REGISTER:
            //register case
            return registerAndLoginToMessage(input, *ch_Opcode);
        case LOGIN:
            //login case
            return registerAndLoginToMessage(input, *ch_Opcode);
        case LOGOUT:
            //logout case
            return *ch_Opcode;
        case FOLLOW:
            //follow case
            return followToMessage(input, *ch_Opcode);
        case POST:
            //post case
            return postOrStatToMessage(input, *ch_Opcode);
        case PM:
            //pm case
            return pmToMessage(input, *ch_Opcode);
        case USERLIST:
            //User list case
            return *ch_Opcode;
        default:
            //stat case
            return postOrStatToMessage(input, *ch_Opcode);
    }
}

    //region Encoding Functions

/**
 * Part of the StringToMessage function
 * when the message is identified as a Login or Register request -->
 * processing the rest of the string by the user as a Login or Register (according to the OpCode) Message Type.
 * @param input                 String represent the input that was entered by the user.
 * @param ch_Opcode             char array represents the Opcode of this message.
 * @return      Char Array that represents the final Login or Register message
 */
char* EncoderDecoder::registerAndLoginToMessage(std::string input, char *ch_Opcode) {
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
    output[index] = &this->zeroDelimiter;
    index++;
    for (char &i : password) {
        output[index] = &i;
        index++;
    }
    output[index] = &this->zeroDelimiter;
    return *output;
}

/**
 * Part of the StringToMessage function
 * when the message is identified as a follow request --> processing the rest of the string by the user as a Follow Message Type.
 * @param input                 String represent the input that was entered by the user.
 * @param ch_Opcode             char array represents the Opcode of this message.
 * @return      Char Array that represents the final follow message
 */
char* EncoderDecoder::followToMessage(std::string input, char *ch_Opcode) {
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
void EncoderDecoder::insertElementsToFollowInput(char *ch_Opcode, char &yesOrNo, std::vector<std::string> &names,char *output) {
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
        output[index] = this->zeroDelimiter;
        index++;
    }
}

/**
 * Part of the StringToMessage function
 * when the message is identified as a Stat or Post request -->
 * processing the rest of the string by the user as a PM or Stat (according to the OpCode) Message Type.
 * @param input                 String represent the input that was entered by the user.
 * @param ch_Opcode             char array represents the Opcode of this message.
 * @return      Char Array that represents the final PM or Stat message
 */
char* EncoderDecoder::postOrStatToMessage(std::string input, char *ch_Opcode) {
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
    output[index] = &this->zeroDelimiter;
    return *output;
}

/**
 * Part of the StringToMessage function
 * when the message is identified as a PM request --> processing the rest of the string by the user as a PM Message Type.
 * @param input                 String represent the input that was entered by the user.
 * @param ch_Opcode             char array represents the Opcode of this message.
 * @return      Char Array that represents the final PM message
 */
char* EncoderDecoder::pmToMessage(std::string input, char *ch_Opcode) {
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
    output[index] = &this->zeroDelimiter;
    //adding all the content of the message to the output array
    for (char &i : input) {
        output[index] = &i;
        index++;
    }
    //adding the '\0' delimiter
    output[index] = &this->zeroDelimiter;
    return *output;
}

//endregion Encoding Functions

    //region Decoding Functions
/**
* Convert the short number represents the opcode of the message to the message type string
* @param opcode            short number represent the opcode of the message type
* @return      string represents the message type
*/
std::string EncoderDecoder::messageToString(char *messageFromServer) {

    short incomingMessageOpcode = bytesToShort(messageFromServer);
    switch(incomingMessageOpcode){
        case NOTIFICATION:
            return "NOTIFICATION " + notificationToString(messageFromServer);
        case ACK:
            return "ACK " + ackToString(messageFromServer);
        default:
            return "ERROR " + errorToString(messageFromServer);

    }
}

/**
* part of the messageToString Function;
* convert the char array to String notification message to display on the screen to the client
* @param messageFromServer             Char Array that was recieved from the server
* @return              String representation of the message from the server.
*/
std::string EncoderDecoder::notificationToString(char *messageFromServer) {
    std::string output;
    char notificationType = messageFromServer[2];
    if(notificationType == '0'){
        //PM message
        output.append("PM ");
    }
    else{
        //Post Message
        output.append("Public ");
    }
    int index = 3;
    //inserting the posting username
    index = insertCharsToOutput(messageFromServer, output, index);
    output.append(" ");
    //inserting the content of the message
    index = insertCharsToOutput(messageFromServer, output, index);
    return output;
}

/**
* Insert to chars to string until it reaches the delimiter
* @param messageFromServer             Char* recieved by the server
* @param output                        String to add the chars to
* @param index                         current index of the "messageFromServer" array.
* @return
*/
int EncoderDecoder::insertCharsToOutput(char *messageFromServer, string &output, int index) {
    while(messageFromServer[index] != zeroDelimiter){
        output.append(to_string(messageFromServer[index]));
        index++;
    }
    return index;
}

/**
 * part of the messageToString Function;
 * convert the char array to String ack message to display on the screen to the client
 * @param messageFromServer             Char Array that was recieved from the server
 * @return              String representation of the message from the server.
 */
std::string EncoderDecoder::ackToString(char *messageFromServer) {
    short opcode = getOpcodeFromMessageFromServer(messageFromServer);
    switch(opcode){
        case FOLLOW:
            return;
        case USERLIST:
            return;
        case STAT:
            return;
        default:
            //in case it's one of the following:
            // 1.Register
            // 2.Login
            // 3.Logout
            // 4.Post
            // 5.Pm
            return std::to_string(opcode);

    }
}

/**
 * part of the messageToString Function;
 * convert the char array to String error message to display on the screen to the client
 * @param messageFromServer             Char Array that was recieved from the server
 * @return              String representation of the message from the server.
 */
std::string EncoderDecoder::errorToString(char *messageFromServer) {
    short opcode = getOpcodeFromMessageFromServer(messageFromServer);
    //returning the number type of the error
    return std::to_string(opcode);

}
/**
* Part Of the "ackToString" and "errorToString"
* converting from the message tha was received from the server what type of message failed or succeeded.
* @param messageFromServer             Char array that was received from the server.
* @return
*/
short EncoderDecoder::getOpcodeFromMessageFromServer(char *messageFromServer) {
    char* errorTypeArray[2];
    //getting the error type from the array.
    errorTypeArray[0] = &messageFromServer[2];
    errorTypeArray[1] = &messageFromServer[3];
    short opcode = bytesToShort(*errorTypeArray);
    return opcode;
}

//endregion Decoding Functions