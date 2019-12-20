#include "Front_Head.h"

void Frt_End_Cycle(Node* start);

void Frt_Call_Func (Node* start);

void Frt_Func (Node* start, int comp);

void Call_Func (Node* start);

static FILE* f;

static Hash* hash;

static int comp1 = 0;

void Insert_Tabs (FILE* f, int number)
{
    int i = 0;
    for (i = 0; i < number; i++)
        fprintf (f, "\t");
}

void Frt_Expr (Node* start)
{
    if (start->left)
        if (start->node_type != CALL)
            Frt_Expr (start->left);

    if (start->node_type == CALL)
        Frt_Call_Func (start);

    if (start->node_type == NUMBER)
        fprintf (f, "%.lf", start->data);

    if (start->node_type == OPERATOR)
        switch ((int)start->data)
        {
            case ADD:
            {
                fprintf (f, " + ");
                break;
            }
            case MIN:
            {
                fprintf (f, " - ");
                break;
            }
            case MUL:
            {
                fprintf (f, " * ");
                break;
            }
            case DIV:
            {
                fprintf (f, " / ");
                break;
            }
            case SQRT:
            {
                fprintf (f, "sqrt ");
                break;
            }
        }
        else if (start->node_type == VAR)
            fprintf (f, "%s", start->sym);

    if (start->right)
        if (start->node_type != CALL)
            Frt_Expr (start->right);
}

void Frt_Print (Node* start)
{
    int var = start->right->data;
    fprintf (f, "%s выступает_в_сенате\n", start->right->sym);
}

void Frt_Read (Node* start)
{
    int var = start->right->data;
    fprintf (f, "%s познает_волю_богов\n", start->right->sym);
}

void Frt_One_Cond (Node* start, int comp)
{
    Node* left = start->left->left;
    Node* right = start->left->right;

    Node* condi = start->left;


    if (start->data == CYCLE)
        fprintf (f, "%s\n", key_words_str [CYCLE]);

    fprintf (f, "%s:\n", key_words_str [(int)(condi->data)]);

    int temp_comp = comp;

    if (strcmp (start->right->sym, "C")== 0)
        Frt_Func (start->right->right, comp1);
    else Frt_Func (start->right, comp1);


    if (strcmp (start->right->sym, "C") == 0)
    {
        if (start->right->left->right->right->node_type == OPERATOR)
        {
            comp1++;
            Frt_One_Cond(start->right->left->right->right, comp1);
        }
        else
            Frt_Func (start->right->left->right, comp1);
    }
}

void Frt_Invade (Node* start, int comp)
{
    comp1++;
    int temp = comp1;
    fprintf (f, "%s ", key_words_str [SUDDENLY]);
    Frt_Expr (start->left->left);
    fprintf (f, " %s ", key_words_str [INVADE]);
    Frt_Expr (start->left->right);
    fprintf (f, "\n");
    fprintf (f, "%s\n", key_words_str [BEGIN]);
    Frt_One_Cond (start, comp1);

    fprintf (f, "%s\n", key_words_str [END]);
}

void Frt_Assign (Node* start)
{
    int var_num = start->left->data;

    fprintf (f, "%s имеет_силу ", start->left->sym);
    Frt_Expr (start->right);
    fprintf (f, "\n");

}

void Frt_Return (Node* start)
{
    fprintf (f, "В_Рим_возвращается ");
    Frt_Expr (start->right);
    fprintf (f, "\n");
}

void Frt_Call_Args (Node* start)
{
    Frt_Expr (start->right);
    if (start->left)
        Frt_Call_Args (start->left);
}

void Frt_Call_Func (Node* start)
{
    char* func = start->right->sym;

    fprintf (f, "%s (", func);

    if (start->left)
        Frt_Call_Args (start->left);
    fprintf (f, ")");
}

void Frt_Func (Node* start, int comp)
{
    assert (start);

    if (strcmp (start->sym, "B") == 0)
    {
        fprintf (f, "AVE_MARIA!\n\n");
        Frt_Func (start->right, comp);
        fprintf (f, "\nDEUS_VULT!\n");
    }
    if (start->right == 0)
            return;
    if (start->right->node_type == CALL)
        Frt_Call_Func (start->right);
    else switch ((int)(start->right->data))
    {
        case ASSIGN:
        {
            Frt_Assign (start->right);
            break;
        }
        case PRINT:
        {
            Frt_Print (start->right);
            break;
        }
        case READ:
        {
            Frt_Read(start->right);
            break;
        }
        case CYCLE:
        case IF:
        {
            Frt_Invade (start->right, comp);
            break;
        }
        case RETURN:
        {
            Frt_Return (start->right);
            break;
        }
        case WRITE:
        {
            fprintf (f, "\noutf \"%s\"\n\n", start->right->right->sym);
            break;
        }
    }

    fprintf (f, "\n");

    if (start->left)
        Frt_Func (start->left, comp);

}

void Frt_Func_Args (Node* start)
{
    if (start->left)
        Frt_Func_Args (start->left);

    fprintf (f, "%s", start->right->sym);
}

void Frt_Func_Up (Node* start, int comp)
{
    fprintf (f, "%s (", start->right->sym);

    if (start->left)
        Frt_Func_Args (start->left);

    fprintf (f, ")\n");

    if (start->right)
    {
        Frt_Func (start->right->right, comp);
    }

}

void Frt_End_Cycle (Node* start, int comp)
{
    switch ((int)(start->right->node_type))
    {
        case K_WORD:                    // Global assignation
        {
            Frt_Assign (start->right);
            break;
        }
        case LINK:                      // Function declaration
        {
            Frt_Func_Up (start->right, comp);
            break;
        }
    }

    fprintf (f, "\n");

    if (start->left)
        Frt_End_Cycle (start->left, comp);
}


void Frt_End (Node* start, Hash* hash1, int comp)
{
    f = fopen ("From Frontend-1 with love.txt", "w");
    hash = hash1;

    int randd = rand()%1000 + 1000;

    Frt_End_Cycle (start->right, comp);


    fclose (f);
}

