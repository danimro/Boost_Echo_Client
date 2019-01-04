//
// Created by tomer on 02/01/19.
//
#include <boost/algorithm/string.hpp>
#include <thread>
#include "IOTask.h"

IOTask::IOTask(ConnectionHandler *connectionHandler) : ch(connectionHandler){}

void IOTask::run() {
    while(true) {
        std::string userRequest;
        std::getline(std::cin ,userRequest);

        if (!ch->sendUserInput(userRequest)) {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            break;
        }
        if ((boost::to_upper_copy<std::string>(userRequest).compare("LOGOUT")) == 0){
            while (this->ch->getLogoutStatus() == PENDING){
                //busy wait for logout result
            }
            if(this->ch->getLogoutStatus() == PROCEED){
                this->ch->setLogoutStatus(PENDING);
            }
            else{
                break;
            }
        }
    }
}