//
// Created by tomer on 02/01/19.
//

#ifndef BOOST_ECHO_CLIENT_ENCODERDECODER_H
#define BOOST_ECHO_CLIENT_ENCODERDECODER_H

#include <string>
#include <vector>
#include <map>

/**
 * Enum to identify each Opcode
 */
enum Opcode{
    REGISTER = 1,
    LOGIN = 2,
    LOGOUT = 3,
    FOLLOW = 4,
    POST = 5,
    PM = 6,
    USERLIST = 7,
    STAT = 8,
    NOTIFICATION = 9,
    ACK = 10,
    ERROR = 11
};


class EncoderDecoder{

    public:
        /**
         * Default constructor.
         */
        EncoderDecoder() = default;

       /**
        * Initialising the delimiter, and the values of the Messages's opcodes.
        */
        void init();

        /**
         * default Destructor.
         */
        ~EncoderDecoder() = default;
    private:

        //region Fields

        /**
         * Map of string as keys, and short as values, to connect between type of user request to it's matching OpCode.
         */
        std::map<std::string,short> commandDictionary;
        /**
         * Char of '0' used as delimiter in the communication between the client and the server.
         */
        char zeroDelimiter;

        //endregion Fields

        //region Encoding Functions

        /**
         * Converting a short number to array of chars.
         * @param num               short number to convert.
         * @param bytesArr          Char array to put the number into.
         */
        void shortToBytes(short num, char* bytesArr);

        /**
         * Convert the input string from the client to a char array to send to the server.
         * @param input         String represent the user input.
         * @return              Char* represent a bytes array to send to the server to process.
         */
        char* stringToMessage(std::string input);

        /**
         * Part of the StringToMessage function
         * when the message is identified as a Login or Register request -->
         * processing the rest of the string by the user as a Login or Register (according to the OpCode) Message Type.
         * @param input                 String represent the input that was entered by the user.
         * @param ch_Opcode             char array represents the Opcode of this message.
         * @return      Char Array that represents the final Login or Register message
         */
        char* registerAndLoginToMessage(std::string input, char *ch_Opcode);
        /**
         * Part of the StringToMessage function
         * when the message is identified as a follow request --> processing the rest of the string by the user as a Follow Message Type.
         * @param input                 String represent the input that was entered by the user.
         * @param ch_Opcode             char array represents the Opcode of this message.
         * @return      Char Array that represents the final follow message
         */
        char* followToMessage(std::string input, char *ch_Opcode);
        /**
         * Part of the FollowToMessage
         * combining all the char array that represents a Follow message,
         * and building one array that include all the information needed by the server.
         * @param ch_Opcode             Char array represent the first two bytes that are the Opcode of the message.
         * @param yesOrNo               Char that represent whether the the user want to follow or unfollow the people in the list.
         * @param names                 Vector of strings represent all the UserNames the current client want to follow or unfollow
         * @param output                Char array to bput all the given information in, in the correct order
         */
        void insertElementsToFollowInput(char *ch_Opcode, char &yesOrNo, std::vector<std::string> &names,
                                         char* output);
        /**
         * Part of the StringToMessage function
         * when the message is identified as a Stat or Post request -->
         * processing the rest of the string by the user as a PM or Stat (according to the OpCode) Message Type.
         * @param input                 String represent the input that was entered by the user.
         * @param ch_Opcode             char array represents the Opcode of this message.
         * @return      Char Array that represents the final PM or Stat message
         */
        char* postOrStatToMessage(std::string input, char *ch_Opcode);
        /**
         * Part of the StringToMessage function
         * when the message is identified as a PM request --> processing the rest of the string by the user as a PM Message Type.
         * @param input                 String represent the input that was entered by the user.
         * @param ch_Opcode             char array represents the Opcode of this message.
         * @return      Char Array that represents the final PM message
         */
        char* pmToMessage(std::string input, char *ch_Opcode);

        //endregion Encoding Functions

        //region Decoding Functions

        /**
        * Convert the short number represents the opcode of the message to the message type string.
        * @param opcode            short number represent the opcode of the message type.
        * @return      string represents the message type.
        */
        std::string messageToString(char* messageFromServer);

        /**
         * part of the messageToString Function.
         * convert the char array to String notification message to display on the screen to the client.
         * @param messageFromServer             Char Array that was recieved from the server.
         * @return              String representation of the message from the server.
         */
        std::string notificationToString(char* messageFromServer);

        /**
         * part of the messageToString Function.
         * convert the char array to String ack message to display on the screen to the client.
         * @param messageFromServer             Char Array that was recieved from the server.
         * @return              String representation of the message from the server.
         */
        std::string ackToString(char* messageFromServer);
        /**
         * part of the messageToString Function.
         * convert the char array to String error message to display on the screen to the client.
         * @param messageFromServer             Char Array that was recieved from the server.
         * @return              String representation of the message from the server.
         */
        std::string errorToString(char* messageFromServer);

        /**
         * Insert to chars to string until it reaches the delimiter.
         * @param messageFromServer             Char* recieved by the server.
         * @param output                        String to add the chars to.
         * @param index                         current index of the "messageFromServer" array.
         * @return                              Integer represents the current free index of the given output array.
         */
        int insertCharsToOutput(char *messageFromServer, std::string &output, int index);


        /**
         * Part of the "ackToString" functions.
         * translating all the information in the given array from the server as it was an Follow Ack Message.
         * @param messageFromServer             Char array that was received from the server.
         * @return      String representation of the given array as a follow Ack message.
         */
        std::string followOrUserListAckToString(char *messageFromServer);

        /**
         * Part of the "ackToString" functions.
         * translating all the information in the given array from the server as it was an stat Ack Message.
         * @param messageFromServer             Char array that was received from the server.
         * @return      String representation of the given array as a stat Ack message.
         */
        std::string statAckToString(char *messageFromServer);


        /**
         * Taking the first two chars from the given index from the given char array and convert them to short number.
         * @param input                 char array to take the number from
         * @param startIndex            integer index to start taking the number from
         * @return                      Short number that was taken from the first two bytes from the given array.
         */
         short gettingShortFromCharArray(char* input,int startIndex);

         /**
         * Converting Char array to a Short number.
         * @param bytesArr              Char array to convert.
         * @return        Short number that is the bytes value of what was in the bytes array.
         */
         short bytesToShort(char* bytesArr);

        //endregion Decoding Functions
};



#endif //BOOST_ECHO_CLIENT_ENCODERDECODER_H
