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
    log_write("Start - Carregando save.");
    bancoDB->loadFile();
    
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
        log_write("Run [Handle: %d] - Aguardando comunicacao do cliente.", cp->getHandle());
        ret = SocketServer::readSocket(cp->getHandle(), &len, sizeof(unsigned int));
        log_write("Run [Handle: %d] - Recebido: %d bytes.", cp->getHandle(), ret);
                
        if(ret <= 0){
            log_write("Run [Handle: %d] - Cliente desconectou.", cp->getHandle());
            SocketServer::shutdownSocket(cp->getHandle());
            return;
        }   

        ret = SocketServer::readSocket(cp->getHandle(), &type, sizeof(char));
        log_write("Run [Handle: %d] - Recebido: %d bytes.", cp->getHandle(), ret);
        log_write("Run [Handle: %d] - Tipo de mensagem: %c.", cp->getHandle(), type);
        
        if(ret <= 0){
            log_write("Run [Handle: %d] - Cliente desconectou.", cp->getHandle());
            SocketServer::shutdownSocket(cp->getHandle());
            return;
        }
        
        log_write("Run [Handle: %d] - Criando pacote.", cp->getHandle());
        msg = new Msg;
        msg->setType(type);
        
        if(len > 0){
            buffer = (char *) malloc(len);
            ret = SocketServer::readSocket(cp->getHandle(), buffer, len);
            log_write("Run [Handle: %d] - Tamanho recebido: %d", cp->getHandle(), ret);
            
            if(ret <= 0){
                log_write("Run [Handle: %d] - Cliente desconectou.", cp->getHandle());
                SocketServer::shutdownSocket(cp->getHandle());
                delete(msg);
                return;
            }
            
            msg->setBuffer(buffer, len);
        }
        
        switch(type){
            case 'C':
                if (cp->GetConta() != NULL){
                    log_write("Run [Handle: %d] - Conta já carregada: %d.", cp->getHandle(),cp->conta->getNumeroConta());
                }else{
                    log_write("Run [Handle: %d] - Carregando conta.", cp->getHandle());
                    cp->SetConta(loadAcc(msg,cp));
                }
                break;
                
            case 'N':
                if (cp->GetConta() != NULL){
                    log_write("Run [Handle: %d] - Conta já carregada: %d.", cp->getHandle(),cp->conta->getNumeroConta());
                }else{
                    log_write("Run [Handle: %d] - Criando Conta.", cp->getHandle());
                    cp->SetConta(saveAcc(msg,cp));
                }
                break;
                
            case 'E':
                if (cp->GetConta() != NULL){
                    log_write("Run [Handle: %d] - Carregando saldo.", cp->getHandle());
                    saldoAcc(cp,cp->GetConta());
                }else{
                    log_write("Run [Handle: %d] - Conta nao carregada.", cp->getHandle());
                }
                break;
                
            case 'D':
                if (cp->GetConta() != NULL){
                    log_write("Run [Handle: %d] - Deposito.", cp->getHandle());
                    depositaAcc(msg,cp,cp->GetConta());
                }else{
                    log_write("Run [Handle: %d] - Conta nao carregada.", cp->getHandle());
                }
                break;
                
            case 'S':
                if (cp->GetConta() != NULL){
                    log_write("Run [Handle: %d] - Saque.", cp->getHandle());
                    saqueAcc(msg,cp,cp->GetConta());
                }else{
                    log_write("Run [Handle: %d] - Conta nao carregada.", cp->getHandle());
                }
                break;
            case 'T':
                if(cp->GetConta() != NULL){
                    log_write("Run [Handle: %d] - Transferencia.", cp->getHandle());
                    transferenciaAcc(msg,cp,cp->GetConta());
                }else{
                    log_write("Run [Handle: %d] - Conta nao carregada.", cp->getHandle());
                }
                break;
            case 'X':
                log_write("Run [Handle: %d] - Solicitado desligamento do servidor.", cp->getHandle());
                ret = cp->getBanco()->saveFile();
                if(ret == 1){
                    log_write("Run [Handle: %d] - Desligando.", cp->getHandle());
                    exit(1);
                }
                log_write("Run [Handle: %d] - Nao foi possivel salvar os arquivos. %d", cp->getHandle(), ret);
                log_write("Run [Handle: %d] - Desligamento encerrado.", cp->getHandle());
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
    log_write("LoadAcc [Handle: %d] - Conta recebida: %d", cp->getHandle(), numeroConta);
    
    log_write("LoadAcc [Handle: %d] - Procurando por conta: %d", cp->getHandle(), numeroConta);
    conta = cp->getBanco()->searchFile(numeroConta);
    
    if(conta == NULL){
        log_write("LoadAcc [Handle: %d] - Conta não encontrada: %d", cp->getHandle(), numeroConta);
        msgReturn = new Msg();
        msgReturn->setType('c');
        len = msgReturn->getBuffer(&buffer);
        len = SocketServer::writeSocket(cp->getHandle(),buffer, len);
        log_write("LoadAcc [Handle: %d] - Dados enviados: %d", cp->getHandle(), len);
        return NULL;
    }
    
    log_write("LoadAcc [Handle: %d] - Conta encontrada: %d", cp->getHandle(), numeroConta);
    
    log_write("LoadAcc [Handle: %d] - Criando pacote.", cp->getHandle());
    msgReturn = new Msg();
    msgReturn->setType('C');
    msgReturn->add(conta->getNumeroConta());
    msgReturn->add( (char *) conta->getTitularConta().c_str(), conta->getTitularConta().size());
    
    len = msgReturn->getBuffer(&buffer);
    log_write("LoadAcc [Handle: %d] - Tamanho do buffer: %d bytes.", cp->getHandle(), len);
    
    len = SocketServer::writeSocket(cp->getHandle(),buffer, len);
    log_write("LoadAcc [Handle: %d] - Dados enviados: %d bytes.", cp->getHandle(), len);
    
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
    log_write("SaveAcc [Handle: %d] - Conta carregada: %d bytes.", cp->getHandle(), numeroConta);
    
    len = msg->next(&titular);
    strTitular = string(titular,len);
    log_write("SaveAcc [Handle: %d] - Titular da conta carregado: %s", cp->getHandle(), strTitular.c_str());
    
    msg->next(&saldo);
    log_write("SaveAcc [Handle: %d] - Saldo inserido: %d", cp->getHandle(), saldo);
    
    conta = new Conta();
        
    conta->setNumeroConta(numeroConta);

    conta->setTitularConta(strTitular);

    conta->setSaldoDisponivel(saldo);
    
    cp->getBanco()->add(conta);
    
    log_write("SaveAcc [Handle: %d] - Conta %d salva no sistema", cp->getHandle(), conta->getNumeroConta());
    msgReturn = new Msg();
    msgReturn->setType('N');
    len = msgReturn->getBuffer(&buffer);
    len = SocketServer::writeSocket(cp->getHandle(),buffer, len);
    log_write("SaveAcc [Handle: %d] - Enviando %d Bytes", cp->getHandle(), len);
    delete(msgReturn);
    
    return conta;
}

