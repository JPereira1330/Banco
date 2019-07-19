
/* 
 * File:   Operacoes.cpp
 * Author: nyex
 * 
 * Created on 18 de julho de 2019, 20:27
 */

#include <stdlib.h>
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
   
    int valor;
    Conta conta;
    int numeroConta;
    Interface inter;
    
    numeroConta = inter.getNumeroConta();
   
    if(loadConta(numeroConta) == 0){
        conta = inter.getNovaConta(numeroConta);
        saveConta(&conta);
    }else{
        switch(inter.printMenuConta(&conta)){
            case 1:
                valor = inter.getValorSacado();
                sacarDinheiroConta(&conta, valor);
                break;
            case 2:
                valor = inter.getValorDepositado();
                depositarDinheiroConta(&conta, valor);
                break;
            //case 3:
                // Transferir
                //break;
            case 9:
                exit(1);
                break;
            default:
                exit(1);
        }
    }

    start();
}

int Operacoes::loadConta(int numeroConta) {

    int ret;
    int retorno;
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
    msg->setType('C');
    
    len = msg->getBuffer(&buffer);
    retorno = getSocketClient()->writeSocket(buffer, len);
    log_write("Retorno do write: %d", retorno);
        
    getSocketClient()->readSocket(&len, sizeof(unsigned int));
    getSocketClient()->readSocket(&tipo, sizeof(char));
 
    log_write("Recebido tamanho %d do tipo %c", len, tipo);
    
    //delete(msg);
    //delete(buffer);
    
    //getSocketClient()->shutdownSocket();
    //getSocketClient()->closeSocket();
    
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
    char tipo;
    Msg *msg;
    char *buffer;
    
    log_write("Tentando estabelecer conexão");
    ret = getSocketClient()->connectSocket();
    
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
    
    getSocketClient()->readSocket(&len, sizeof(unsigned int));
    getSocketClient()->readSocket(&tipo, sizeof(char));
 
    log_write("Recebido tamanho %d do tipo %c", len, tipo);
    
    //delete(msg);          - Da overflow
    //delete(buffer);       - Da overflow
    
    //getSocketClient()->shutdownSocket();
    //getSocketClient()->closeSocket();
    
    switch(tipo){
        case 'n':
            return 0;
        case 'N':
            return 1;
    }
 
    return 3;
}

int Operacoes::sacarDinheiroConta(Conta* conta, int saque) {
    
    int ret;
    
    conta->sacar(saque);
    conta->save();
    
    // Enviar para o servidor
    log_write("Tentando estabelecer conexão");
    ret = getSocketClient()->connectSocket();
    
     if(!ret){
        log_write("Nao foi possivel se conectar no servidor: %d", ret);
        return 2;
    }
    
    return 1;
}

int Operacoes::depositarDinheiroConta(Conta* conta, int deposito) {

    int ret;
    int len;
    char tipo;
    Msg *msg;
    char *buffer;
    
    conta->deposito(deposito);
    conta->save();
    
    // Enviar para o servidor
    log_write("Tentando estabelecer conexão");
    ret = getSocketClient()->connectSocket();
    
     if(!ret){
        log_write("Nao foi possivel se conectar no servidor: %d", ret);
        return 2;
    }
    
    log_write("Criando pacote para conexão");
    msg = new Msg();
    msg->setType('D');
    msg->add(conta->getNumeroConta());
    msg->add(conta->getSaldoDisponivel());
    
    len = msg->getBuffer(&buffer);
    
    ret = getSocketClient()->writeSocket(buffer, len);
    log_write("Retorno do write: %d", ret);
    
    getSocketClient()->readSocket(&len, sizeof(unsigned int));
    getSocketClient()->readSocket(&tipo, sizeof(char));
    
    return 1;
}
