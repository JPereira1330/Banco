
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
    
    log_write("Start - Estabelecendo conexao.");
    ret = this->getSocketClient()->connectSocket();
    
    if(!ret){
        log_write("Start - Não foi possivel se conectar ao servidor.");
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
            case 1: // Case de saque
                log_write("Start - Inicializando saque.");
                valor = inter.getValor("Deposito", "Valor a ser sacado.");
                valor = this->sacarDinheiroConta(conta, valor);
                inter.printRetSaque(valor);
                break;
                
            case 2: // Case de deposito
                log_write("Start - Inicializando deposito.");
                valor = inter.getValor("Deposito", "Valor a ser depositado.");
                valor = this->depositarDinheiroConta(conta, valor);
                inter.printRetDeposito(valor);                
                break;
                
            case 3: // Case de transferencia
                log_write("Start - Inicializando transferencia.");
                valor = inter.getValor("Transferencia","Valor a ser transferido.");
                numeroConta = inter.getNumeroConta();
                this->transferirDinheiroConta(conta, numeroConta, valor);
                break;
                
            case 4: // Case de extrato 
                log_write("Start - Inicializando extrato.");
                this->saldoDinheiroConta(conta);
                inter.printSaldoConta(conta->getSaldoDisponivel());
                break;
                
            case 8: // Solicita o desligamento do servidor
                log_write("Start - Solicitando desligamento");
                this->solicitaDesligamentoSrv();
                break;
                
            case 9: // Case para finalizar o programa
                log_write("Start - Encerrando processos.");
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

    char *buffer;
    char tipo;
    Msg *msg;
    unsigned int len;
    unsigned int lenBuffer;
    
    log_write("Carregar Conta - Criando pacote");
    msg = new Msg();
    msg->setType('C');
    msg->add(conta->getNumeroConta());
    
    len = msg->getBuffer(&buffer);
    log_write("Carregar Conta - Tamanaho do buffer: %d bytes.", len);
    
    len = getSocketClient()->writeSocket(buffer, len);
    log_write("Carregar Conta - Dados enviados: %d bytes.", len);

    delete(msg);
    
    len = getSocketClient()->readSocket(&lenBuffer, sizeof(unsigned int));
    log_write("Carregar Conta - Dados recebido: %d bytes.", len);
    
    if(len == 0){
        log_write("Carregar Conta - Erro na leitura dos dados.");
        delete(msg);
        return 0;
    }
    
    len = getSocketClient()->readSocket(&tipo, sizeof(char));
    log_write("Carregar Conta - Dados recebido: %d bytes.", len);
        
    if(len == 0){
        log_write("Carregar Conta - Erro na leitura dos dados.");
        delete(msg);
        return 0;
    }
    
    msg = new Msg();
    msg->setType(tipo);

    log_write("Carregar Conta - Tipo recebido: %c.", tipo);   

    switch(tipo){
        case 'c':
            delete(msg);
            return 0;
        case 'C':
            getServerConta(conta, msg, lenBuffer);
            delete(msg);
            return 1;
    }
 
    return 3;
}

int Operacoes::saveConta(Conta *conta) {

    char tipo;
    Msg *msg;
    char *buffer;
    unsigned int len;
    
    len = conta->getTitularConta().size();
    
    log_write("Criar Conta - Criando pacote");
    msg = new Msg();
    msg->setType('N');
    msg->add(conta->getNumeroConta());
    msg->add((char *) conta->getTitularConta().c_str(), len);
    msg->add(conta->getSaldoDisponivel());
    
    len = msg->getBuffer(&buffer);
    log_write("Criar Conta - Tamanaho do buffer: %d bytes.", len);
    
    len = getSocketClient()->writeSocket(buffer, len);
    log_write("Criar Conta - Dados enviados: %d bytes.", len);

    len = readMsg(this->getSocketClient(), msg);
    log_write("Criar Conta - Dados recebido: %d bytes.", len);
    
    // Caso não receba nenhum dado do servidor.
    if(len == 0){
        log_write("Criar Conta - Erro na leitura dos dados.");
        delete(msg);
        return 0;
    }

    tipo = msg->getType();
    log_write("Criar Conta - Tipo recebido: %c.", tipo);
    
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
    
    int len;
    int saldo;
    char tipo;
    char *buffer;
    Msg *msg;
    
    log_write("Saque - Criando pacote.");
    msg = new Msg();
    msg->setType('S');
    msg->add(saque);
    
    len = msg->getBuffer(&buffer);
    log_write("Saque - Tamanaho do buffer: %d bytes.", len);
    
    len = this->getSocketClient()->writeSocket(buffer, len);
    log_write("Saque - Dados enviados: %d bytes.", len);
    
    len = readMsg(this->getSocketClient(), msg);
    log_write("Saque - Dados recebido: %d bytes.", len);
    
    // Caso não receba nenhum dado do servidor.
    if(len == 0){
        log_write("Saque - Erro na leitura dos dados.");
        delete(msg);
        return 0;
    }
    
    tipo = msg->getType();
    log_write("Saque - Tipo recebido: %c.", tipo);
    
    msg->next(&saldo);
    log_write("Saque - Saldo atualizado: R$%d.", saldo);
    
    conta->setSaldoDisponivel(saldo);
    
    delete(msg);
    
    if(tipo == 's'){
        return 0;
    }
        
    return 1;
}

