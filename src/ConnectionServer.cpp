//
// Created by tomer on 02/01/19.
//

#include "ConnectionServer.h"

ConnectionServer::ConnectionServer(ConnectionHandler* connectionHandler):ch(connectionHandler){}

void ConnectionServer::run() {
    while(true){
        std::string answer;
        if (!ch->getLine(answer)) {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            break;
        }

        unsigned long len=answer.length();

        answer.resize(len-1);
        std::cout << answer << std::endl;
        if (answer == "ACK 3") {
            std::cout << "Ready to exit. Press enter\n" << std::endl;
            break;
        }
    }
}