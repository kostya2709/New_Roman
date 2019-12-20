/*#include "Front_Head.h"

void Frt_End_Cycle(Node* start);

void Frt_Call_Func (Node* start);

void Frt_Func (Node* start, int comp);

void Call_Func (Node* start);

FILE* f;

Hash* hash;

int comp1 = 0;

void Frt_Expr (Node* start)
{
    if (start->left)
        if (start->node_type != CALL)
            Frt_Expr (start->left);

    if (start->right)
        if (start->node_type != CALL)
            Frt_Expr (start->right);

    if (start->node_type == CALL)
        Frt_Call_Func (start);

    if (start->node_type == NUMBER)
        fprintf (f, "push %.lf\n", start->data);

    if (start->node_type == OPERATOR)
        switch ((int)start->data)
        {
            case ADD:
            {
                fprintf (f, "add\n");
                break;
            }
            case MIN:
            {
                fprintf (f, "min\n");
                break;
            }
            case MUL:
            {
                fprintf (f, "mul\n");
                break;
            }
            case DIV:
            {
                fprintf (f, "div\n");
                break;
            }
            case SQRT:
            {
                fprintf (f, "sqrt\n");
                break;
            }
        }
        else if (start->node_type == VAR)
            fprintf (f, "push [ax+%lg]\n", start->data);
}

void Frt_Print (Node* start)
{
    int var = start->right->data;
    fprintf (f, "push [ax+%d]\n", var);
    fprintf (f, "out\n");
}

void Frt_Read (Node* start)
{
    int var = start->right->data;
    fprintf (f, "in\n");
    fprintf (f, "pop [ax+%d]\n", var);
}

void Frt_One_Cond (Node* start, int comp)
{
    Node* left = start->left->left;
    Node* right = start->left->right;

    Node* condi = start->left;

    int in_while = 0;

    if (start->data == CYCLE)
    {
        in_while = 1;
        fprintf (f, "while_comp%d_back:\n", comp);
    }

    Frt_Expr (left);
    Frt_Expr (right);


    switch ((int)(condi->data))
    {
        case MORE:
        {
            fprintf (f, "ja ");
            break;
        }
        case EMORE:
        {
            fprintf (f, "jae ");
            break;
        }
        case LESS:
        {
            fprintf (f, "jl ");
            break;
        }
        case ELESS:
        {
            fprintf (f, "jle ");
            break;
        }
        case EQUAL:
        {
            fprintf (f, "je ");
            break;
        }
        case UNEQUAL:
        {
            fprintf (f, "jne ");
            break;
        }
    }

    fprintf (f, "comp%d:\n", comp);
    fprintf (f, "jmp comp%d_else:\n", comp);
    fprintf (f, "comp%d:\n", comp);

    int temp_comp = comp;

    if (strcmp (start->right->sym, "C")== 0)
        Frt_Func (start->right->right, comp1);
    else Frt_Func (start->right, comp1);

    if (in_while)
        fprintf (f, "jmp while_comp%d_back:\n", temp_comp);


    if (!in_while) fprintf (f, "jmp comp%d_out:\n", comp);
    fprintf (f, "comp%d_else:\n\n", temp_comp);

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
    Frt_One_Cond (start, comp1);

    fprintf (f, "comp%d_out:\n\n\n", temp);
}

void Frt_Assign (Node* start)
{
    int var_num = start->left->data;

    Frt_Expr (start->right);
    fprintf (f, "pop [ax+%d]\n", var_num);

}

void Frt_Return (Node* start)
{
    Frt_Expr (start->right);
    fprintf (f, "\npush ax\n");
    fprintf (f, "push %d\n", MAX_VAR_IN_FUNC);
    fprintf (f, "min\n");
    fprintf (f, "pop ax\n");
    fprintf (f, "ret\n\n");
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

    if (start->left)
        Frt_Call_Args (start->left);

    fprintf (f, "push ax\n");
    fprintf (f, "push %d\n", MAX_VAR_IN_FUNC);
    fprintf (f, "add\n");
    fprintf (f, "pop ax\n");

    fprintf (f, "call %s:\n\n", func);

}

void Frt_Func (Node* start, int comp)
{
    assert (start);

    if (strcmp (start->sym, "B") == 0)
        Frt_Func (start->right, comp);
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

    fprintf (f, "pop [ax+%d]\n", (int)start->right->data);
}

void Frt_Func_Up (Node* start, int comp)
{
    fprintf (f, "%s:\n", start->right->sym);

    if (start->left)
        Frt_Func_Args (start->left);

    fprintf (f, "\n");

    if (start->right)
    {
        Frt_Func (start->right->right, comp);
    }

}

void Back_End_Cycle (Node* start, int comp)
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
    f = fopen (file_asm, "w");
    hash = hash1;

    fprintf (f, "call %s:\n", MAIN);
    int randd = rand()%1000 + 1000;
    fprintf (f, "jmp end%d:\n\n\n", randd);

    Frt_End_Cycle (start->right, comp);

    fprintf (f, "end%d:\n", randd);
    fprintf (f, "end");

    fclose (f);
}

*/
