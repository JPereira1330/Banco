/* 
 * File:   ClientProcessor.h
 * Author: nyex
 *
 * Created on 17 de julho de 2019, 20:04
 */

#ifndef CLIENTPROCESSOR_H
#define CLIENTPROCESSOR_H

#include <thread>

#include "Msg.h"
#include "BancoDB.h"

using namespace std;

class ClientProcessor{

public:
    ClientProcessor();
    ClientProcessor(const ClientProcessor& orig);
    virtual ~ClientProcessor();

    void SetBancoDB(BancoDB* bancoDB);
    void SetHandle(int handle);
    int getHandle();
    BancoDB *getBanco();
    Conta* GetConta();
    void SetConta(Conta* conta);

    
    int start();
    static void run(ClientProcessor *cp);
    
private:
    int handle;
    BancoDB *bancoDB;
    Conta *conta;
    
    static Conta *loadAcc(Msg *msg, ClientProcessor *cp);
    static Conta *saveAcc(Msg *msg, ClientProcessor *cp);
    static int depositaAcc(Msg *msg, ClientProcessor *cp, Conta *conta);
    static int saqueAcc(Msg *msg, ClientProcessor *cp, Conta *conta);
    static int saldoAcc(ClientProcessor *cp, Conta *conta);
    static int transferenciaAcc(Msg *msg, ClientProcessor *cp, Conta *conta);
    
};

#endif /* CLIENTPROCESSOR_H */

