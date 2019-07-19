/* 
 * File:   Msg.cpp
 * Author: nyex
 * 
 * Created on 17 de julho de 2019, 19:00
 */

#include <cstdlib>
#include <cstring>

#include "Msg.h"

Msg::Msg() {
    this->position = sizeof(unsigned int) + sizeof(char);
    this->lenBuffer = sizeof(unsigned int) + sizeof(char);
    this->buffer = (char *) malloc(this->lenBuffer);
}

Msg::Msg(const Msg& orig) {
}

Msg::~Msg() {
    if(this->buffer != NULL){
        free(this->buffer);
    }
}

void Msg::setType(char type) {
    this->type = type;
}

int Msg::setBuffer(char* buffer, int len) {
    char *tmpBuffer;
    
    tmpBuffer = (char *) malloc(this->lenBuffer + len);

    if(tmpBuffer == NULL){
        return 0;
    }

    memcpy(tmpBuffer, this->buffer, this->lenBuffer);
    memcpy(&tmpBuffer[this->lenBuffer], buffer, len);
    
    memset(this->buffer,0, this->lenBuffer);
    this->lenBuffer += len;
    
    free(this->buffer);
    this->buffer = tmpBuffer;
    
    return 1;
}


int Msg::add(int valor) {
    char *buffer;
    
    buffer = (char *) malloc(this->lenBuffer + sizeof(int));

    if(buffer == NULL){
        return 0;
    }

    memcpy(buffer, this->buffer, this->lenBuffer);
    memcpy(&buffer[this->lenBuffer], &valor, sizeof(int));
    
    memset(this->buffer,0, this->lenBuffer);
    this->lenBuffer += sizeof(int);
    
    free(this->buffer);
    this->buffer = buffer;
    
    return 1;
}

int Msg::add(unsigned int valor) {
    char *buffer;
    
    buffer = (char *) malloc(this->lenBuffer + sizeof(unsigned int));

    if(buffer == NULL){
        return 0;
    }

    memcpy(buffer, this->buffer, this->lenBuffer);
    memcpy(&buffer[this->lenBuffer], &valor, sizeof(unsigned int));
    
    memset(this->buffer,0, this->lenBuffer);
    this->lenBuffer += sizeof(unsigned int);
    
    free(this->buffer);
    this->buffer = buffer;
    
    return 1;
}

int Msg::add(char* str, int len) {
    char *buffer;
    
    buffer = (char *) malloc(this->lenBuffer + len + sizeof(unsigned int));

    if(buffer == NULL){
        return 0;
    }

    memcpy(buffer, this->buffer, this->lenBuffer);
    memcpy(&buffer[this->lenBuffer], &len, sizeof(unsigned int));
    memcpy(&buffer[this->lenBuffer + sizeof(unsigned int)], str, len);
    
    memset(this->buffer,0, this->lenBuffer);
    this->lenBuffer += len + sizeof(unsigned int);
    
    free(this->buffer);
    this->buffer = buffer;
    
    return 1;
}

unsigned int Msg::getBuffer(char** buffer) {
    int len;
    
    len = this->lenBuffer - (sizeof(unsigned int) + sizeof(char));

    memcpy(this->buffer, &len,  sizeof(unsigned int));
    this->buffer[sizeof(unsigned int)] = this->type;
    *buffer = this->buffer;
    
    return this->lenBuffer;
}

int Msg::next(int *valor) {
   
    if(this->position + sizeof(int) > this->lenBuffer){
        return 0;
    }
    
    memcpy(valor, &this->buffer[this->position], sizeof(int));
    this->position += sizeof(int);
    
    return 1; 
}

int Msg::next(unsigned int* valor) {

    if(this->position + sizeof(unsigned int) > this->lenBuffer){
        return 0;
    }
    
    memcpy(valor, &this->buffer[this->position], sizeof(unsigned int));
    this->position += sizeof(unsigned int);
    
    return 1; 
}

unsigned int Msg::next(char** str) {

    unsigned int len;
    
    if(this->position + sizeof(unsigned int) > this->lenBuffer){
        return 0;
    }
    
    memcpy(&len, &this->buffer[this->position], sizeof(unsigned int));
    this->position += sizeof(unsigned int);

    if(this->position + len > this->lenBuffer){
        return 0;
    }
    
    *str = &this->buffer[this->position];
    this->position += len;
    
    return len; 
}