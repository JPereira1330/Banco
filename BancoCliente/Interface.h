
/* 
 * File:   Interface.h
 * Author: nyex
 *
 * Created on 13 de julho de 2019, 14:00
 */

#ifndef INTERFACE_H
#define INTERFACE_H

#include "SocketClient.h"
#include "Conta.h"

using namespace std;

class Interface {
public:
    Interface();
    Interface(const Interface& orig);
    virtual ~Interface();

    int getNumeroConta();
    int getNovaConta(Conta *conta);
    int printMenuConta(Conta *conta);
    int getValorSacado();
    int getValorDepositado();
    void printSaldoConta(int valor);
    
private:
    void clear();
    
};

#endif /* INTERFACE_H */

