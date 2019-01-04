//
// Created by tomer on 02/01/19.
//

#include "ConnectionServer.h"

ConnectionServer::ConnectionServer(ConnectionHandler* connectionHandler):ch(connectionHandler){}

void ConnectionServer::run() {
    while(true){
        std::string answer = ch->translateMessage();
        std::cout << answer << std::endl;
        if (answer == "ACK 3") {
            this->ch->setLogoutStatus(TERMINATE);
            break;
        }
        else if(answer == "ERROR 3"){
            this->ch->setLogoutStatus(PROCEED);
        }

    }
}