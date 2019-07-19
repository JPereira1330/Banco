/* 
 * File:   ClientProcessor.cpp
 * Author: nyex
 * 
 * Created on 17 de julho de 2019, 20:04
 */
#include <iostream>
#include <thread>
#include <cstring>
#include <iomanip>
#include "ClientProcessor.h"
#include "SocketServer.h"
#include "sys_log.h"

ClientProcessor::ClientProcessor() {
}

ClientProcessor::ClientProcessor(const ClientProcessor& orig) {
}

ClientProcessor::~ClientProcessor() {
}

void ClientProcessor::SetBancoDB(BancoDB* bancoDB) {
    this->bancoDB = bancoDB;
}

void ClientProcessor::SetHandle(int handle) {
    this->handle = handle;
}

int ClientProcessor::getHandle() {
    return this->handle;
}

BancoDB* ClientProcessor::getBanco() {
    return this->bancoDB;
}


int ClientProcessor::start() {
    thread runThread(ClientProcessor::run, this);
    runThread.detach();
    return 1;
}

void ClientProcessor::run(ClientProcessor* cp) {
    
    int len;
    char type;
    char *buffer;
    Msg *msg;
    
    log_write("Esperando a leitura do tamanho: %d", cp->getHandle());
    SocketServer::readSocket(cp->getHandle(), &len, sizeof(unsigned int));

    log_write("Tamanho da mensagem: %d", len);
    SocketServer::readSocket(cp->getHandle(), &type, sizeof(char));
    log_write("Tipo recebido: %c", type);
    
    buffer = (char *) malloc(len);
    SocketServer::readSocket(cp->getHandle(), buffer, len);
    
    log_write("Montando a mensagem");
    msg = new Msg;
    msg->setType(type);
    msg->setBuffer(buffer, len);
    
    switch(type){
        case 'c':
            log_write("Carregando conta");
            loadAcc(msg,cp);
            break;
    }
}

int ClientProcessor::loadAcc(Msg *msg, ClientProcessor *cp) {
   
    int len;
    Conta *acc;
    char *buffer;
    Msg *msgReturn;
    unsigned int numeroConta;
    
    msg->next(&numeroConta);
    log_write("Numero da conta carregado: %d", numeroConta);
    
    acc = cp->getBanco()->searchFile(numeroConta);
    
    if(acc == NULL){
        log_write("Nao foi possivel carregar a conta: %d", numeroConta);
        msgReturn = new Msg();
        msgReturn->setType('c');
        len = msgReturn->getBuffer(&buffer);
        len = SocketServer::writeSocket(cp->getHandle(),buffer, len);
        log_write("Enviando %d Bytes", len);
        return 0;
    }
    
    return 1;
}
