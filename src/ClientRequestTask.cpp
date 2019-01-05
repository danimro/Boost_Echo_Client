//
// Created by tomergon@wincs.cs.bgu.ac.il on 1/5/19.
//

#include <boost/algorithm/string.hpp>
#include "ClientRequestTask.h"

ClientRequestTask::ClientRequestTask(ConnectionHandler *connectionHandler) : ch(connectionHandler){}

void ClientRequestTask::run() {
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