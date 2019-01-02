//
// Created by tomer on 02/01/19.
//

#ifndef BOOST_ECHO_CLIENT_CONNECTIONSERVER_H
#define BOOST_ECHO_CLIENT_CONNECTIONSERVER_H

#include "connectionHandler.h"

class ConnectionServer{
    private:
    ConnectionHandler* ch;
    public:
    ConnectionServer(ConnectionHandler* connectionHandler);
    void run();
};

#endif //BOOST_ECHO_CLIENT_CONNECTIONSERVER_H
