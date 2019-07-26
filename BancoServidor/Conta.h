/*
 * File:   Conta.h
 * Author: nyex
 *
 * Created on 14 de julho de 2019, 19:13
 */

#ifndef CONTA_H
#define CONTA_H

#include <string>

using namespace std;

class Conta {
public:
    Conta();
    Conta(const Conta& orig);
    virtual ~Conta();

    /**
     * Construtor da classe
     * @param nome Nome do titular
     * @param numeroConta Numero da conta do titular
     */
    Conta(string nome, int numeroConta);
    
    /**
     * Metoto utilizado para sacar dinheiro da conta.
     * O dinheiro é adicionado no credito pendente.
     * @param valor - Valor a ser depositado
     * @return 1 Sucesso, 0 Falhou. 
     */
    int sacar(int valor);
    
    /**
     * Metodo utilizado para depositar dinheiro na conta.
     * O dinheiro é adicionado no credito pendente.
     * @param valor
     * @return 1 Sucesso, 0 Falhou. 
     */
    int deposito(int valor);
    
    /**
     * Adiciona o credito pendente no saldo e zera o credito pendente.
     * @return 1 Sucesso, 0 Falhou. 
     */
    int save();
    
    /**
     * Zera o credito pendente
     * @return 1 Sucesso, 0 Falhou. 
     */
    int rollback();
    
    // Setter's
    void setTitularConta(string nome);
    void setNumeroConta(int numeroConta);
    void setSaldoDisponivel(int saldo);
    void setCreditoPendente(int credito);
    
    // Getter's
    string getTitularConta();
    int getNumeroConta();
    int getSaldoDisponivel();
    int getCreditoPendente();
    
private:
    string titularConta;
    int numeroConta;
    int saldoDisponivel;
    int creditoPendente;
    
    
};

#endif /* CONTA_H */

