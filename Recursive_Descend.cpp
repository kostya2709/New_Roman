#include "Front_Head.h"

#define N_TYPE (*program_node)->node_type
#define N_DATA (*program_node)->data

static Hash hash;

void Check_End_Line ();

Node** program_node = NULL;

const char* str_ptr = NULL;

Node* Get_General (Node** prog_node);

Node* Get_Number (void);

Node* Get_Expr (void);

Node* Get_Top (void);

Node* Get_Pow (void);

Node* Get_If (void);

Node* Get_Pbrack (void);

Node* Func (char* oper);

Node* Get_Str (void);

Node* Get_Brackets(void);

Node* Get_Line (void);

Node* Get_Link (void);

Node* Get_Conditions (void);

Node* Get_One_Cond ();

Node* Get_General (Node** prog_node, Hash** hash1)
{
    program_node = prog_node;
    Node* result = Create_Node(NULL, Get_Brackets(), NULL, 27, "PROGRAM", OPERATOR);
    if ((*program_node)->data == NULL && (*program_node)->node_type == OPERATOR)
        ;
    else
    {
        printf ("Expected end of the program, received \"%s\"\n", (*program_node)->sym);
        abort();
    }
    *hash1 = &hash;
    return result;
}

Node* Get_Brackets()
{

    if (((*program_node)->data == BEGIN) && (*program_node)->node_type == K_WORD)
        program_node++;
    else
    {
        printf ("Expected \"%s\", received \"%s\".\n", key_words_str[BEGIN], (*program_node)->sym);
        abort();
    }

    Check_End_Line ();

    Node* temp = Get_Link();

    if (((*program_node)->data == END) && (*program_node)->node_type == K_WORD)
        program_node++;
    else
    {
        printf ("Expected \"%s\", received \"%s\".\n", key_words_str[END], (*program_node)->sym);
        abort();
    }

    Check_End_Line();

    for (int i =0; i < hash.var_amount; i++)
        printf ("%d %d\n", i, hash.var[i]);

    return temp;

}

Node* Get_Link ()
{
    Node* left = Get_Line();
    if ((*program_node)->data == END && (*program_node)->node_type == K_WORD)
        return Create_Node (left, NULL, NULL, LINK, "LINK", LINK);
    else
        return Create_Node (left, Get_Link(), NULL, LINK, "LINK", LINK);
}

Node* Get_Line()
{
    Node* temp = *program_node;

    switch ((*program_node)->node_type)
    {
        case VAR:
        {
            int var_num = Find_Hash (hash.var, hash.var_amount, (*program_node)->data);
            if (var_num == -1)
                var_num = hash.Add_Var ((*program_node)->data);

            program_node++;

            switch ((*program_node)->node_type)
            {
                case K_WORD:
                {
                    switch ((int)((*program_node)->data))
                    {
                        case ASSIGN:
                        {
                            program_node++;
                            temp = Create_Node (temp, Get_Expr(), NULL, ASSIGN, key_words_str[ASSIGN], K_WORD);
                            break;
                        }
                        case PRINT:
                        {
                            program_node++;
                            temp = Create_Node (temp, NULL, NULL, PRINT, key_words_str[PRINT], K_WORD);
                            break;
                        }
                        case READ:
                        {
                            program_node++;
                            temp = Create_Node (temp, NULL, NULL, READ, key_words_str[READ], K_WORD);
                            break;
                        }
                    }
                    break;
                }
                default:
                {
                    printf ("Expected a fuction depending on a variable. ");
                    printf ("Recieved \"%s\".\n", (*program_node)->sym);
                    exit(0);
                }

                break;
            }
            break;
        }
        case K_WORD:
        {
            if ((*program_node)->data == SUDDENLY)
                temp = Get_If();
        }
        break;
    }

    Check_End_Line();

    return temp;
}

Node* Get_Number (void)
{
    program_node++;
    return *(program_node - 1);
}

