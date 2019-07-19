/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>

#include "sys_log.h"

void log_write_i(const char *fileName, int line, const char *format, ...){
    char buf[1024];     // this should really be sized appropriately
                       // possibly in response to a call to vsnprintf()
    va_list vl;
    va_start(vl, format);

    vsnprintf( buf, sizeof( buf), format, vl);

    printf("%s: %d --> %s\n", fileName, line, buf);
    
    va_end( vl);
}