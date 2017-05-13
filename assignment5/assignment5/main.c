#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "type.h"
#include "HashMap.h"

char* getWord(FILE *file);

int main (int argc, const char * argv[])
{
    HashMap hm;
    MapElement toAdd;
    MapElement* retrieved;
    clock_t start, finish;
    FILE *theFile;
    int hashChoice = 1;
    
    /**************************************
     When testing your code, change these values
     ***************************************/
    const char* filename = "input1.txt";
    int tableSize = 4;
    int showBuckets = 1;
    
    initHashMap(&hm, tableSize, hashChoice);
    
    /* Open the file */
    theFile = fopen(filename, "r");
    if(!theFile) {
        fprintf(stderr, "Cannot open %s\n", filename);
        return 1;
    }
    start = clock();
    /* This is an infinite loop, but it is ok because eventually the file will run out */
    while(1){
        toAdd.key = getWord(theFile);
        if(!toAdd.key)
            break;
        
        /* Assume we have not seen the word before */
        toAdd.value = 1;
        
        /* if the word has been seen before, increment its count */
        retrieved = atHashMap(&hm, toAdd.key);
        if(retrieved)
            toAdd.value = retrieved->value + 1;
        
        insertHashMap(&hm, toAdd);
    }
    finish = clock();
    
    printHashMap(&hm, showBuckets);
    printf("concordance ran in %f seconds\n", (float)(finish-start) / (float)CLOCKS_PER_SEC);
    
    fclose(theFile);
    freeHashMap(&hm);
    
    return 0;
}

/*	This is a function which will return the next character string in a file
	Note that it calls malloc/realloc, so the pointer returned MUST BE FREED!!!!
	Also, this function is not totally robust against certain kinds of punctuation, so keep it simple.
 */
char* getWord(FILE *file)
{
    int length = 0;
    int maxLength = 16;
    char character;
    /*char * c =0;*/
    
    char* word = (char*)malloc(sizeof(char) * maxLength);
    assert(word != NULL);
    
    
    while( (character = fgetc(file)) != EOF)
    {
        if((length+1) > maxLength)
        {
            maxLength *= 2;
            word = (char*)realloc(word, maxLength);
        }
        if((character >= '0' && character <= '9') || /*is a number*/
           (character >= 'A' && character <= 'Z') || /*or an uppercase letter*/
           (character >= 'a' && character <= 'z') || /*or a lowercase letter*/
           character == 39) /*or is an apostrophe*/
        {
            word[length] = character;
            length++;
            
        }
        else if(length > 0)
            break;
    }
    
    if(length == 0)
    {
        free(word);
        return NULL;
    }
    word[length] = '\0';
    return word;
}
