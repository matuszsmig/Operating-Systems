#ifndef mylib_h
#define mylib_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct PointerArray{
    int currentPostion;
    struct Block *pointerArray;
};

struct Block{
    int isActive;
    char* file;
};

struct PointerArray initPointerArray(int size);
void count(struct PointerArray *, char *file);
char *valueOnIndex(struct PointerArray *pointerArray, int index);
void *freeOnIndex(struct PointerArray *pointerArray, int index);
void *freeArray(struct PointerArray *pointerArray);


#endif