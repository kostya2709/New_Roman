#include "Front_Head.h"

void Back_End_Cycle(Node* start, int comp);

void Push_Stand_Call_Func (Node* start);

void Push_Stand_Func (Node* start);

void Call_Func (Node* start);

static FILE* f;

static Hash* hash;


static int comp = 0;
static int cond = 0;

void Push_Stand_Expr (Node* start)
{
    if (start->left)
        if (start->node_type != CALL)
            Push_Stand_Expr (start->left);

    if (start->right)
        if (start->node_type != CALL)
            Push_Stand_Expr (start->right);

    if (start->node_type == CALL)
        Push_Stand_Call_Func (start);

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
            fprintf (f, "push [ax+%lg]\n", start->data);
}

void Push_Stand_Print (Node* start)
{
    int var = start->left->data;
    fprintf (f, "push [ax+%d]\n", var);
    fprintf (f, "out\n");
}

void Push_Stand_Read (Node* start)
{
    int var = start->left->data;
    fprintf (f, "in\n");
    fprintf (f, "pop [ax+%d]\n", var);
}

void Push_Stand_One_Cond (Node* start, Node* left, Node* right)
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

        Push_Stand_Expr (left);
        Push_Stand_Expr (right);
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

    Push_Stand_Func (start->left);

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
            Push_Stand_One_Cond(start->parent->right, left, right);

    if (start->right && !(in_while))
        Push_Stand_One_Cond(start->right, left, right);
}

void Push_Stand_Invade (Node* start)
{
    Node* left_expr = start->left->left;
    Node* right_expr = start->left->right;

    Push_Stand_One_Cond (start->right, left_expr, right_expr);

    fprintf (f, "comp%d_out:\n\n\n", comp);
    cond = 0;
    comp++;
}

void Push_Stand_Assign (Node* start)
{
    int var_num = start->left->data;

    Push_Stand_Expr(start->right);

    fprintf (f, "pop [ax+%d]\n", var_num);
}

void Push_Stand_Return (Node* start)
{
    Push_Stand_Expr (start->left);
    fprintf (f, "\npush ax\n");
    fprintf (f, "push %d\n", MAX_VAR_IN_FUNC);
    fprintf (f, "min\n");
    fprintf (f, "pop ax\n");
    fprintf (f, "ret\n\n");
}

void Push_Stand_Call_Args (Node* start)
{
    Push_Stand_Expr (start->left);
    if (start->right)
        Push_Stand_Call_Args (start->right);
}

void Push_Stand_Call_Func (Node* start)
{

    char* func = start->sym;

    if (start->left)
        Push_Stand_Call_Args (start->left);

    fprintf (f, "push ax\n");
    fprintf (f, "push %d\n", MAX_VAR_IN_FUNC);
    fprintf (f, "add\n");
    fprintf (f, "pop ax\n");

    fprintf (f, "call %s:\n\n", func);

}

void Push_Stand_Func (Node* start)
{
    if (start->left == 0)
        return;

    if (start->left->node_type == CALL)
        Push_Stand_Call_Func (start->left);
    else
    switch ((int)(start->left->data))
    {
        case ASSIGN:
        {
            Push_Stand_Assign (start->left);
            break;
        }
        case PRINT:
        {
            Push_Stand_Print (start->left);
            break;
        }
        case READ:
        {
            Push_Stand_Read(start->left);
            break;
        }
        case INVADE:
        {
            Push_Stand_Invade (start->left);
            break;
        }
        case RETURN:
        {
            Push_Stand_Return (start->left);
            break;
        }
        case WRITE:
        {
            fprintf (f, "\noutf \"%s\"\n\n", start->left->left->sym);
        }
    }

    fprintf (f, "\n");

    if (start->right)
        Push_Stand_Func (start->right);

}

void Push_Stand_Func_Args (Node* start)
{
    if (start->right)
        Push_Stand_Func_Args (start->right);

    fprintf (f, "pop [ax+%d]\n", (int)start->left->data);
}

void Push_Stand_Func_Up (Node* start)
{

    fprintf (f, "%s:\n", start->sym);

    if (start->left)
        Push_Stand_Func_Args (start->left);

    fprintf (f, "\n");

    if (start->right)
    {
        Push_Stand_Func (start->right);
    }

}

void Back_Stand_End_Cycle (Node* start)
{
    switch ((int)(start->left->node_type))
    {
        case K_WORD:                    // Global assignation
        {
            Push_Stand_Assign (start->left);
            break;
        }
        case LINK:                      // Function declaration
        {
            Push_Stand_Func_Up (start->left);
            break;
        }
    }

    fprintf (f, "\n");

    if (start->right)
        Back_End_Cycle (start->right, comp);
}


void Back_Stand_End (Node* start, Hash* hash1)
{
    f = fopen (file_asm, "w");
    hash = hash1;

    fprintf (f, "call %s:\n", MAIN);
    int randd = rand()%100000 + 100000;
    fprintf (f, "jmp end%d:\n\n\n", randd);

    Back_End_Cycle (start->right, comp);

    fprintf (f, "end%d:\n", randd);
    fprintf (f, "end");

    fclose (f);
}
