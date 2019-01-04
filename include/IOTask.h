//
// Created by tomer on 02/01/19.
//

#ifndef BOOST_ECHO_CLIENT_IOTASK_H
#define BOOST_ECHO_CLIENT_IOTASK_H

#include "connectionHandler.h"

class IOTask {
private:
    ConnectionHandler *ch;
public:
    IOTask(ConnectionHandler *connectionHandler);
    void run();
};

#endif //BOOST_ECHO_CLIENT_IOTASK_H
