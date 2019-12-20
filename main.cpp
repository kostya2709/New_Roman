#include <stdio.h>
#include <stdlib.h>
#include "Front_Head.h"



int main()
{
    setlocale (LC_ALL, "Russian");

    long int program_size = 0;
    long int num_lines = 0;
    char* program_name = (char*)calloc (1, MAX_PROGRAM_NAME_SIZE);
    sprintf ((char*)program_name, "program_qq.txt");
    char* program_str = NULL;

    program_str = File_Reader (program_name, &num_lines, &program_size);
    printf ("%s\n", program_str);

    char* program_copy = (char*)calloc (1, program_size + 1);
    sprintf (program_copy, "%s", program_str);
    poem_line* pointer = StringMaker (program_copy, program_size, num_lines);


    printf ("hash Kostya = %d\n", Make_Hash_Str("Kostya"));
    printf ("hash hey = %d\n", Make_Hash_Str("hey"));

    int com_num = 0;
printf ("sssuk\n");
    Node** node_line = Tokenization (pointer, num_lines, com_num);printf ("suk\n");

    printf ("\n\nCHECK\n\n");
    int i = 0;
    for (i = 0; i < com_num; i++)
    {
        printf ("%i sym = %s, data = %lg, type = %d\n", i, node_line[i]->sym, node_line[i]->data, node_line[i]->node_type);
    }

    Hash* hash;
    Tree prog_tree;
    prog_tree.Insert_Node(Get_General(node_line, &hash));
    prog_tree.Tree_Simplifier(prog_tree.first_elem);

    //prog_tree.Dump();

    prog_tree.File_Write ("From Frontend with love.txt");

    printf ("\n\nBEFORE FILE_READ\n\n");
    Tree read_tree;
    read_tree.File_Read ("From Frontend with love.txt", *hash);
    //read_tree.File_Write ("From Frontend-1 with love.txt");
    read_tree.Dump();
    Back_End (read_tree.first_elem, hash, 0);

    Frt_End (read_tree.first_elem, hash, 0);

    free (node_line);
    free (pointer);
    free (program_name);
    free (program_str);
    free (program_copy);

    return 0;
}
