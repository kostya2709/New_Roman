#include "Front_Head.h"

void Back_End_Cycle(Node* start);

void Push_Func (Node* start);

char* file_asm = "Test_Lang.txt";

FILE* f;

Hash* hash;


int comp = 0;
int cond = 0;

void Push_Expr (Node* start)
{
    if (start->left)
        Push_Expr (start->left);

    if (start->right)
        Push_Expr (start->right);

    if (start->node_type == NUMBER)
        fprintf (f, "push %lg\n", start->data);

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
        }
        else if (start->node_type == VAR)
            fprintf (f, "push [%lg]\n", start->data);
}

void Push_Print (Node* start)
{
    int var = start->left->data;
    fprintf (f, "push [%d]\n", var);
    fprintf (f, "out\n");
}

void Push_Read (Node* start)
{
    int var = start->left->data;
    fprintf (f, "in\n");
    fprintf (f, "pop [%d]\n", var);
}

void Push_One_Cond (Node* start, Node* left, Node* right)
{
    int in_while = 0;

    if (start->data != ELSE)
    {
        if (start->data == CYCLE)
        {
            in_while = 1;
            fprintf (f, "while_comp%d_back:\n", comp);
            start = start->left;
        }

        Push_Expr (left);
        Push_Expr (right);
    }

    switch ((int)(start->data))
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
            fprintf (f, "jel ");
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
        case ELSE:
        {

            break;
        }

    }

    if (start->data != ELSE)
    {
        fprintf (f, "comp%d_cond%d:\n", comp, cond);
        fprintf (f, "jmp comp%d_cond%d_else:\n", comp, cond);
        fprintf (f, "comp%d_cond%d:\n", comp, cond);
    }

    Push_Func (start->left);

    if (in_while)
        fprintf (f, "jmp while_comp%d_back:\n", comp);

    if (start->data != ELSE)
    {
        if (!in_while) fprintf (f, "jmp comp%d_out:\n", comp);
        fprintf (f, "comp%d_cond%d_else:\n\n", comp, cond);
    }

    cond++;

    if (in_while)
        if (start->parent->right)
            Push_One_Cond(start->parent->right, left, right);

    if (start->right && !(in_while))
        Push_One_Cond(start->right, left, right);
}

void Push_Invade (Node* start)
{
    Node* left_expr = start->left->left;
    Node* right_expr = start->left->right;

    Push_One_Cond (start->right, left_expr, right_expr);

    fprintf (f, "comp%d_out:\n\n\n", comp);
    cond = 0;
    comp++;
}

void Push_Assign (Node* start)
{
    int var_num = start->left->data;

    Push_Expr(start->right);

    fprintf (f, "pop [%d]\n", var_num);
}

void Push_Func_Args (Node* start)
{

}

void Push_Return (Node* start)
{
    Push_Expr (start->left);
    fprintf (f, "ret\n\n");
}

void Push_Func (Node* start)
{
    if (start->left == 0)
        return;

    switch ((int)(start->left->data))
    {
        case ASSIGN:
        {
            Push_Assign (start->left);
            break;
        }
        case PRINT:
        {
            Push_Print (start->left);
            break;
        }
        case READ:
        {
            Push_Read(start->left);
            break;
        }
        case INVADE:
        {
            Push_Invade (start->left);
            break;
        }
        case RETURN:
        {
            Push_Return (start->left);
            break;
        }
    }

    fprintf (f, "\n");

    if (start->right)
        Push_Func (start->right);

}

void Push_Func_Up (Node* start)
{

    fprintf (f, "%s:\n", start->sym);

    if (start->left) printf ("ARR\n");
        //Push_Func_Args (start->left);

    if (start->right)
    {
        Push_Func (start->right);
    }

}

void Back_End_Cycle (Node* start)
{
    switch ((int)(start->left->node_type))
    {
        case K_WORD:                    // Global assignation
        {
            Push_Assign (start->left);
            break;
        }
        case LINK:                      // Function declaration
        {
            Push_Func_Up (start->left);
            break;
        }
    }

    fprintf (f, "\n");

    if (start->right)
        Back_End_Cycle (start->right);
}


void Back_End (Node* start, Hash* hash1)
{
    f = fopen (file_asm, "w");
    hash = hash1;
    fprintf (f, "call %s:\n", MAIN);
    int randd = rand()%100000 + 100000;
    fprintf (f, "jmp end%d:\n\n\n", randd);

    Back_End_Cycle (start->right);

    fprintf (f, "end%d:\n", randd);
    fprintf (f, "end");

    fclose (f);
}
