#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <iostream>
#include <unistd.h>
#include <ctype.h>
#include <math.h>
#include <stdarg.h>

#include "key_words.h"

#define ACCUR "%lg"

#define ENDL Write_Str_To_PDF("\n\\end{math}\n\\end{center}\n");
#define STARTL Write_Str_To_PDF("\n\\begin{center}\n\\begin{math}\n");
#define PUT_STR(str) Write_Str_To_PDF (str);

typedef double elem_t;

const int FILE_SIZE = 256;

const int FILE_NAME_SIZE = 32;

const int operator_size = 6;


struct Node
{
public:

    Node* left;
    Node* right;
    Node* parent;

    elem_t data;

    char* sym;

    char node_type;

     Node();

    ~Node();

};

struct Tree
{

public:

    int elem_num;

    Node* first_elem;

    Tree();

    /*! This function inserts a node into your tree!
    @param[in] node_new New node that you want to insert.
    @param[in] node1 The node which precedes the new one.
    @param[in] pos The position to insert. Expects 0 for left or 1 for right.
    \return 0 if the function behaves correctly.
    */
    int Insert_Node (Node* node1, Node* node_new, int pos);


    /*! This function inserts a node into your tree! Attention! The function expects that the tree is empty!
    @param[in] node_new New node that you want to insert.
    \return 0 if the function behaves correctly.
    */
    int Insert_Node (Node* node_new);

    /*! This function displays all tree's nodes and edges in tree_dump.png.*/
    int Dump (void);

    /*!This function reads the information about the tree from the file f*/
    int File_Read (char* file_name);

    int Make_Simple_Tree (Node* node1);

    int Calculate_Consts (Node* node1);

    Node* Find_Derive (Node* node1, const char* der_var, int show);

    void Tree_Simplifier (Node* node1);

    ~Tree();

private:
    /*! An auxiliary function. Prints info about a tree to a GV file.*/
    void Tree_Print (const Node* node1, FILE* f);

    /*! An auxiliary function. Prints info about a tree to a GV file.*/
    void Tree_Info_Dump (const Node* node1, FILE* f);

    /*!An auxiliary function for File_Read.*/
    void File_Read_Cycle (Node* node1);

    int Kill_One (Node* node1);

    int Kill_Zero (Node* node1);

};

char* Delete_Str_Trash (char* str);

/*! Converts path from int into char* */
char* Path_Convert (int way);

elem_t Find_Sol (elem_t a, elem_t b, int operator_t);

elem_t Find_Sol (elem_t a, int operator_t);

Node* Tree_Copy (Node* prev_node);

Node* Create_Node (Node* left, Node* right, Node* parent, elem_t data, char* sym, int node_type);

int Write_From_File (char* file_name);

int File_Clean ();

int Write_Expr_To_PDF (Node* node1, int bracket);

Node* Get_General (Node** prog_node);

int Compile_LaTex (void);

int Insert_Var (Node* node1, char* var, elem_t value);

int Find_Operator (char* oper);

void Read_Aloud (char* str, ...);

void Read_Aloud_File (char* file_name);

int Random (int min, int max);

char* Write_Rand (char* str, int min, int max);

int Write_Str_To_PDF (char* string);

int Menu (void);

Tree* Derive_Main (char* file_name, char* der_var, int der_num, double value);

int Write_Double_To_PDF (double value);

