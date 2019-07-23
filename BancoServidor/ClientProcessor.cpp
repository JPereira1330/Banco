/* 
 * File:   ClientProcessor.cpp
 * Author: nyex
 * 
 * Created on 17 de julho de 2019, 20:04
 */
#include <iostream>
#include <thread>
#include <cstring>
#include <stdlib.h>
#include <string>
#include <iomanip>
#include "ClientProcessor.h"
#include "SocketServer.h"
#include "sys_log.h"

ClientProcessor::ClientProcessor() {
    conta = NULL;
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

void ClientProcessor::SetConta(Conta* conta) {
    this->conta = conta;
}


int ClientProcessor::getHandle() {
    return this->handle;
}

BancoDB* ClientProcessor::getBanco() {
    return this->bancoDB;
}

Conta* ClientProcessor::GetConta() {
    return conta;
}



int ClientProcessor::start() {
    thread runThread(ClientProcessor::run, this);
    runThread.detach();
    return 1;
}

void ClientProcessor::run(ClientProcessor* cp) {
    
    int len;
    int ret;
    char type;
    char *buffer;
    Msg *msg;

    
    do{
        len = 0;
        log_write("Handle da leitura: %d", cp->getHandle());
        ret = SocketServer::readSocket(cp->getHandle(), &len, sizeof(unsigned int));

        if(ret <= 0){
            log_write("Cliente desconectou: %d", cp->getHandle());
            SocketServer::shutdownSocket(cp->getHandle());
            return;
        }   

        log_write("Tamanho da mensagem: %d", len);
        ret = SocketServer::readSocket(cp->getHandle(), &type, sizeof(char));

        if(ret <= 0){
            log_write("Cliente desconectou: %d", cp->getHandle());
            SocketServer::shutdownSocket(cp->getHandle());
            return;
        }

        log_write("Tipo recebido: %c", type);
        
        msg = new Msg;
        msg->setType(type);
        
        if(len > 0){
            buffer = (char *) malloc(len);
            ret = SocketServer::readSocket(cp->getHandle(), buffer, len);

            if(ret <= 0){
                log_write("Cliente desconectou: %d", cp->getHandle());
                SocketServer::shutdownSocket(cp->getHandle());
                delete(msg);
                return;
            }
            
            msg->setBuffer(buffer, len);
        }
        
        switch(type){
            case 'C':
                if (cp->GetConta() != NULL){
                    log_write("Conta ja estava carregada %d", cp->conta->getNumeroConta());
                }else{
                    log_write("Carregando conta");
                    cp->SetConta(loadAcc(msg,cp));
                }
                
                break;
            case 'N':
                if (cp->GetConta() != NULL){
                    log_write("Conta ja estava carregada %d", cp->conta->getNumeroConta());
                }else{
                    log_write("Criando conta");
                    cp->SetConta(saveAcc(msg,cp));
                }
                
                break;
            case 'E':
                if (cp->GetConta() != NULL){
                    log_write("Tirando saldo");
                    saldoAcc(cp,cp->GetConta());
                }else{
                    log_write("Conta nao criada em memoria");
                }
                break;
        }
        
        delete(msg);
    }while(1);
}

Conta *ClientProcessor::loadAcc(Msg *msg, ClientProcessor *cp) {
   
    int len;
    char *buffer;
    Msg *msgReturn;
    unsigned int numeroConta;
    Conta *conta;
    
    msg->next(&numeroConta);
    log_write("Numero da conta carregado: %d", numeroConta);
    
    conta = cp->getBanco()->searchFile(numeroConta);
    
    if(conta == NULL){
        log_write("Nao foi possivel carregar a conta: %d", numeroConta);
        msgReturn = new Msg();
        msgReturn->setType('c');
        len = msgReturn->getBuffer(&buffer);
        len = SocketServer::writeSocket(cp->getHandle(),buffer, len);
        log_write("Enviando %d Bytes", len);
        return NULL;
    }
    
    log_write("Conta encontrada: %d", numeroConta);
    
    // Preparando pacote para enviar para o cliente
    msgReturn = new Msg();
    msgReturn->setType('C');
    msgReturn->add(conta->getNumeroConta());
    msgReturn->add( (char *) conta->getTitularConta().c_str(), conta->getTitularConta().size());
    
    len = msgReturn->getBuffer(&buffer);
    len = SocketServer::writeSocket(cp->getHandle(),buffer, len);
    
    log_write("Enviando %d Bytes", len);
    
    return conta;
}

Conta *ClientProcessor::saveAcc(Msg *msg, ClientProcessor *cp) {
   
    int len;
    int saldo;
    char *buffer;
    Msg *msgReturn;
    unsigned int numeroConta;
    char *titular;
    string strTitular;
    Conta *conta;
    
    msg->next(&numeroConta);
    log_write("Numero da conta carregado: %d", numeroConta);
    
    len = msg->next(&titular);
    strTitular = string(titular,len);
    log_write("Titular da conta carregado: %s", strTitular.c_str());
    
    msg->next(&saldo);
    log_write("Saldo inserido: %d", saldo);
    
    conta = new Conta();
        
    conta->setNumeroConta(numeroConta);
        log_write("1");
    conta->setTitularConta(strTitular);
        log_write("2");
    conta->setSaldoDisponivel(saldo);
    
    log_write("antes add");
    cp->getBanco()->add(conta);
    
    log_write("Conta %d salva no sistema", conta->getNumeroConta());
    msgReturn = new Msg();
    msgReturn->setType('N');
    len = msgReturn->getBuffer(&buffer);
    len = SocketServer::writeSocket(cp->getHandle(),buffer, len);
    log_write("Enviando %d Bytes", len);
    delete(msgReturn);
    
    return conta;
}

int ClientProcessor::depositaAcc(Msg *msg, ClientProcessor *cp, Conta  *conta) {
   
    int len;
    char *buffer;
    Msg *msgReturn;
    int valor;
    
    msg->next(&valor);
    log_write("Valor carregado: %d", valor);

    /*
     * ARRUMAR
     */
    cp->getBanco()->searchFile(conta->getNumeroConta())->setSaldoDisponivel(valor);
    
    log_write("Deposito efetuado na conta %d no valor de %d", conta->getNumeroConta(), valor);
    msgReturn = new Msg();
    msgReturn->setType('D');
    len = msgReturn->getBuffer(&buffer);
    len = SocketServer::writeSocket(cp->getHandle(),buffer, len);
    log_write("Enviando %d Bytes", len);
    
    delete(msgReturn);
    
    return 0;
}

int ClientProcessor::saldoAcc(ClientProcessor* cp, Conta *conta) {
    
    int len;
    int saldo;
    Msg *msg;
    char *buffer;
    
    saldo = conta->getSaldoDisponivel();
    
    msg = new Msg();
    msg->setType('E');
    msg->add(saldo);
    
    len = msg->getBuffer(&buffer);
    len = SocketServer::writeSocket(cp->getHandle(), buffer, len);
    log_write("Enviando %d Bytes", len);
    
    delete(msg);
    
    return 1;
}