int Operacoes::depositarDinheiroConta(Conta* conta, int deposito) {

    int saldo;
    int len;
    char tipo;
    Msg *msg;
    char *buffer;
    
    log_write("Deposito - Criando pacote.");
    msg = new Msg();
    msg->setType('D');
    msg->add(deposito);
    
    len = msg->getBuffer(&buffer);
    log_write("Deposito - Tamanaho do buffer: %d bytes.", len);
    
    len = this->getSocketClient()->writeSocket(buffer, len);
    log_write("Deposito - Dados enviados: %d bytes.", len);
    
    len = readMsg(this->getSocketClient(),msg);
    log_write("Deposito - Dados recebido: %d bytes.", len);
    
    // Caso não receba nenhum dado do servidor.
    if(len == 0){
        log_write("Deposito - Erro na leitura dos dados.");
        delete(msg);
        return 0;
    }
    
    tipo = msg->getType();
    log_write("Deposito - Tipo recebido: %c.", tipo);
    
    msg->next(&saldo);
    log_write("Deposito - Saldo atualizado: R$%d.", saldo);
    
    conta->setSaldoDisponivel(saldo);
    
    delete(msg);

    if(tipo == 'd'){
        return 0;
    }
        
    return 1;
}

int Operacoes::getServerConta(Conta* conta, Msg *msg, int len) {
    
    char *buffer;
    int ret;
    int numeroConta;
    int strLen;
    
    buffer = (char *) malloc(len);
    
    log_write("getServerConta - Aguardando restante da mensagem. [%d Bytes]", len);
    ret = getSocketClient()->readSocket(buffer, len);
    log_write("getServerConta - Dados recebido: %d bytes.", ret);
    
    if(ret == 0){
        log_write("getServerConta- Erro na leitura dos dados.");
        delete(msg);
        return 0;
    }
    
    msg->setBuffer(buffer, len);
    free(buffer);
    msg->next(&numeroConta);
    strLen = msg->next(&buffer);
    
    conta->setNumeroConta(numeroConta);
    conta->setTitularConta(string(buffer, strLen));
    
    return 1;
}

int Operacoes::saldoDinheiroConta(Conta* conta) {
    
    int len;
    Msg *msg;
    char *buffer;
    int saldo;
    
    log_write("Saldo - Criando pacote.");
    msg = new Msg();
    msg->setType('E');
    
    len = msg->getBuffer(&buffer);
    log_write("Saldo - Tamanaho do buffer: %d bytes.", len);

    len = this->getSocketClient()->writeSocket(buffer, len);
    log_write("Saldo - Dados enviados: %d bytes.", len);
    
    len = readMsg(this->getSocketClient(), msg);
    log_write("Saldo - Dados recebido: %d bytes.", len);
    
    // Caso não receba nenhum dado do servidor.
    if(len == 0){
        log_write("Saldo - Erro na leitura dos dados.");
        delete(msg);
        return 0;
    }

    msg->next(&saldo);
    log_write("Saldo - Saldo atualizado: R$%d.", saldo);
    delete(msg);
    
    conta->setSaldoDisponivel(saldo);
    return 1;
}

int Operacoes::transferirDinheiroConta(Conta* origem, int destino, int valor) {
    
    int len;
    Msg *msg;
    char *buffer;
    
    log_write("Transferencia - Criando pacote.");
    msg = new Msg();
    msg->setType('T');
    msg->add(destino);
    msg->add(valor);
    
    len = msg->getBuffer(&buffer);
    log_write("Transferencia - Tamanaho do buffer: %d bytes.", len);
    
    if(len <= 0){
        log_write("Erro de leitura");
        delete(msg);
        return 0;
    }
    
    len = this->getSocketClient()->writeSocket(buffer, len);
    log_write("Transferencia - Dados enviados: %d bytes.", len);
    
    if(len <= 0){
        log_write("Erro de leitura");
        delete(msg);
        return 0;
    }
    
    len = readMsg(this->getSocketClient(), msg);
    log_write("Saldo - Dados recebido: %d bytes.", len);
    
    if(len <= 0){
        log_write("Erro de leitura");
        delete(msg);
        return 0;
    }

    switch(msg->getType()){
        case 't':
            delete(msg);
            return 0;
            break;
        case 'T':
            delete(msg);
            return 1;
            break;
    }
    
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

int Operacoes::solicitaDesligamentoSrv(){
    
    int len;
    Msg *msg;
    char *buffer;
    
    log_write("Delisgando Servidor - Criando pacote");
    msg = new Msg();
    msg->setType('X');
    msg->getBuffer(&buffer);

    len = msg->getBuffer(&buffer);
    log_write("Delisgando Servidor - Tamanaho do buffer: %d bytes.", len);
    
    if(len <= 0){
        log_write("Erro de leitura");
        delete(msg);
        return 0;
    }
    
    len = this->getSocketClient()->writeSocket(buffer, len);
    log_write("Delisgando Servidor - Dados enviados: %d bytes.", len);
    
    if(len <= 0){
        log_write("Erro de leitura");
        delete(msg);
        return 0;
    }
    
    return 1;
}