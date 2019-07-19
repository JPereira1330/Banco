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
    
    int start();
    static void run(ClientProcessor *cp);
    
private:
    int handle;
    BancoDB *bancoDB;
    
    static int loadAcc(Msg *msg, ClientProcessor *cp);
    
};

#endif /* CLIENTPROCESSOR_H */

