#ifndef LAB6_Z1_EXHEADER_H
#define LAB6_Z1_EXHEADER_H
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 4096
#define MAX_CLIENTS 10

typedef enum message{
    STOP,
    LIST,
    TOALL,
    TOONE,
    INIT
} message;

#endif //LAB6_Z1_EXHEADER_H
