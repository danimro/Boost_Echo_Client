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
            //if the log out was successful --> tell the IO thread to terminate
            this->ch->setLogoutStatus(TERMINATE);
            break;
        }
        else if(answer == "ERROR 3"){
            //if the logout didn't succeeded --> tell the IO thread to continue working
            this->ch->setLogoutStatus(PROCEED);
        }
    }
}