/*
 * File:   Conta.cpp
 * Author: nyex
 * 
 * Created on 14 de julho de 2019, 19:13
 */

#include <iostream>
#include <string.h>
#include "Conta.h"
#include "sys_log.h"

Conta::Conta() {
}

Conta::Conta(const Conta& orig) {
}

Conta::Conta(string nome, int numeroConta) {
    setTitularConta(nome);
    setNumeroConta(numeroConta);
}

Conta::~Conta() {
}

int Conta::deposito(int valor) {
    
    // Verificando se valor para deposito é negativo
    if( valor < 0 ){
        return 0;
    }
    
    setCreditoPendente(valor);
    return 1;
}

int Conta::sacar(int valor) {
    
    log_write("Temp - %d < %d", valor, this->getSaldoDisponivel());
    
    // Verificando se possui saldo
    if( valor > this->getSaldoDisponivel() ){
        log_write("Temp - Entrou!");
        return 0;
    }
    
    log_write("Temp - Passou!!");
    setCreditoPendente(valor * (-1));    
    return 1;
}

int Conta::save() {
    setSaldoDisponivel( getSaldoDisponivel() + getCreditoPendente() );
    setCreditoPendente(0);
    return 1;
}

int Conta::rollback() {
    setCreditoPendente(0);
    return 1;
}


// GETTER's AND SETTER's
void Conta::setTitularConta(string titular) {
    this->titularConta = titular;
}

void Conta::setNumeroConta(int numeroConta) {
    this->numeroConta = numeroConta;
}

void Conta::setSaldoDisponivel(int saldo) {
    this->saldoDisponivel = saldo;
}

void Conta::setCreditoPendente(int credito) {
    this->creditoPendente = credito;
}

string Conta::getTitularConta() {
    return this->titularConta;
}

int Conta::getNumeroConta() {
    return this->numeroConta;
}

int Conta::getSaldoDisponivel() {
    return this->saldoDisponivel;
}

int Conta::getCreditoPendente() {
    return this->creditoPendente;
}
