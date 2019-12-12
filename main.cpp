#include <stdio.h>
#include <stdlib.h>
#include "Front_Head.h"



int main()
{

    setlocale (LC_ALL, "RUS");

    long int program_size = 0;
    long int num_lines = 0;
    char* program_name = (char*)calloc (1, MAX_PROGRAM_NAME_SIZE);
    sprintf ((char*)program_name, "program_qq.txt");
    char* program_str = NULL;

    program_str = File_Reader (program_name, &num_lines, &program_size);
    printf ("%s\n", program_str);

    char* program_copy = (char*)calloc (1, program_size);
    sprintf (program_copy, "%s", program_str);
    poem_line* pointer = StringMaker (program_copy, program_size, num_lines);


    printf ("hash Kostya = %d\n", Make_Hash_Str("Kostya"));
    printf ("hash hey = %d\n", Make_Hash_Str("hey"));

    int com_num = 0;

    Node** node_line = Tokenization (pointer, num_lines, com_num);

    printf ("\n\nCHECK\n\n");
    int i = 0;
    for (i = 0; i < com_num; i++)
    {
        printf ("%i data = %lg, type = %d\n", i, node_line[i]->data, node_line[i]->node_type);
    }

    Hash* hash;
    Tree prog_tree;
    prog_tree.Insert_Node(Get_General(node_line, &hash));
    prog_tree.Tree_Simplifier(prog_tree.first_elem);
    prog_tree.Dump();

    Back_End (prog_tree.first_elem, hash);

    free (program_name);
    free (program_str);
    free (program_copy);

    return 0;
}
