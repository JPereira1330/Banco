/* 
 * File:   main.cpp
 * Author: nyex
 *
 * Created on 13 de julho de 2019, 14:00
 */

#include <cstdlib>
#include <cstring>

#include "Interface.h"
#include "SocketClient.h"
#include "Msg.h"
#include "Operacoes.h"

using namespace std;

int main(int argc, char** argv) {
    
    SocketClient sockClient;
    Operacoes op;
    
    sockClient.setIP(argv[1]);
    sockClient.setPort(atoi(argv[2]));
    
    op.setSocketClient(&sockClient);
    op.start();
    
    return 0;
}

