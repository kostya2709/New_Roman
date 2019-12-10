#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Tree_Head.h"

const int MAX_PROGRAM_SIZE = 1024;
const int MAX_PROGRAM_NAME_SIZE = 64;

const int MAX_WORD_NUM = 1024;
const int MAX_VAR_NAME = 64;
const int MAX_VAR_AMOUNT = 64;

char* File_Reader (char* file_name, long* num_lines, long* num_symb);

Node** Tokenization (const char* program_str, int& com_num);

int* Make_Hash_Array (char** array, int size);

int Make_Hash_Str (char* str);

int Find_Hash (int* hash_ar, int size, int target);

void Back_End (Node* start, Hash* hash1);
