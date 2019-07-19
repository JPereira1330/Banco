/* 
 * File:   Operacoes.h
 * Author: nyex
 *
 * Created on 18 de julho de 2019, 20:27
 */

#ifndef OPERACOES_H
#define OPERACOES_H

#include "SocketClient.h"
#include "Conta.h"

using namespace std;

class Operacoes {
public:
    Operacoes();
    Operacoes(const Operacoes& orig);
    virtual ~Operacoes();
    
    void setSocketClient(SocketClient *sc);
    SocketClient *getSocketClient();

    void start();
    
    /**
     * Função responsavel para carregar a conta
     * @param numeroConta
     * @return 0 Conta não existe,
     *          1 Conta existe,
     *          2 Erro de conexão,
     *          3 Erro estrutural
     */
    int loadConta(int numeroConta);
    
    /**
     * 
     * @param conta
     * @return 
     */
    int saveConta(Conta *conta);
    
private:
    SocketClient *sockClient;
};

#endif /* OPERACOES_H */

