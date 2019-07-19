
/* 
 * File:   sys_log.h
 * Author: nyex
 *
 * Created on 18 de julho de 2019, 19:00
 */
#include <stdarg.h>


#ifndef SYS_LOG_H
#define SYS_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#define log_write(D_FORMAT, ...) log_write_i(__FILE__, __LINE__, D_FORMAT, ##__VA_ARGS__)
  
    void log_write_i(const char *fileName, int line, const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif /* SYS_LOG_H */

