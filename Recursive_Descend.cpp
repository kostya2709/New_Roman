#include "Front_Head.h"

#define N_TYPE (*program_node)->node_type
#define N_DATA (*program_node)->data

static Hash hash;

int min_var = 0;

int func_counter = 0;

int cur_arg_num = 0;

void Print_Info (Node* node);

void Print_Vars (void);

void Print_Funcs (void);

Stack_t var_stk = {};

void Check_End_Line (void);

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

Node* Get_Brackets ();

Node* Get_Line ();

Node* Get_Link ();

Node* Get_Conditions (Node* var_left, Node* var_right);

Node* Get_One_Cond (Node* var_left, Node* var_right);

Node* Get_Program (void);

Node* Get_Dec (void);

Node* Get_Global (void);

Node* Get_Func_Args (int& num);

Node* Get_Return (void);

Node* Get_Call (void);

Node* Get_String (void);

Node* Get_Call_Args (int& num);

Node* Get_General (Node** prog_node, Hash** hash1)
{
    program_node = prog_node;

    Stack_Construct (&var_stk);

    Node* result = Create_Node(NULL, Get_Program(), NULL, 27, "START", OPERATOR);
    if ((*program_node)->data == NULL && (*program_node)->node_type == OPERATOR)
        ;
    else
    {
        printf ("Expected end of the program, received \"%s\"\n", (*program_node)->sym);
        abort();
    }
    *hash1 = &hash;

    Stack_Destruct (&var_stk);

    return result;
}

Node* Get_Brackets ()
{

    if (((*program_node)->data == BEGIN) && (*program_node)->node_type == K_WORD)
        program_node++;
    else
    {
        printf ("Expected \"%s\", received \"%s\".\n", key_words_str[BEGIN], (*program_node)->sym);
        abort();
    }

    Check_End_Line ();

    Stack_Push (&var_stk, hash.var_amount);

    Node* temp = Get_Link ();
    sprintf (temp->sym, "OP");
    Node* bracket = Create_Node (NULL, temp, NULL, NULL, "B", LINK);

    hash.var_amount = Stack_Pop (&var_stk);

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

    return bracket;

}

Node* Get_Link ()
{
    Node* right = Get_Line ();
    if ((*program_node)->data == END && (*program_node)->node_type == K_WORD)
        return Create_Node (NULL, right, NULL, LINK, "OP", LINK);
    else
        return Create_Node (Get_Link(), right, NULL, LINK, "OP", LINK);
}

