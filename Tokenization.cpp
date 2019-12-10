#include "Front_Head.h"



Node** Tokenization (const char* program_str, int& com_num)
{
    Hash hash;

    Node** array = (Node**)calloc (sizeof (Node*), MAX_VAR_NAME);
    int cur_com_num = 0;

    char cur_sym = *program_str;
    double cur_val = 0;
    int letter_num = 0;
    char* cur_str = (char*)calloc (1, MAX_VAR_NAME);

    printf ("stroka = %s\n", program_str);

    while (*program_str)
    {
        cur_sym = *program_str;
printf ("sym = %c\n", *program_str);

        if (cur_sym == ' ' || cur_sym == '\t')
        {
            program_str++;
            printf ("SPACE\n");
            continue;
        }

        if (isdigit (cur_sym) || (cur_sym == '-'))
        {
            sscanf (program_str, "%lf %n", &cur_val, &letter_num);
            array[cur_com_num] = Create_Node(NULL, NULL, NULL, cur_val, "", NUMBER);
            cur_com_num++;
            printf ("num = %lg, let_num = %d\n", cur_val, letter_num);

            program_str += letter_num;

            if (*(program_str - 1) == '\n')
            {
                array[cur_com_num] = Create_Node (NULL, NULL, NULL, 0, "\\n", END_LINE);
                cur_com_num++;
            }

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

            if (*(program_str - 1) == '\n')
            {
                array[cur_com_num] = Create_Node (NULL, NULL, NULL, 0, "\\n", END_LINE);
                cur_com_num++;
            }
        }

    }

    array[cur_com_num] = Create_Node (NULL, NULL, NULL, NULL, "", OPERATOR);
    cur_com_num++;

    free (cur_str);
    com_num = cur_com_num;

    return array;
}

