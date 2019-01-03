//
// Created by tomer on 02/01/19.
//
#include <boost/algorithm/string.hpp>
#include "IOTask.h"

IOTask::IOTask(ConnectionHandler *connectionHandler) : ch(connectionHandler),isLoggedOut(false) {}

void IOTask::run() {
    while(true) {
        std::string userRequest;
        std::getline(std::cin ,userRequest);
        unsigned long len = userRequest.length();

        if (!ch->sendUserInput(userRequest)) {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            break;
        }
        if ((boost::to_upper_copy<std::string>(userRequest).compare("LOGOUT")) == 0){
            isLoggedOut = true;
        }
        if ((isLoggedOut)){
            break;
        }

    }
}