Node* Get_Expr (void)
{
    Node* res = Get_Top ();
    Node* res3 = res;
    while ((*program_node)->node_type == OPERATOR && ((*program_node)->data == ADD || (*program_node)->data == MIN))
    {
        Node* prev = *program_node;
        program_node++;
        Node* res2 = Get_Top();

        if (prev->data == ADD)
            res3 = Create_Node(res3, res2, NULL, ADD, "+", OPERATOR);
        else res3 = Create_Node(res3, res2, NULL, MIN, "-", OPERATOR);
    }

    return res3;
}

Node* Get_Top (void)
{
    Node* val = Get_Pow ();
    while (((*program_node)->data == MUL || (*program_node)->data == DIV)&&((*program_node)->node_type == OPERATOR))
    {
        Node* prev = *program_node;
        program_node++;
        Node* val2 = Get_Pow();

        if (prev->data == MUL)
            val = Create_Node (val, val2, NULL, MUL, "*", OPERATOR);
        else
            val = Create_Node (val, val2, NULL, DIV, "/", OPERATOR);
    }
assert (val);
    return val;
}


Node* Get_Pbrack (void)
{
    if (((*program_node)->node_type == K_WORD) && ((*program_node)->data == OPEN_BR))
    {
        program_node++;
        Node* val = Get_Expr();
    if (((*program_node)->node_type == K_WORD) && ((*program_node)->data == CLOSE_BR))
            ;
        else
        {
            printf ("Expected ')'!\n");
            system ("pause");
        }
        program_node++;
        return val;
    }
    else if ((*program_node)->node_type == NUMBER)
        return Get_Number();
    else
        Get_Str();

}

Node* Func ()
{
    Node* prev = *program_node;
    program_node++;

    if (!(((*program_node)->node_type == K_WORD) && ((*program_node)->data == OPEN_BR)))
    {
        printf ("Expected an opening bracket after function %s!\n", prev->sym);
        printf ("Recieved data = %lg, type = %d instead!\n", (*program_node)->data, (*program_node)->node_type);
        system ("pause");
    }
    program_node++;
    Node* val = Get_Expr();
    if (!(((*program_node)->node_type == K_WORD) && ((*program_node)->data == CLOSE_BR)))
    {
        printf ("Expected a closing bracket after function %s!\n", prev->sym);
        printf ("Recieved %s\n", (*program_node)->sym);
        system ("pause");
    }
    program_node++;

    return Create_Node (NULL, val, NULL, prev->data, prev->sym, OPERATOR);
}

Node* Get_Str (void)
{
    if ((*program_node)->node_type == OPERATOR)
                return Func();
    else
    if ((*program_node)->node_type == VAR)
        if (Find_Hash (hash.var, hash.var_amount, (*program_node)->data) != -1)
        {
            program_node++;
            return *(program_node - 1);
        }
        else
        {
            printf ("This citizen, \"%s\", was not born yet!\n", (*program_node)->sym);
            abort();
        }

}

Node* Get_Pow (void)
{
    Node* val = Get_Pbrack ();
    while ((*program_node)->node_type == OPERATOR && (*program_node)->data == POW)
    {
        program_node++;
        Node* val2 = Get_Pbrack();

        val = Create_Node (val, val2, NULL, POW, oper_str[POW], OPERATOR);
    }
assert (val);
    return val;
}

Node* Get_Conditions (void)
{
    if (((*program_node)->data == BEGIN) && (*program_node)->node_type == K_WORD)
        program_node++;
    else
    {
        printf ("Expected \"%s\", received \"%s\".\n", key_words_str[BEGIN], (*program_node)->sym);
        abort();
    }

    Check_End_Line ();

    Node* temp = Get_One_Cond();

    if (((*program_node)->data == END) && (*program_node)->node_type == K_WORD)
        program_node++;
    else
    {
        printf ("Expected \"%s\", received \"%s\".\n", key_words_str[END], (*program_node)->sym);
        abort();
    }

    return temp;
}

