/* 
 * File:   main.cpp
 * Author: nyex
 *
 * Created on 13 de julho de 2019, 19:14
 */

#include <iostream>
#include <cstdlib>

#include "SocketServer.h"
#include "ClientProcessor.h"
#include "sys_log.h"

using namespace std;

int main(int argc, char** argv) {
    
    log_write("Iniciando o programa");
    ClientProcessor *cp;
    BancoDB bdb;
    SocketServer socksrv;
    socksrv.setPort(atoi(argv[1]));
    socksrv.setQueue(99);
    socksrv.openSocket();
    
    do{
        log_write("Esperando a conexao");
        cp = new ClientProcessor();
        int hand = socksrv.acceptSocket();
        log_write("Conexao efetuada com o handle: %d", hand);
        cp->SetHandle(hand);
        cp->SetBancoDB(&bdb);
        cp->start();
    }while(1);
    
    return 0;
}

