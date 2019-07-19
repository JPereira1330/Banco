/* 
 * File:   Msg.h
 * Author: nyex
 *
 * Created on 17 de julho de 2019, 19:00
 */

#ifndef MSG_H
#define MSG_H

class Msg {
public:
    Msg();
    Msg(const Msg& orig);
    virtual ~Msg();
    
    void setType(char type);
    int setBuffer(char *buffer, int len);
    
    int add(int valor);
    int add(unsigned int valor);
    int add(char *str, int len);
    unsigned int getBuffer(char **buffer); 
    
    int next(int *valor);
    int next(unsigned int *valor);
    unsigned int next(char **str);
    
private:
    char type;
    char *buffer;
    unsigned int lenBuffer;
    unsigned int position;

};

#endif /* MSG_H */

