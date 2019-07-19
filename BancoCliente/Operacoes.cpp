
/* 
 * File:   Operacoes.cpp
 * Author: nyex
 * 
 * Created on 18 de julho de 2019, 20:27
 */

#include <cstring>

#include "Operacoes.h"
#include "Msg.h"
#include "SocketClient.h"
#include "sys_log.h"
#include "Interface.h"

Operacoes::Operacoes() {
}

Operacoes::Operacoes(const Operacoes& orig) {
}

Operacoes::~Operacoes() {
}

void Operacoes::setSocketClient(SocketClient *sc) {
    this->sockClient = sc;
}

SocketClient *Operacoes::getSocketClient() {
    return this->sockClient;
}

void Operacoes::start() {
   
    Conta conta;
    int numeroConta;
    Interface inter;
    
    numeroConta = inter.getNumeroConta();
   
    switch(loadConta(numeroConta)){
        case 0:
            conta = inter.getNovaConta(numeroConta);
            saveConta(&conta);
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
    }
    
}

int Operacoes::loadConta(int numeroConta) {

    int ret;
    char *buffer;
    char tipo;
    Msg *msg;
    unsigned int len;
    
    log_write("Tentando estabelecer conexao");
    ret = getSocketClient()->connectSocket();
    
    if(!ret){
        log_write("Nao foi possivel se conectar no servidor. %d", ret);
        return 2;
    }
    
    log_write("Criando pacote de comunicacao");
    msg = new Msg();
    msg->add(numeroConta);
    msg->setType('c');
    
    len = msg->getBuffer(&buffer);
    int retorno = getSocketClient()->writeSocket(buffer, len);
    log_write("Retorno do write: %d", retorno);
    
    getSocketClient()->readSocket(&len, sizeof(unsigned int));
    getSocketClient()->readSocket(&tipo, sizeof(char));
 
    log_write("Recebido tamanho %d do tipo %c", len, tipo);
    
    delete(msg);
    getSocketClient()->shutdownSocket();
    getSocketClient()->closeSocket();
    
    switch(tipo){
        case 'c':
            return 0;
        case 'C':
            return 1;
    }
 
    return 3;
}

int Operacoes::saveConta(Conta *conta) {
    
    int ret;
    int retorno;
    int len;
    int lenNomeTitular;
    Msg *msg;
    char *buffer;
    
    //getSocketClient()->setIP("127.0.0.1");
    //getSocketClient()->setPort(2020);
    
    log_write("Tentando estabelecer conexão");
    //ret = getSocketClient()->connectSocket();
    
    
    if(!ret){
        log_write("Nao foi possivel se conectar no servidor: %d", ret);
        return 2;
    }
    
    lenNomeTitular = conta->getTitularConta().size();
    
    log_write("Criando pacote para conexão");
    msg = new Msg();
    msg->setType('N');
    msg->add( conta->getNumeroConta());
    msg->add( (char *) conta->getTitularConta().c_str(), lenNomeTitular );
    
    len = msg->getBuffer(&buffer);
    
    retorno = getSocketClient()->writeSocket(buffer, len);
    log_write("Retorno do write: %d", retorno);
    
    delete(msg);
}


