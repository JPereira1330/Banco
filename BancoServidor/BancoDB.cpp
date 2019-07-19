/* 
 * File:   BancoDB.cpp
 * Author: nyex
 * 
 * Created on 14 de julho de 2019, 20:40
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include "BancoDB.h"
#include "Conta.h"

#define DIR_BANCO "banco.db"

BancoDB::BancoDB() {
}

BancoDB::BancoDB(const BancoDB& orig) {
}

BancoDB::~BancoDB() {
    for (list<Conta *>::iterator it = this->contas.begin(); it != this->contas.end(); ++it) {
        delete(*it);
    }
}

void BancoDB::add(Conta* conta) {
    this->contas.push_back(conta);
}

int BancoDB::saveFile() {
    DBCONTA dbconta;
    FILE *fp;
    fp = fopen(DIR_BANCO, "w");

    if (!fp) {
        return 0;
    }

    for (list<Conta *>::iterator it = this->contas.begin(); it != this->contas.end(); ++it) {
        dbconta.numero_conta = (*it)->getNumeroConta();
        dbconta.saldo_conta = (*it)->getSaldoDisponivel();
        snprintf(dbconta.nome_dono, sizeof(dbconta.nome_dono), "%s", (*it)->getTitularConta().c_str());
        fwrite(&dbconta,1,sizeof(DBCONTA), fp);
    }

    fclose(fp);
    return 1;
}

int BancoDB::loadFile() {
    DBCONTA dbconta;
    FILE *fp;
    fp = fopen(DIR_BANCO, "r");
    int lenFile, totalContas;
    Conta *conta;
    
    if(contas.size()){
        return 0;
    }
    
    if(!fp){
        return 0;
    }
    
    fseek(fp, 0L, SEEK_END);
    lenFile = ftell(fp);
    rewind(fp);
    totalContas = lenFile / sizeof(DBCONTA);
    
    for(int i = 0; i < totalContas; i++){
        fread(&dbconta,1,sizeof(DBCONTA), fp);
        
        conta = new Conta();
        conta->setTitularConta(string(dbconta.nome_dono));
        conta->setNumeroConta(dbconta.numero_conta);
        conta->setSaldoDisponivel(dbconta.saldo_conta);
        
        this->add(conta);
    }
    
    return 1;
}

Conta* BancoDB::searchFile(int numeroConta) {
    
    for (list<Conta *>::iterator it = this->contas.begin(); it != this->contas.end(); ++it) {
        if( (*it)->getNumeroConta() == numeroConta ){
            return *it;
        }
    }
    
    return NULL;
}
