//
// Created by tomergon@wincs.cs.bgu.ac.il on 1/5/19.
//


#include "ServerListenerTask.h"

ServerListenerTask::ServerListenerTask(ConnectionHandler* connectionHandler):ch(connectionHandler){}

void ServerListenerTask::run() {
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