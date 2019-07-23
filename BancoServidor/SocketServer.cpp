/* 
 * File:   SocketServer.cpp
 * Author: nyex
 * 
 * Created on 16 de julho de 2019, 19:28
 */

#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <list>
#include <stdio.h>

#include "SocketServer.h"

SocketServer::SocketServer() {
    this->handler = socket(AF_INET, SOCK_STREAM, 0);
    
    memset(&srvAddr,0,sizeof(struct sockaddr_in));
    this->srvAddr.sin_family = AF_INET;
    this->srvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
}

SocketServer::SocketServer(const SocketServer& orig) {
}

SocketServer::~SocketServer() {
}

void SocketServer::setPort(unsigned short int port) {
    this->srvAddr.sin_port = htons(port);
}

void SocketServer::setQueue(unsigned short int queue) {
    this->queue = queue;
}

int SocketServer::openSocket() {
    
    if(this->handler <= 0){
        return 0;
    }
    
    if(bind(handler, (struct sockaddr*) &srvAddr, sizeof(srvAddr)) < 0){
        return 0;
    }
    
    if( listen(handler, this->queue) < 0){
        return 0;
    }
    
    return 1; 
}

int SocketServer::acceptSocket() {
    int tamSockAddr = sizeof(this->srvAddr);
    return accept(this->handler, (struct sockaddr*) &srvAddr, (socklen_t*) &tamSockAddr);
}

void SocketServer::closeSocket() {
    close(this->handler);
}

int SocketServer::readSocket(int handler, void* buffer, unsigned int len) {
    int bytes, totalBytes;
    char *bufferRecv;
    
    bytes = 0;
    totalBytes = 0;
    bufferRecv = (char *) buffer; 
    
    do{
        bytes = recv(handler, &bufferRecv[totalBytes], len-totalBytes, 0);
        if(bytes > 0){
            totalBytes += bytes;
        }
    } while (bytes > 0 && totalBytes < (int) len);
    
    if(bytes <= 0){
        return bytes;
    }
    return totalBytes;
}

int SocketServer::writeSocket(int handler, void* buffer, unsigned int len) {
    int bytes, totalBytes;
    char*bufferSend;
    
    bytes = 0;
    totalBytes = 0;
    bufferSend = (char *) buffer; 
    
    do{
        bytes = send(handler, &bufferSend[totalBytes], len-totalBytes, 0);
        if(bytes > 0){
            totalBytes += bytes;
        }
    } while (bytes > 0 && totalBytes < (int) len);
    
    if(bytes < 0){
        return bytes;
    }
    
    return totalBytes;
}

void SocketServer::shutdownSocket(int handler) {
    shutdown(handler, SHUT_RDWR);
}
