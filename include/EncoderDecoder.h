//
// Created by tomer on 02/01/19.
//

#ifndef BOOST_ECHO_CLIENT_ENCODERDECODER_H
#define BOOST_ECHO_CLIENT_ENCODERDECODER_H

#include <string>
#include <vector>
#include <map>

class EncoderDecoder{
    public:
        /**
         * Default constructor
         */
        EncoderDecoder() = default;
        
        /**
        * Initialising the delimiter, and the values of the Messages's opcodes
        */
        void init();

        /**
         * Converting Char array to a Short number
         * @param bytesArr              Char array to convert
         * @return        Short number that is the bytes value of what was in the bytes array
         */
        short bytesToShort(char* bytesArr);

        /**
         * Converting a short number to array of chars
         * @param num               short number to convert
         * @param bytesArr          Char array to put the number into
         */
        void shortToBytes(short num, char* bytesArr);
        /**
         * Convert the input string from the client to a char array to send to the server.
         * @param input         String represent the user input
         * @return              Char* represent a bytes array to send to the server to process
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
    private:
        /**
         * Map of string as keys, and short as values, to connect betwwen type of user request to it's matching OpCode
         */
        std::map<std::string,short> commandDictionary;
        /**
         * Char of '0' used as delimiter in the communication between the client and the server
         */
        char zeroDelimiter;

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

        /**
         * default Destructor
         */
        ~EncoderDecoder() = default;
};



#endif //BOOST_ECHO_CLIENT_ENCODERDECODER_H
