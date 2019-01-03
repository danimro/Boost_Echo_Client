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



/*class ConnectionServer{
private:
    ConnectionHandler* ch;
public:
    ConnectionServer(ConnectionHandler* connectionHandler):ch(connectionHandler){}
    void run(){
        while(true){
            std::string answer;
            if (!ch->getLine(answer)) {
                std::cout << "Disconnected. Exiting...\n" << std::endl;
                break;
            }

            unsigned long len=answer.length();

            answer.resize(len-1);
            std::cout << answer << std::endl;
            if (answer.compare("ACK 3") == 0) {
                std::cout << "Ready to exit. Press enter\n" << std::endl;
                break;
            }
        }
    };
};

class IOTask {
private:
    ConnectionHandler *ch;
    bool isLoggedOut;
public:
    IOTask(ConnectionHandler *connectionHandler) : ch(connectionHandler),isLoggedOut(false) {}

    void run() {
        while(true) {
            string userRequest;
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
    };
};*/

