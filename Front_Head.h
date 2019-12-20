#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Tree_Head.h"

typedef struct {
    char* start;
    char* end;
    }
poem_line;


const int MAX_PROGRAM_SIZE = 1024;
const int MAX_PROGRAM_NAME_SIZE = 64;

const int MAX_WORD_NUM = 2048;
const int MAX_VAR_NAME = 1024;
const int MAX_VAR_AMOUNT = 64;
const int MAX_FUNCS_AMOUNT = 64;
const int MAX_ARGS_AMOUNT = 10;
const int MAX_VAR_IN_FUNC = 64;
const int MAX_GLOBAL_NUM = 32;

bool Is_Symb (char a);

static char* file_asm = "../../ASSSembler/asm_progs/Lang/Lang.txt";

char* File_Reader (char* file_name, long* num_lines, long* num_symb);

Node** Tokenization (const poem_line* pointer, int num_lines, int& com_num);

int64_t* Make_Hash_Array (char** array, int size);

int64_t Make_Hash_Str (char* str);

int Find_Hash (int64_t* hash_ar, int size, int64_t target);

int Find_Hash (int64_t* hash_ar, int min, int max, int64_t target, int last_global);

void Back_End (Node* start, Hash* hash1, int comp);

void Back_Stand_End (Node* start, Hash* hash1);

poem_line* StringMaker (char* str, int length, int amount);

const int MAX_STACK_SIZE = 20;

typedef double elem_t;

struct Stack_t
{
    elem_t data [MAX_STACK_SIZE];
    int size;
    int max_size;
    char* name;
    int error;
};

int Stack_Construct (Stack_t* stk);

int Stack_Destruct (Stack_t* stk);

int Stack_Push (Stack_t* stk, elem_t value);

elem_t Stack_Pop (Stack_t* stk);

elem_t Show_First (Stack_t* stk);

int Dump (Stack_t* stk, int line);

int Stack_OK (Stack_t* stk);

int Insert_Node (Node* node1, Node* node_new, int pos);

Node* Copy_Node (Node* node);

int Is_Roman_Num (const char* number);

int Roman_Number (const char* number);

void Frt_End (Node* start, Hash* hash1, int comp);
