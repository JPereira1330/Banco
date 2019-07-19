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

Interface::Interface() {
}

Interface::Interface(const Interface& orig) {
}

Interface::~Interface() {
}

int Interface::getNumeroConta() {

    unsigned int numeroConta;

    clear();
    cout << " [ BANCO JC ] " << endl;
    cout << endl;
    cout << " [ * ] Digite o numero da conta: " << endl;
    cin >> numeroConta;
    
    return numeroConta;
}

Conta Interface::getNovaConta(int numeroConta){

    string nomeTitular;
    
    clear();
    cout << " [ MENU PRINCIPAL - NOVA CONTA ] " << endl;
    cout << endl;
    cout << " [ * ] Numero da conta: " << numeroConta << endl;
    cout << " [ * ] Nome do tiular: ";
    cin >> nomeTitular;
    
    Conta conta;
    conta.setNumeroConta(numeroConta);
    conta.setTitularConta(nomeTitular);
    
    return conta;
}

int Interface::printMenuConta(Conta *conta) {

    int op;
    
    clear();
    cout << " [ MENU PRINCIPAL " << conta->getNumeroConta() << " ] " << endl;
    cout << endl;
    cout << " [ 1 ] Sacar Dinheiro" << endl;
    cout << " [ 2 ] Depositar Dinheiro" << endl;
    cout << " [ 3 ] Transferir Dinheiro" << endl;
    cout << " [ 4 ] Extrato da conta" << endl;
    cout << " [ 9 ] Sair da conta" << endl;
    cin >> op;
    
    switch(op){
        case 1:
            return getValorSacado();
            break;
    }
    
    return 0;
}

int Interface::getValorSacado() {
    
    int valor;
    
    clear();
    cout << " [ MENU PRINCIPAL - SAQUE ] " << endl;
    cout << endl;
    cout << " [ * ] Valor a ser sacado: " << endl;
    cin >> valor;
    
    return valor;
}

int Interface::getValorDepositado() {
    
    int valor;
    
    clear();
    cout << " [ MENU PRINCIPAL - DEPOSITO ] " << endl;
    cout << endl;
    cout << " [ * ] Valor a ser depositado: " << endl;
    cin >> valor;
    
    return valor;
}


void Interface::clear() {
    for(int i = i; i<=20; i++){
        cout << endl;
    }
}

