#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#define empty -666

#define Stack_Constructor( this )         \
    Stack_Construct ( &this );            \
    this.name = #this;


const int MAX_STACK_SIZE = 20;

typedef int elem_t;

struct Stack_t
{
    elem_t data[20];
    int size;
    int max_size;
    char* name;
    int error;
};

int Stack_Construct (Stack_t* stk);

int Stack_Destruct (Stack_t* stk);

int Stack_Push (Stack_t* stk, elem_t value);

elem_t Stack_Pop (Stack_t* stk);

int Dump (Stack_t* stk, int line);

int Stack_OK (Stack_t* stk);

