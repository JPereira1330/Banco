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

void Interface::printMenuConta() {

}

void Interface::clear() {
    for(int i = i; i<=20; i++){
        cout << endl;
    }
}

