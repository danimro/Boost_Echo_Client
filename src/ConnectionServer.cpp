//
// Created by tomer on 02/01/19.
//

#include "ConnectionServer.h"

ConnectionServer::ConnectionServer(ConnectionHandler* connectionHandler):ch(connectionHandler){}

void ConnectionServer::run() {
    while(true){
        std::string answer = ch->translateMessage();
        /*if (!ch->getFrameAscii(answer,'\0')) {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            break;
        }*/

        unsigned long len = answer.length();

        std::cout << answer << std::endl;
        if (answer == "ACK 3") {
            break;
        }
    }
}