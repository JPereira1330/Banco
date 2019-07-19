/* 
 * File:   SocketServer.h
 * Author: nyex
 *
 * Created on 16 de julho de 2019, 19:28
 */

#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#include <netdb.h>
using namespace std;

class SocketServer {
public:
    SocketServer();
    SocketServer(const SocketServer& orig);
    virtual ~SocketServer();

    void setPort(unsigned short int port);
    void setQueue(unsigned short int queue);

    int openSocket();
    int acceptSocket();
    
    static int writeSocket(int handler, void *buffer, unsigned int len);
    static int readSocket(int handler, void *buffer, unsigned int len);
    
    void closeSocket();
    static void shutdownSocket(int handler);

private:
    int handler;
    struct sockaddr_in srvAddr;
    unsigned short int queue;
};

#endif /* SOCKETSERVER_H */

