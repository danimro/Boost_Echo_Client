//
// Created by tomer on 02/01/19.
//
#include "IOTask.h"

IOTask::IOTask(ConnectionHandler *connectionHandler) : ch(connectionHandler),isLoggedOut(false) {}

void IOTask::run() {
    while(true) {
        std::string userRequest;
        std::getline(std::cin ,userRequest);
        std::locale loc;
        unsigned long len = userRequest.length();
        if (!ch->sendLine(userRequest)) {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            break;
        }
        if ((std::toupper(userRequest,loc).compare("LOGOUT")) == 0){
            isLoggedOut = true;
        }
        if ((isLoggedOut) && (len == 0) ){
            break;
        }

    }
}