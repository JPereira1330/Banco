/* 
 * File:   BancoDB.h
 * Author: nyex
 *
 * Created on 14 de julho de 2019, 20:40
 */

#ifndef BANCODB_H
#define BANCODB_H

#include <list>
#include <string.h>
#include "Conta.h"

using namespace std;

typedef struct dbconta{
    int numero_conta;
    char nome_dono[32];
    float saldo_conta;
} DBCONTA;

class BancoDB {
public:
    BancoDB();
    BancoDB(const BancoDB& orig);
    virtual ~BancoDB();
    
    void add(Conta *conta);
    int saveFile();
    int loadFile();
    Conta *searchFile(int numeroConta);
    
private:
    list<Conta*> contas;
    
};

#endif /* BANCODB_H */

