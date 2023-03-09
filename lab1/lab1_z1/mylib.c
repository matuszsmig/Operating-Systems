#include <stdio.h>
#include "mylib.h"

int maxSize = 100;

struct PointerArray initPointerArray(int size){
    struct PointerArray myArray;

    myArray.pointerArray = calloc(size, sizeof(struct Block));
    myArray.currentPostion = -1;

    return myArray;
}

void count(struct PointerArray *pointerArray, char *file){
    char * mkFile = calloc(maxSize, sizeof(char));
    char * rmFile = calloc(maxSize, sizeof(char));
    char * countValues = calloc(maxSize, sizeof(char));

    strcpy(mkFile, "mkdir tmp; wc ");
    strcat(mkFile, file);
    strcat(mkFile, " > tmp/text.txt");

    strcpy(rmFile, "rm tmp/text.txt; rm -d tmp");

    system(mkFile);

    FILE* searchFile;
    char ch;
    searchFile = fopen("tmp/text.txt", "r");

    if (searchFile == NULL)
    {
        printf("Incorrect file");
        exit(1);
    }

    int itr = 0;
    do {
        ch = fgetc(searchFile);
        countValues[itr] = ch;
        itr+=1;
    } while (ch != EOF);

    fclose(searchFile);

    system(rmFile);

    pointerArray->currentPostion+=1;
    pointerArray->pointerArray[pointerArray->currentPostion].file =
            (char*) calloc(itr, sizeof(char));
    pointerArray->pointerArray[pointerArray->currentPostion].isActive = 1;

    strcpy((char *) pointerArray->pointerArray[pointerArray->currentPostion].file, countValues);

    //printf("%s", pointerArray->pointerArray[pointerArray->currentPostion].file);
    free(countValues);
}

char *valueOnIndex(struct PointerArray *pointerArray, int index){
    return (char *) pointerArray->pointerArray[index].file;
}

void *freeOnIndex(struct PointerArray *pointerArray, int index){
    pointerArray->pointerArray[index].isActive = 0;
    free(pointerArray->pointerArray[index].file);
    return NULL;
}

void *freeArray(struct PointerArray *pointerArray){
    int arrayIndex = pointerArray->currentPostion;
    for (int i = 0; i!= arrayIndex+1; i++){
        if (pointerArray->pointerArray[i].isActive == 1){
            free(pointerArray->pointerArray[i].file);
            pointerArray->pointerArray[i].isActive = 0;
        }
    }
    pointerArray->currentPostion = -1;
    return NULL;
}