#include "Front_Head.h"



Node** Tokenization (const poem_line* pointer, int num_lines, int& com_num)
{
    Hash hash;

    Node** array = (Node**)calloc (sizeof (Node*), MAX_WORD_NUM);
    int cur_com_num = 0;

    char* program_str = pointer[0].start;
    char cur_sym = *program_str;
    double cur_val = 0;
    int letter_num = 0;
    char* cur_str = (char*)calloc (1, MAX_VAR_NAME);
    int i = 0;


    for (i = 0; i < num_lines; i++)
    {
        program_str = pointer[i].start;
        cur_sym = *program_str;


        while (cur_sym == ' ' || cur_sym == '\t')
        {
            program_str++;
            cur_sym = *program_str;
        }

        if (cur_sym == '\0')
            continue;


        while (program_str < pointer[i].end)
        {
            cur_sym = *program_str;

        if (isdigit (cur_sym) || (cur_sym == '-'))
        {
            sscanf (program_str, "%lf %n", &cur_val, &letter_num);
            array[cur_com_num] = Create_Node(NULL, NULL, NULL, cur_val, "", NUMBER);
            cur_com_num++;
            printf ("num = %lg, let_num = %d\n", cur_val, letter_num);

            program_str += letter_num;

        }

        else
        {
            if (isalpha (cur_sym))
                sscanf ((char*)program_str, "%[^ ,\n,\t,+,-,*,/,^,(,),#, 0] %n", cur_str, &letter_num);

            else
                sscanf ((char*)program_str, "%1s %n", cur_str, &letter_num);

            printf ("cur_str = \"%s\", num = %d\n", cur_str, letter_num);

            int hash_ans = Find_Hash (hash.oper, OPERATOR_NUMBER, Make_Hash_Str (cur_str));
            if (hash_ans != -1)
                array[cur_com_num] = Create_Node(NULL, NULL, NULL, hash_ans, cur_str, OPERATOR);
            else
            {
                hash_ans = Find_Hash (hash.k_words, K_WORDS_NUMBER, Make_Hash_Str (cur_str));
                if (hash_ans != -1)
                    array[cur_com_num] = Create_Node(NULL, NULL, NULL, hash_ans, cur_str, K_WORD);
                else
                    array[cur_com_num] = Create_Node(NULL, NULL, NULL, Make_Hash_Str(cur_str), cur_str, VAR);

            }

            cur_com_num++;
            program_str += letter_num;
        }
        }
        array[cur_com_num] = Create_Node (NULL, NULL, NULL, END_LINE, "\n", END_LINE);
        cur_com_num++;
    }

    array[cur_com_num] = Create_Node (NULL, NULL, NULL, NULL, "", OPERATOR);
    cur_com_num++;

    free (cur_str);
    com_num = cur_com_num;

    return array;
}

