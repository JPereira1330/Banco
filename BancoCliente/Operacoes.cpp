
/* 
 * File:   Operacoes.cpp
 * Author: nyex
 * 
 * Created on 18 de julho de 2019, 20:27
 */

#include <stdlib.h>
#include <cstring>
#include <string>

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
      
    int ret;
    int valor;
    Conta *conta;
    int numeroConta;
    Interface inter;
    
    log_write("Tentando estabelecer conexao");
    ret = this->getSocketClient()->connectSocket();
    
    if(!ret){
        log_write("Nao foi possivel se conectar no servidor. %d", ret);
        return;
    }   
    
    conta = new Conta();
    numeroConta = inter.getNumeroConta();
    conta->setNumeroConta(numeroConta);

    if(loadConta(conta) == 0){
        inter.getNovaConta(conta);
        saveConta(conta);
    }
    
    do{
        ret = inter.printMenuConta(conta);
        switch(ret){
            case 1:
                valor = inter.getValorSacado();
                this->sacarDinheiroConta(conta, valor);
                break;
            case 2:
                valor = inter.getValorDepositado();
                this->depositarDinheiroConta(conta, valor);
                break;
            //case 3:
                // Transferir
                //break;
            case 4:
                saldoDinheiroConta(conta);
                inter.printSaldoConta(conta->getSaldoDisponivel());
                break;
            case 9:
                this->getSocketClient()->shutdownSocket();
                this->getSocketClient()->closeSocket();
                exit(1);
                break;
            default:
                exit(1);
        }
    }while(1);

}

int Operacoes::loadConta(Conta *conta) {

    int retorno;
    char *buffer;
    char tipo;
    Msg *msg;
    unsigned int len;
    
    log_write("Criando pacote de comunicacao");
    msg = new Msg();
    msg->setType('C');
    msg->add(conta->getNumeroConta());
    
    len = msg->getBuffer(&buffer);
    retorno = getSocketClient()->writeSocket(buffer, len);
    log_write("Retorno do write: %d", retorno);
    
    delete(msg);
    
    // Capturanod pacotes enviados do servidor
    getSocketClient()->readSocket(&len, sizeof(unsigned int));
    getSocketClient()->readSocket(&tipo, sizeof(char));
 
    msg = new Msg();
    msg->setType(tipo);
    
    log_write("Recebido tamanho %d do tipo %c", len, tipo);
    
    switch(tipo){
        case 'c':
            delete(msg);
            return 0;
        case 'C':
            getServerConta(conta, msg, len);
            delete(msg);
            return 1;
    }
 
    return 3;
}

int Operacoes::saveConta(Conta *conta) {
    
    int retorno;
    int len;
    int lenNomeTitular;
    char tipo;
    Msg *msg;
    char *buffer;

    lenNomeTitular = conta->getTitularConta().size();
    
    log_write("Criando pacote para conexão");
    msg = new Msg();
    msg->setType('N');
    msg->add(conta->getNumeroConta());
    msg->add((char *) conta->getTitularConta().c_str(), lenNomeTitular);
    msg->add(conta->getSaldoDisponivel());
    
    len = msg->getBuffer(&buffer);
    
    retorno = getSocketClient()->writeSocket(buffer, len);
    log_write("Retorno do write: %d", retorno);
    
    // Liberando espaço na memoria
    delete(msg);

    msg = new Msg();
    len = readMsg(this->getSocketClient(), msg);

    if(len == 0){
        log_write("Erro na leitura");
        delete(msg);
        return 0;
    }

    
    tipo = msg->getType();
    log_write("Recebido tamanho %d do tipo %c", len, tipo);
    delete(msg);
    
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

int Operacoes::getServerConta(Conta* conta, Msg *msg, int len) {
    
    char *buffer;
    int numeroConta;
    int strLen;
    
    buffer = (char *) malloc(len);
    
    getSocketClient()->readSocket(buffer, len);
    
    msg->setBuffer(buffer, len);
    free(buffer);
    msg->next(&numeroConta);
    strLen = msg->next(&buffer);
    
    conta->setNumeroConta(numeroConta);
    conta->setTitularConta(string(buffer, strLen));
    
    return 1;
}

int Operacoes::saldoDinheiroConta(Conta* conta) {
    
    int ret;
    int len;
    Msg *msg;
    char *buffer;
    int saldo;
    
    // Criando pacote de requisição
    msg = new Msg();
    msg->setType('E');
    
    // Definindo buffer e capturando tamanho
    len = msg->getBuffer(&buffer);
    
    for(int i=0; i < len; i++){
        log_write("%d", buffer[i]);
    }
    
    // Enviando requisição do saldo da conta
    ret = this->getSocketClient()->writeSocket(buffer, len);
    log_write("Enviado: %d Bytes", ret);
    
    // Liberando espaço na memoria
    delete(msg);
    
    msg = new Msg();
    len = readMsg(this->getSocketClient(), msg);
    log_write("[TEMP] Recebeu saldo - Tamanho: %d", len);
    if(len == 0){
        log_write("Erro na leitura");
        delete(msg);
        return 0;
    }
    
    msg->next(&saldo);
    delete(msg);
    conta->setSaldoDisponivel(saldo);
    return 1;
}

int Operacoes::readMsg(SocketClient* sc, Msg* msg) {
    
    int ret;
    int len;
    char tipo;
    char *buffer;
    
    ret = sc->readSocket(&len, sizeof(unsigned int));
    
    if(ret <= 0){
        log_write("Erro de leitura");
        return 0;
    }
    
    ret = sc->readSocket(&tipo, sizeof(char));
    
    if(ret <= 0){
        log_write("Erro de leitura");
        return 0;
    }
    
    msg->setType(tipo);
    
    if(len == 0){
        return 1;
    }
     
    buffer = (char *) malloc(len);
    
    len = sc->readSocket(buffer, len);
    
    if(len <= 0){
        free(buffer);
        return 0;
    }
    
    msg->setBuffer(buffer, len);
    free(buffer);
    
    return 1;
}


