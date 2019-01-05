//
// Created by tomergon@wincs.cs.bgu.ac.il on 1/5/19.
//

#ifndef BOOST_ECHO_CLIENT_CLIENTREQUESTTASK_H
#define BOOST_ECHO_CLIENT_CLIENTREQUESTTASK_H

#include "ConnectionHandler.h"

class ClientRequestTask {
private:
    ConnectionHandler *ch;
public:
    ClientRequestTask(ConnectionHandler *connectionHandler);
    void run();
};

#endif //BOOST_ECHO_CLIENT_CLIENTREQUESTTASK_H
