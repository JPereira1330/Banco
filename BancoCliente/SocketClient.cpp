/* 
 * File:   SocketClient.cpp
 * Author: nyex
 * 
 * Created on 16 de julho de 2019, 20:14
 */

#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "SocketClient.h"

SocketClient::SocketClient() {
    this->handler = socket(AF_INET, SOCK_STREAM, 0);
    
    memset(&srvAddr,0,sizeof(struct sockaddr_in));
    this->srvAddr.sin_family = AF_INET;
}

SocketClient::SocketClient(const SocketClient& orig) {
}

SocketClient::~SocketClient() {
}

void SocketClient::setIP(char *ip) {
    this->srvAddr.sin_addr.s_addr = inet_addr(ip);
}

void SocketClient::setPort(unsigned short int port) {
    this->srvAddr.sin_port = htons(port);
}

int SocketClient::connectSocket() {

    if(this->handler <= 0){
        return 0;
    }
    
    if(connect(this->handler, (struct sockaddr *) &srvAddr, sizeof(srvAddr)) < 0){
        return 0;
    }
    
    return 1; 
}

int SocketClient::readSocket(void* buffer, unsigned int len) {
    int bytes, totalBytes;
    char *bufferRecv;
    
    bytes = 0;
    totalBytes = 0;
    bufferRecv = (char *) buffer; 
    
    do{
        bytes = recv(this->handler, &bufferRecv[totalBytes], len-totalBytes, 0);
        if(bytes > 0){
            totalBytes += bytes;
        }
    } while (bytes > 0 && totalBytes < len);
    
    if(bytes < 0){
        return bytes;
    }
    
    return totalBytes;
}

int SocketClient::writeSocket(void* buffer, unsigned int len) {
    int bytes, totalBytes;
    char*bufferSend;
    
    bytes = 0;
    totalBytes = 0;
    bufferSend = (char *) buffer; 
    
    do{
        bytes = send(this->handler, &bufferSend[totalBytes], len-totalBytes, 0);
        if(bytes > 0){
            totalBytes += bytes;
        }
    } while (bytes > 0 && totalBytes < len);
    
    if(bytes < 0){
        return bytes;
    }
    
    return totalBytes;
}

void SocketClient::closeSocket() {
    close(this->handler);
}

void SocketClient::shutdownSocket() {
    shutdown(this->handler, SHUT_RDWR);
}
