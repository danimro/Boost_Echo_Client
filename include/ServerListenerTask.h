//
// Created by tomergon@wincs.cs.bgu.ac.il on 1/5/19.
//

#ifndef BOOST_ECHO_CLIENT_SERVERLISTENERTASK_H
#define BOOST_ECHO_CLIENT_SERVERLISTENERTASK_H

#include "ConnectionHandler.h"

class ServerListenerTask{
private:
    ConnectionHandler* ch;
public:
    ServerListenerTask(ConnectionHandler* connectionHandler);
    void run();
};

#endif //BOOST_ECHO_CLIENT_SERVERLISTENERTASK_H
