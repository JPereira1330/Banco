/* 
 * File:   SocketClient.h
 * Author: nyex
 *
 * Created on 16 de julho de 2019, 20:14
 */

#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <netdb.h>
#include <string>

using namespace std;

class SocketClient {
public:
    SocketClient();
    SocketClient(const SocketClient& orig);
    virtual ~SocketClient();
    
    void setPort(unsigned short int port);
    void setIP(char *ip);
    
    int connectSocket();
    
    int writeSocket(void *buffer, unsigned int len);
    int readSocket(void *buffer, unsigned int len);
    
    void closeSocket();
    void shutdownSocket();
    
private:
    int handler;
    struct sockaddr_in srvAddr;
    
};

#endif /* SOCKETCLIENT_H */