int ClientProcessor::depositaAcc(Msg *msg, ClientProcessor *cp, Conta  *conta) {
    int ret;
    int len;
    int valor;
    char *buffer;
    Msg *msgReturn;
    
    msg->next(&valor);
    log_write("Valor carregado: %d", valor);
    
    msgReturn = new Msg();
    ret = conta->deposito(valor);
    
    if(ret == 0){
        log_write("Deposito não foi efetuado na conta %d no valor de %d", conta->getNumeroConta(), valor);
        conta->rollback();
        msgReturn->setType('d');
        len = msgReturn->getBuffer(&buffer);
        len = SocketServer::writeSocket(cp->getHandle(),buffer, len);
        log_write("Enviando %d Bytes", len);
        
        return 0;
    }
    
    conta->save();
    
    log_write("Deposito efetuado na conta %d no valor de %d", conta->getNumeroConta(), valor);
    msgReturn->setType('D');
    len = msgReturn->getBuffer(&buffer);
    len = SocketServer::writeSocket(cp->getHandle(),buffer, len);
    log_write("Enviando %d Bytes", len);
    
    delete(msgReturn);
    
    return 1;
}

int ClientProcessor::saqueAcc(Msg *msg, ClientProcessor *cp, Conta  *conta) {
    int ret;
    int len;
    int valor;
    char *buffer;
    Msg *msgReturn;
    
    msg->next(&valor);
    log_write("Saque [Handle: %d] - Valor carregado: %d.", cp->getHandle(),valor);
    
    msgReturn = new Msg();
    ret = conta->sacar(valor);
    
    if(ret == 0){
        log_write("Saque [Handle: %d] - Não foi possivel realizar o saque.", cp->getHandle());
        conta->rollback();
        msgReturn->setType('d');
        len = msgReturn->getBuffer(&buffer);
        len = SocketServer::writeSocket(cp->getHandle(),buffer, len);
        log_write("Saque [Handle: %d] - Enviando: %d Bytes.", cp->getHandle(), len);
        
        return 0;
    }
    
    conta->save();
    
    log_write("Saque [Handle: %d] - Saque realizado com sucesso.", cp->getHandle());
    msgReturn->setType('D');
    len = msgReturn->getBuffer(&buffer);
    len = SocketServer::writeSocket(cp->getHandle(),buffer, len);
    log_write("Saque [Handle: %d] - Enviando: %d Bytes.", cp->getHandle(), len);
    
    delete(msgReturn);
    
    return 1;
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

int ClientProcessor::transferenciaAcc(Msg *msg, ClientProcessor *cp, Conta *conta){
    int destino;
    int valor;
    int ret;
    int len;
    char *buffer;
    Msg *msgReturn;
    Conta *dest;
    
    msgReturn = new Msg();
    
    msg->next(&destino);
    log_write("Transferencia [Handle: %d] - Conta destino: %d.", cp->getHandle(),destino);
    
    msg->next(&valor);
    log_write("Transferencia [Handle: %d] - Valor a ser transferido: %d.", cp->getHandle(),valor);
    
    dest = new Conta();
    dest = cp->getBanco()->searchFile(destino);
    
    if(dest == NULL){
        log_write("Transferencia [Handle: %d] - Conta %d não encontrada.", cp->getHandle(),destino);
        msgReturn->setType('t');
        len = msgReturn->getBuffer(&buffer);
        len = SocketServer::writeSocket(cp->getHandle(),buffer, len);
        log_write("Transferencia [Handle: %d] - Enviando: %d Bytes.", cp->getHandle(), len);
        delete(msgReturn);
        return 0;
    }
    
    log_write("Transferencia [Handle: %d] - Conta %d encontrada.", cp->getHandle(),destino);
    
    if(conta->getNumeroConta() == dest->getNumeroConta()){
        log_write("Transferencia [Handle: %d] - Transferencia para a mesma conta.", cp->getHandle(),destino);
        msgReturn->setType('t');
        len = msgReturn->getBuffer(&buffer);
        len = SocketServer::writeSocket(cp->getHandle(),buffer, len);
        log_write("Transferencia [Handle: %d] - Enviando: %d Bytes.", cp->getHandle(), len);
        delete(msgReturn);
        return 0;
    }
    
    
    log_write("Transferencia [Handle: %d] - Saque da conta origem: %d.", cp->getHandle(), valor);
    ret = conta->sacar(valor);

    if(ret != 1){
        log_write("Transferencia [Handle: %d] - Nao foi possivel sacar o dinheiro.", cp->getHandle());
        conta->rollback();
        msgReturn->setType('t');
        len = msgReturn->getBuffer(&buffer);
        len = SocketServer::writeSocket(cp->getHandle(),buffer, len);
        log_write("Saque [Handle: %d] - Enviando: %d Bytes.", cp->getHandle(), len);
        delete(msgReturn);
        return 0;
    }
    
    log_write("Transferencia [Handle: %d] - Deposito da conta destino: %d.", cp->getHandle(), valor);
    ret = dest->deposito(valor);

    if(ret != 1){
        log_write("Transferencia [Handle: %d] - Nao foi possivel depositar o dinheiro.", cp->getHandle());
        dest->rollback();
        conta->rollback();
        msgReturn->setType('t');
        len = msgReturn->getBuffer(&buffer);
        len = SocketServer::writeSocket(cp->getHandle(),buffer, len);
        log_write("Saque [Handle: %d] - Enviando: %d Bytes.", cp->getHandle(), len);
        delete(msgReturn);
        return 0;
    }

    conta->save();
    dest->save();
    log_write("Transferencia [Handle: %d] - Salvando alteracoes.", cp->getHandle());
    
    msgReturn->setType('T');
    len = msgReturn->getBuffer(&buffer);
    len = SocketServer::writeSocket(cp->getHandle(),buffer, len);
    log_write("Transferencia [Handle: %d] - Enviando: %d Bytes.", cp->getHandle(), len);
    
    delete(msgReturn);
    return 1;
}