Node* Get_One_Cond ()
{
    if ((*program_node)->node_type != K_WORD)
    {
        printf ("Expected a condition, received \"%s\"\n", (*program_node)->sym);
        exit(0);
    }

    bool in_cycle = 0;

    if ((*program_node)->data == CYCLE)
    {
        in_cycle = 1;
        program_node++;
        Check_End_Line();

        if ((*program_node)->node_type != K_WORD)
        {
            printf ("Expected a condition, received \"%s\"\n", (*program_node)->sym);
            exit(0);
        }
    }

    Node* temp = NULL;

    Node* temp2 = NULL;

    Node* cyc = NULL;

    switch ((int)((*program_node)->data))
    {
        case (MORE):
        {
            temp = Create_Node (NULL, NULL, NULL, MORE, "A", OPERATOR);
            break;
        }
        case (EMORE):
        {
            temp = Create_Node (NULL, NULL, NULL, EMORE, "AE", OPERATOR);
            break;
        }
        case (LESS):
        {
            temp = Create_Node (NULL, NULL, NULL, LESS, "L", OPERATOR);
            break;
        }
        case (ELESS):
        {
            temp = Create_Node (NULL, NULL, NULL, ELESS, "LE", OPERATOR);
            break;
        }
        case (EQUAL):
        {
            temp = Create_Node (NULL, NULL, NULL, EQUAL, "E", OPERATOR);
            break;
        }
        case (UNEQUAL):
        {
            temp = Create_Node (NULL, NULL, NULL, UNEQUAL, "UE", OPERATOR);
            break;
        }
        case (ELSE):
        {
            temp = Create_Node (NULL, NULL, NULL, ELSE, "ELSE", OPERATOR);
            break;
        }
        default:
        {
            printf ("Unexpected type of condition, received \"%s\".\n", (*program_node)->sym);
            break;
        }
    }
    program_node++;

    if ((*program_node)->node_type == END_LINE)
    {
        program_node++;
        temp2 = Get_Brackets();
    }
    else
    {
        temp2 = Get_Line();
        Node* temp_temp = Create_Node (temp2, NULL, NULL, LINK, "LINK", LINK);
        temp2 = temp_temp;
    }

    temp->left = temp2;
    temp2->parent = temp;

    if ((*program_node)->node_type == K_WORD && (*program_node)->data == END)
    {
        if (in_cycle)
            return Create_Node (temp, NULL, NULL, CYCLE, "WHILE", K_WORD);

        return temp;
    }
    else
    {
        Node* temp3 = Get_One_Cond();

        if (in_cycle)
        {
            cyc = Create_Node (temp, NULL, NULL, CYCLE, "WHILE", K_WORD);
            cyc->right = temp3;
            temp3->parent = cyc;
            return cyc;
        }

        temp->right = temp3;
        temp3->parent = temp;

        return temp;
    }
}

Node* Get_If (void)
{

    program_node++;
    Node* left = Get_Expr();
    if ((*program_node)->node_type == K_WORD && (*program_node)->data == INVADE)
            ;
    else
    {
        printf ("Expected comparison using \"%s\", received \"%s\"\n", key_words_str[INVADE], (*program_node)->sym);
        exit(0);
    }
    program_node++;

    Node* right = Get_Expr();

    Check_End_Line();

    Node* temp = Create_Node(left, right, NULL, INVADE, "SLINK", LINK);

    temp = Create_Node (temp, Get_Conditions(), NULL, INVADE, "SLINK", LINK);

    return temp;
}

void Check_End_Line ()
{
    if ((*program_node)->node_type == END_LINE)
        program_node++;
    else
    {
        printf ("Expected \"\\n\" as a line terminating symbol, received \"%s\"\n", (*program_node)->sym);
        abort();
    }
}
