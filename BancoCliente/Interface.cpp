/* 
 * File:   Interface.cpp
 * Author: nyex
 * 
 * Created on 13 de julho de 2019, 14:00
 */

#include <iostream>
#include <string.h>

#include "Interface.h"
#include "SocketClient.h"
#include "Msg.h"
#include "Operacoes.h"
#include "Conta.h"
#include "sys_log.h"

Interface::Interface() {
}

Interface::Interface(const Interface& orig) {
}

Interface::~Interface() {
}

int Interface::getNumeroConta() {

    unsigned int numeroConta;

    cout << " [ BANCO JC ] " << endl;
    cout << endl;
    cout << " [ * ] Digite o numero da conta: " << endl;
    cin >> numeroConta;
    
    return numeroConta;
}

int Interface::getNovaConta(Conta *conta){

    string nomeTitular;
    int saldo;

    cout << " [ MENU PRINCIPAL - NOVA CONTA ] " << endl;
    cout << endl;
    cout << " [ * ] Numero da conta: " << conta->getNumeroConta() << endl;
    cout << " [ * ] Nome do tiular: ";
    cin >> nomeTitular;
    cout << " [ * ] Saldo da Conta: R$";
    cin >> saldo;
    
    conta->setTitularConta(nomeTitular);
    conta->setSaldoDisponivel(saldo);
    
    return 1;
}

int Interface::printMenuConta(Conta *conta) {

    int op;
    
    cout << " [ MENU PRINCIPAL ] " << endl;
    cout << endl;
    cout << "Numero Conta: " << conta->getNumeroConta() << endl;
    cout << "Titular: " << conta->getTitularConta() << endl;
    cout << endl;
    cout << " [ 1 ] Sacar Dinheiro" << endl;
    cout << " [ 2 ] Depositar Dinheiro" << endl;
    cout << " [ 3 ] Transferir Dinheiro" << endl;
    cout << " [ 4 ] Saldo da conta" << endl;
    cout << " [ 9 ] Sair da conta" << endl;
    cin >> op;
    
    return op;
}

int Interface::getValor(string titulo, string dado) {
    
    int valor;
    
    cout << " [ "<< titulo << " ] " << endl;
    cout << endl;
    cout << " [ * ] "<< dado <<": R$";
    fflush(stdout);
    cin >> valor;

    
    return valor;
}

void Interface::printSaldoConta(int valor) {
    cout << " [ SALDO DA CONTA ] " << endl;
    cout << endl;
    cout << " [ * ] Saldo: R$" << valor << endl;
    cout << endl;
}

void Interface::printRetDeposito(int retorno) {
    cout << " [ DEPOSITO ] " << endl;
    switch(retorno){
        case 0:
            cout << "[ * ] Não foi possivel realizar o deposito." << endl;
            break;
        case 1:
            cout << "[ * ] Depositado." << endl;
            break;
    }
}

void Interface::printRetSaque(int retorno) {
    cout << " [ SAQUE ] " << endl;
    switch(retorno){
        case 0:
            cout << "[ * ] Não foi possivel realizar o saque." << endl;
            break;
        case 1:
            cout << "[ * ] Saque efetuado." << endl;
            break;
    }
}


void Interface::clear() {
    for(int i = i; i<=20; i++){
        cout << endl;
    }
}

