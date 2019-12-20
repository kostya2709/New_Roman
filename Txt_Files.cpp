#include "Front_Head.h"

const int ANSWER_STR = 2048;

char* cur_ptr = NULL;

int Find_Node_Char (Node& node1, char* sym);

void Insert_Tabs (FILE* f, int number)
{
    int i = 0;
    for (i = 0; i < number; i++)
        fprintf (f, "\t");
}



void File_Write_Cycle (Node* node1, FILE* f)
{
    fprintf (f, "(");

    switch ((int)node1->node_type)
    {
        case NUMBER:
        {
            fprintf (f, "%lg", node1->data);
            break;
        }
        case STRING:
        {
            fprintf (f, "\"%s\"", node1->sym);
            break;
        }
        case VAR:
        {
            if (node1->parent->node_type == CALL || (!strcmp (node1->parent->sym, "DEF")))
                fprintf (f, "%s", node1->sym);
            else fprintf (f, "@%d%s", (int)node1->data, node1->sym);
            break;
        }
        default:
        {
            fprintf (f, "%s", node1->sym);
            break;
        }
    }

    if ((node1->left == 0) && (node1->right))
        fprintf (f, "()");

    else if (node1->left)
        File_Write_Cycle (node1->left, f);


    if ((node1->right == 0) && (node1->left))
        fprintf (f, "()");

    else if (node1->right)
        File_Write_Cycle (node1->right, f);

    fprintf (f, ")");
}

int Tree::File_Write (char* file_name)
{
    FILE* f = fopen (file_name, "w");

    File_Write_Cycle (this->first_elem, f);

    fclose (f);

    return 0;
}


void File_Read_Cycle (Node* node1, Hash& hash)
{
    if (cur_ptr && *cur_ptr == '\0')
        return;

    int letter_num = 0;

    cur_ptr += 1;    ///Skips a brace

    char cur_sym = *cur_ptr;
    char* cur_str = (char*)calloc (MAX_VAR_NAME, 1);

    elem_t cur_val = 0;

    if (cur_sym == '@')
    {
        cur_ptr++;
        sscanf (cur_ptr, "%lf %n", &cur_val, &letter_num);
        cur_ptr += letter_num;
        sscanf (cur_ptr, "%[^()] %n", cur_str, &letter_num);
        cur_ptr += letter_num;
        Change_Node (*node1, cur_val, cur_str, VAR);
    }

    else if (isdigit (cur_sym))
    {
        sscanf (cur_ptr, "%lg %n", &cur_val, &letter_num);
        Change_Node (*node1, cur_val, "", NUMBER);
        printf ("num = %lg, let_num = %d\n", cur_val, letter_num);

        cur_ptr += letter_num;

    }

    else
    {
            bool is_string = 0;
            if (!Is_Symb (cur_sym))
                sscanf (cur_ptr, "%[^ ()] %n", cur_str, &letter_num);
            else if (cur_sym == '\"')
            {
                cur_ptr++;
                sscanf (cur_ptr, "%[^\"] %n", cur_str, &letter_num);
                letter_num += 1;              ///Skip letters in string and braces
                printf ("cur_str_str = \"%s\"\n", cur_str);
                is_string = 1;
            }
            else
            {
                sscanf (cur_ptr, "%1s %n", cur_str, &letter_num);
            }


            int64_t cur_hash = Make_Hash_Str (cur_str);
            int hash_ans = Find_Hash (hash.oper, OPERATOR_NUMBER, cur_hash);
            if (is_string)
                Change_Node (*node1, NULL, cur_str, STRING);
            else if (hash_ans != -1)
                Change_Node (*node1, hash_ans, cur_str, OPERATOR);
            else if (Find_Node_Char (*node1, cur_str))
                ;
            else Change_Node (*node1, cur_hash, cur_str, VAR);

            printf ("cur_str = \"%s\", num = %d, data = %lg, node_type = %d\n", cur_str, letter_num, node1->data, node1->node_type);

            cur_ptr += letter_num;
            free (cur_str);
    }

    if (*cur_ptr == '(' && *(cur_ptr + 1) == ')')
        cur_ptr++;
    else if (*cur_ptr == ')')
    {
        return;
    }
    else
    {
        Node* node_left = new Node();
        Insert_Node (node1, node_left, 0);
        File_Read_Cycle (node_left, hash);
    }

        cur_ptr++;

        if ((cur_ptr + 1) && *cur_ptr == '(' && *(cur_ptr + 1) == ')')
            cur_ptr++;
        else if (*cur_ptr == ')')
            return;
        else
        {
            Node* node_right = new Node();
            Insert_Node (node1, node_right, 1);
            File_Read_Cycle (node_right, hash);
        }
        if (*cur_ptr == ')')
            cur_ptr++;

}

int Tree::File_Read (char* file_name, Hash& hash)
{
    FILE* f = fopen (file_name, "r");

    char* string = (char*)calloc (1, ANSWER_STR);
    fread (string, 1, ANSWER_STR, f);
    cur_ptr = string;

    int letter_num = 0;

    Node* node0 = new Node();
    this->first_elem = node0;

    File_Read_Cycle (this->first_elem, hash);
    fclose (f);

    return 0;

}


int Find_Node_Char (Node& node1, char* sym)
{
    if (!strcmp (sym, "OP"))
        Change_Node (node1, NULL, "OP", LINK);
    else if (!strcmp (sym, "B"))
        Change_Node (node1, LINK, "B", LINK);
    else if (!strcmp (sym, "START"))
        Change_Node (node1, 27, "START", OPERATOR);
    else if (!strcmp (sym, "IF"))
        Change_Node (node1, IF, "IF", K_WORD);
    else if (!strcmp (sym, "WHILE"))
        Change_Node (node1, CYCLE, "WHILE", K_WORD);
    else if (!strcmp (sym, "MORE"))
        Change_Node (node1, MORE, "MORE", OPERATOR);
    else if (!strcmp (sym, "EMORE"))
        Change_Node (node1, EMORE, "EMORE", OPERATOR);
    else if (!strcmp (sym, "LESS"))
        Change_Node (node1, LESS, "LESS", OPERATOR);
    else if (!strcmp (sym, "ELESS"))
        Change_Node (node1, ELESS, "ELESS", OPERATOR);
    else if (!strcmp (sym, "EQUAL"))
        Change_Node (node1, EQUAL, "EQUAL", OPERATOR);
    else if (!strcmp (sym, "UNEQUAL"))
        Change_Node (node1, UNEQUAL, "UNEQUAL", OPERATOR);
    else if (!strcmp (sym, "C"))
        Change_Node (node1, NULL, "C", LINK);
    else if (!strcmp (sym, "D"))
        Change_Node (node1, LINK, "D", LINK);
    else if (!strcmp (sym, "DEF"))
        Change_Node (node1, NULL, "DEF", LINK);
    else if (!strcmp (sym, "CALL"))
        Change_Node (node1, NULL, "CALL", CALL);
    else if (!strcmp (sym, ","))
        Change_Node (node1, LINK, ",", LINK);
    else if (!strcmp (sym, "RETURN"))
        Change_Node (node1, RETURN, "RETURN", K_WORD);
    else if (!strcmp (sym, "OUTPUT"))
        Change_Node (node1, WRITE, "OUTPUT", K_WORD);
    else return 0;

    return 1;

}
