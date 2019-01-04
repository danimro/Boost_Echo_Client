//
// Created by tomer on 01/01/19.
//

#include <stdlib.h>
#include <iostream>
#include <thread>
#include <boost/thread.hpp>
#include "connectionHandler.h"
#include <boost/algorithm/string.hpp>
#include <IOTask.h>
#include <ConnectionServer.h>

using namespace boost;
using namespace std;
using boost::asio::ip::tcp;

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;



int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
        return -1;
    }
    std::string host = argv[1];
    auto port = (short)atoi(argv[2]);

    ConnectionHandler connectionHandler(host, port);
    if (!connectionHandler.connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }
    IOTask IO(&connectionHandler);
    std::thread IOThread(&IOTask::run,&IO);
    ConnectionServer Com(&connectionHandler);
    std::thread ComThread(&ConnectionServer::run, &Com);

    IOThread.join();
    ComThread.join();
    return 0;
}