Node* Get_Line ()
{

    Node* temp = *program_node;

    switch ((*program_node)->node_type)
    {
        case VAR:
        {
            printf ("var %s, %lg\n", (*program_node)->sym, (*program_node)->data);

            int64_t var_num = Find_Hash (hash.funcs, hash.funcs_num, (*program_node)->data);
            if (var_num != -1)
                ;
            else
            {
                var_num = Find_Hash (hash.var, hash.var_amount, (*program_node)->data);
                if (var_num == -1)
                    var_num = hash.Add_Var ((*program_node)->data) - 1;
                if (var_num < hash.last_global)
                    (*program_node)->data = var_num;
                else
                    (*program_node)->data = var_num - hash.last_global;
            }

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
                            temp = Create_Node (temp, Get_Expr (), NULL, ASSIGN, "\'=\'", K_WORD);
                            break;
                        }
                        case PRINT:
                        {
                            program_node++;
                            temp = Create_Node (NULL, temp, NULL, PRINT, "OUTPUT", K_WORD);
                            break;
                        }
                        case READ:
                        {
                            program_node++;
                            temp = Create_Node (NULL, temp, NULL, READ, "INPUT", K_WORD);
                            break;
                        }
                        case OPEN_BR:
                        {
                            temp = Get_Call();
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
            switch ((int)(*program_node)->data)
            {
                case (SUDDENLY):
                {
                    temp = Get_If();
                    break;
                }
                case (RETURN):
                {
                    temp = Get_Return();
                    break;
                }
                case (WRITE):
                {
                    temp = Get_String();
                    break;
                }
            }



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
        return Get_Str();

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
    {
        int64_t var_num = Find_Hash (hash.funcs, hash.funcs_num, (*program_node)->data);
        if (var_num != -1)
        {
            program_node++;
            return Get_Call();
        }

        int64_t cur_hash = Make_Hash_Str ((*program_node)->sym);
        cur_hash = Find_Hash (hash.var, hash.var_amount, cur_hash);
        if (cur_hash != -1)
        {
            if (cur_hash < hash.last_global)
                (*program_node)->data = cur_hash;
            else
                (*program_node)->data = cur_hash - hash.last_global;
            program_node++;
            return *(program_node - 1);
        }
        else
        {
            printf ("This citizen, \"%s\", was not born yet!\n", (*program_node)->sym);
            abort();
        }
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

Node* Get_Conditions (Node* var_left, Node* var_right)
{
    if (((*program_node)->data == BEGIN) && (*program_node)->node_type == K_WORD)
        program_node++;
    else
    {
        printf ("Expected \"%s\", received \"%s\".\n", key_words_str[BEGIN], (*program_node)->sym);
        abort();
    }

    Check_End_Line ();

    Node* temp = Get_One_Cond (var_left, var_right);
    //if (temp->node_type)

    if (((*program_node)->data == END) && (*program_node)->node_type == K_WORD)
        program_node++;
    else
    {
        printf ("Expected \"%s\", received \"%s\".\n", key_words_str[END], (*program_node)->sym);
        abort();
    }

    return temp;
}

Node* Get_One_Cond (Node* var_left, Node* var_right)
{
    if ((*program_node)->node_type != K_WORD)
    {
        printf ("Expected a condition, received \"%s\"\n", (*program_node)->sym);
        exit(0);
    }

    Node* if_node = Create_Node (IF, "IF", K_WORD);

    if ((*program_node)->data == CYCLE)
    {
        program_node++;
        Check_End_Line();
        if_node = Create_Node (CYCLE, "WHILE", K_WORD);

        if ((*program_node)->node_type != K_WORD)
        {
            printf ("Expected a condition, received \"%s\"\n", (*program_node)->sym);
            exit(0);
        }
    }

    Node* cond = NULL;

    Node* temp2 = NULL;

    Node* cyc = NULL;

    if ((*program_node)->data == ELSE)
    {
        program_node++;
        if ((*program_node)->node_type == END_LINE)
        {
            program_node++;
            temp2 = Get_Brackets();
            temp2 = temp2->right;
        }
        else
            temp2 = Get_Line();

        return temp2;
    }

    switch ((int)((*program_node)->data))
    {
        case (MORE):
        {
            cond = Create_Node (MORE, "MORE", OPERATOR);
            break;
        }
        case (EMORE):
        {
            cond = Create_Node (EMORE, "EMORE", OPERATOR);
            break;
        }
        case (LESS):
        {
            cond = Create_Node (LESS, "LESS", OPERATOR);
            break;
        }
        case (ELESS):
        {
            cond = Create_Node (ELESS, "ELESS", OPERATOR);
            break;
        }
        case (EQUAL):
        {
            cond = Create_Node (EQUAL, "EQUAL", OPERATOR);
            break;
        }
        case (UNEQUAL):
        {
            cond = Create_Node (UNEQUAL, "UNEQUAL", OPERATOR);
            break;
        }
        default:
        {
            printf ("Unexpected type of condition, received \"%s\".\n", (*program_node)->sym);
            break;
        }
    }

    Insert_Node (if_node, cond, 0);
    Insert_Node (cond, var_left, 0);
    Insert_Node (cond, var_right, 1);

    program_node++;

    if ((*program_node)->node_type == END_LINE)
    {
        program_node++;
        temp2 = Get_Brackets();
    }
    else
    {
        temp2 = Get_Line();
        Node* temp_temp = Create_Node (LINK, "B", LINK);
        Node* oper = Create_Node (NULL, "OP", LINK);
        Insert_Node (temp_temp, oper, 1);
        Insert_Node (oper, temp2, 1);
        temp2 = temp_temp;
    }


    if ((*program_node)->node_type == K_WORD && (*program_node)->data == END)
    {
        Insert_Node (if_node, temp2, 1);
        return if_node;
    }
    else
    {
        Node* temp3 = Get_One_Cond (Copy_Node (var_left), Copy_Node (var_right));
        int is_else = 0;
        if (temp3->node_type == LINK) is_else = 1;
        Node* connect = Create_Node (NULL, "C", LINK);
        Node* bracket = Create_Node (NULL, "B", LINK);
        if (!is_else)
        {
            Node* oper = Create_Node (NULL, "OP", LINK);
            Insert_Node (bracket, oper, 1);
            Insert_Node (oper, temp3, 1);
        }
        else
        {
            Insert_Node (bracket, temp3, 1);
        }

        Insert_Node (connect, bracket, 0);
        Insert_Node (if_node, connect, 1);
        Insert_Node (connect, temp2, 1);

        return if_node;
    }
}

Node* Get_If (void)
{

    program_node++;
    Node* var_left = Get_Expr();
    if ((*program_node)->node_type == K_WORD && (*program_node)->data == INVADE)
            ;
    else
    {
        printf ("Expected comparison using \"%s\", received \"%s\"\n", key_words_str[INVADE], (*program_node)->sym);
        exit(0);
    }
    program_node++;

    Node* var_right = Get_Expr();

    Check_End_Line();

    Node* temp = Get_Conditions (var_left, var_right);

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

Node* Get_Program (void)
{
    if (N_TYPE != VAR)
    {
        printf ("Expected a declaration of a function or a global variable\n");
        printf ("Received \"%s\"\n", (*program_node)->sym);
        exit (0);
    }

    Node* temp = NULL;
    Node* l_node = Create_Node (LINK, "D", LINK);

    if ((*(program_node + 1))->node_type == K_WORD
            && (*(program_node + 1))->data == OPEN_BR)
    {
        temp = Get_Dec();
        func_counter++;
        hash.var_amount = hash.last_global;
    }
    else
        temp = Get_Global();

    Insert_Node (l_node, temp, 1);

    if ((*program_node)->data == NULL && (*program_node)->node_type == OPERATOR)
        ;
    else
    {
        Node* temp2 = Get_Program ();
        Insert_Node (l_node, temp2, 0);
    }

    return l_node;
}

Node* Get_Dec()
{
    Node* f_node = *program_node;
    f_node->node_type = LINK;

    int64_t temp_h = Make_Hash_Str ((*program_node)->sym);
    int64_t pos = Find_Hash (hash.funcs, hash.funcs_num, temp_h);
    if (pos != -1)
    {
        printf ("This function \"%s\" was already declared in this scope!", (*program_node)->sym);
        exit (0);
    }
    hash.Add_Func (temp_h);

    program_node += 2;  //Jumped over '('
    int arg_num = 0;

    Node* result = Create_Node (NULL, "DEF", LINK);

    if (N_TYPE == K_WORD && N_DATA == CLOSE_BR)
        program_node++;
    else
    {
        arg_num = 1;
        Node* left = Get_Func_Args (arg_num);
        Insert_Node (result, left, 0);
    }

    hash.args[hash.funcs_num - 1] = arg_num;

    cur_arg_num = 0;

    Check_End_Line();

    Insert_Node (result, f_node, 1);

    Insert_Node (f_node, Get_Brackets (), 1);

    return result;

}

Node* Get_Global()
{
    Node* l_node = Create_Node (ASSIGN, "assign", K_WORD);


    int64_t var_num = Find_Hash (hash.var, hash.var_amount, (*program_node)->data);
    if (var_num == -1)
        {
            var_num = hash.Add_Var ((*program_node)->data) - 1;
            hash.last_global += 1;
        }
    (*program_node)->data = var_num;
    Print_Info (*program_node);

    Insert_Node (l_node, *program_node, 0);
    program_node++;

    if ((N_TYPE == K_WORD) && (N_DATA == ASSIGN))
        ;
    else
    {
        printf ("Expected assignation of a global value \"%s\"\n", (*(program_node - 1))->sym);
        printf ("Received \"%s\"\n", (*program_node)->sym);
        exit (0);
    }
    program_node++;

    Insert_Node (l_node, Get_Expr(), 1);

    Check_End_Line();

    return l_node;
}

Node* Get_Call()
{
    char* func_name_ptr = (*(program_node - 1))->sym;
    int64_t func_hash = (*(program_node - 1))->data;
    int min_var = hash.var_amount;

    Node* call = Create_Node (min_var, "CALL", CALL);
    Node* func = Create_Node (min_var, (*(program_node - 1))->sym, VAR);
    Insert_Node (call, func, 1);

    int64_t found = Find_Hash (hash.funcs, hash.funcs_num, func_hash);
    if (found == -1)
    {
        printf ("Function \"%s\" was not declared in this scope, %lg\n", (*(program_node - 1))->sym, (*(program_node - 1))->data);
        Print_Funcs();
        exit (0);
    }
    program_node++;

    int cur_arg = 0;

    if (N_TYPE == K_WORD && N_DATA == CLOSE_BR)
        program_node++;
    else
        {
            Insert_Node (call, Get_Call_Args (cur_arg), 0);
        }

    if (cur_arg != hash.args[found])
    {
        printf ("Invalid number of arguments in function \"%s\".\n", func_name_ptr);
        printf ("Expected %d arguments, received %d arguments.\n", hash.args[found]);
        exit (0);
    }

    return call;
}

Node* Get_Call_Args (int& args_num)
{
    Node* link = Create_Node (LINK, ",", LINK);
    Node* arg = Get_Expr();
    Insert_Node (link, arg, 1);
    args_num++;

    if (N_TYPE == K_WORD && N_DATA == CLOSE_BR)
    {
        program_node++;
        return link;
    }
    else if (N_TYPE == K_WORD && N_DATA == COMMA)
    {
        program_node++;
        Insert_Node (link, Get_Call_Args (args_num), 0);
        return link;
    }
    else
    {
        printf ("Expected \",\" to enumerate arguments or \")\"to finish argument enumeration.\n");
        printf ("Received \"%s\".\n", (*program_node)->sym);
        exit (0);
    }
}

Node* Get_Func_Args(int& num)
{
    Node* l_node = Create_Node (LINK, ",", LINK);

    if (N_TYPE == VAR)
        ;
    else
    {
        printf ("Expected an argument in function declaration.\n");
        printf ("Received \"%s\"\n", (*program_node)->sym);
        exit (0);
    }

    int64_t cur_hash = Make_Hash_Str ((*program_node)->sym);
    cur_hash = Find_Hash (hash.var, hash.var_amount, cur_hash);
    if (cur_hash != -1)
    {
        printf ("Name of a global variable and names of function arguments cannot coincide!\n");
        printf ("In declaration received \"%s\".\n", (*program_node)->sym);
        exit (0);
    }
        else
        {
            (*program_node)->data = cur_arg_num;
            hash.Add_Var (Make_Hash_Str ((*program_node)->sym));
            cur_arg_num++;
        }

    Insert_Node (l_node, *program_node, 1);
    program_node++;

    if (N_DATA == COMMA && N_TYPE == K_WORD)
    {
        num += 1;
        program_node++;
        Insert_Node (l_node, Get_Func_Args (num), 0);
        program_node--;
    }
    if (N_DATA == CLOSE_BR && N_TYPE == K_WORD)
    {
        program_node++;
        return l_node;
    }
    else
    {
        printf ("Expected an argument or a closing bracket in function declaration.\n");
        printf ("Received \"%s\"\n", (*program_node)->sym);
        exit (0);
    }
}

Node* Get_Return ()
{
    Node* temp = (*program_node);
    sprintf (temp->sym, "RETURN");
    program_node++;

    Node* expr = Get_Expr();
    Insert_Node(temp, expr, 1);

    return temp;
}

void Print_Funcs (void)
{
    printf ("there are %d funcs\n", hash.funcs_num);
    for (int i = 0; i < hash.funcs_num; i++)
        printf ("%d %lld args %d\n", i, hash.funcs[i], hash.args[i]);
    return;
}

void Print_Info (Node* node)
{
    printf ("\ndata = %lg\nsym = \"%s\"\ntype=\"%d\"\nhash=\"%lld\"\n\n", node->data, node->sym, node->node_type, Make_Hash_Str((*program_node)->sym));
}

void Print_Vars (void)
{
    for (int i = 0; i < hash.var_amount; i++)
        printf ("%d %lld\n", i, hash.var[i]);
}

Node* Get_String ()
{
    Node* n_write = *program_node;
    sprintf (n_write->sym, "OUTPUT");
    program_node++;

    if ((*program_node)->node_type != STRING)
    {
        printf ("Expected a string after \"%s\".\n", (*(program_node - 1))->sym);
        printf ("Received \"%s\"\n", (*program_node)->sym);
        exit (0);
    }

    Insert_Node (n_write, *program_node, 1);
    program_node++;

    return n_write;
}
