#include "Front_Head.h"

#include <algorithm>
#include "Calc_tree_handler.cpp"

void Back_End_Cycle_x86(Node* start);

void Push_Call_Func_x86 (Node* start);

void Push_Func_x86 (Node* start, int comp);

void Call_Func_x86 (Node* start);

char* Print_Var_x86 (int var_index);

FILE* f_x86;

Hash* hash_x86;

int comp1_x86 = 0;

int func_args          = 0;         //Number of arguments while workng in a function
int param_num_call     = 0;         //Number of arguments while calling
int param_num_call_cur = 0;

char print_var [10] = {};
int MAX_ARG_REG_NUM = 2;

inline void (*Print_Str_x86 [3][2][3])(int command, int dst, int src);

enum
{
    tree_top          = -1,
    return_number     =  0,
    return_register   =  1,
    return_var        =  2,
    all_done          =  3
};

const char* calc_registers[] = 
{
    "rax", "rbx", "r10", "r11", "r12", "r13", "r14", "r15" 
};

enum
{
    rax, rbx, r10, r11, r12, r13, r14, r15 
};

const char* arg_registers[] = 
{
    "rdi", "rsi", "rdx", "rcx", "r8", "r9", "rax"
};

enum
{
    rdi, rsi, rdx, rcx, r8, r9, arg_rax
};

enum
{
    __MEM, __REG, __IMM, __ARG
};

const char* commands[] = 
{
    "mov", "add", "lea" 
};

enum 
{
    __MOV, __ADD, __LEA, __SUB, __MUL, __DIV
};

inline void Print_SUB_MEM_REG (int command, int dst, int src)
{
    fprintf (f_x86, "\t%s qword [rbp-%d], %d\n", commands [command], (dst + 1) * 8, calc_registers [src]);
}

inline void Print_SUB_MEM_IMM (int command, int dst, int src)
{
    fprintf (f_x86, "\t%s qword [rbp-%d], %d\n", commands [command], (dst + 1) * 8, src);
}

inline void Print_SUB_REG_MEM (int command, int dst, int src)
{
    fprintf (f_x86, "\t%s %s, qword [rbp-%d]\n", commands [command], calc_registers [dst], (src + 1) * 8);
}

inline void Print_SUB_REG_REG (int command, int dst, int src)
{
    fprintf (f_x86, "\t%s %s, %s\n", commands [command], calc_registers [dst], calc_registers [src]);
}

inline void Print_SUB_REG_IMM (int command, int dst, int src)
{
    fprintf (f_x86, "\t%s %s, %d\n", commands [command], calc_registers [dst], src);
}

void Print_Asm_Str (int command, int src_type, int dst_type, int src, int dst)
{
    
    Print_Str_x86 [__SUB][__MEM][__REG] = Print_SUB_MEM_REG;
    Print_Str_x86 [__SUB][__MEM][__IMM] = Print_SUB_MEM_IMM;
    Print_Str_x86 [__SUB][__REG][__MEM] = Print_SUB_REG_MEM;
    Print_Str_x86 [__SUB][__REG][__REG] = Print_SUB_REG_REG;
    Print_Str_x86 [__SUB][__REG][__IMM] = Print_SUB_REG_IMM;
}

int Push_Expr_x86 (Node* start, int& ret_flag, int& register_count, bool on_top)
{

    int start_flag = ret_flag;

    int left_res    = 0;
    int right_res   = 0;
    int left_state  = 0;
    int right_state = 0;

    if (start->left)
        if (start->node_type != CALL)
        {
            left_res = Push_Expr_x86 (start->left, ret_flag, register_count, 0);
            left_state = ret_flag;
        }

    if (start->right)
        if (start->node_type != CALL)
        {
            right_res = Push_Expr_x86 (start->right, ret_flag, register_count, 0);
            right_state = ret_flag;
        }

    if (start->node_type == CALL)
    {
        Push_Call_Func_x86 (start);
        ret_flag = return_register;
        int reg_num = rax;
        
        if (!on_top)
        {
            fprintf (f_x86, "\tmov %s, rax\n", calc_registers [register_count]);
            reg_num = register_count;
            register_count++;
        }
        return reg_num;
    }

    if (start->node_type == NUMBER)
    {
        ret_flag = return_number;
        return start->data;
    }

    if (start->node_type == OPERATOR)
    {

        switch ((int)start->data)
        {
            case ADD:
            {

                if (right_state == return_number || right_state == return_register)
                {
                    std::swap (right_res, left_res);
                    std::swap (right_state, left_state);
                }

                if (left_state == return_number && right_state == return_var)
                {
                    if (on_top)
                    {
                        if (start_flag == right_res)
                        {
                            fprintf (f_x86, "\tadd qword %s, %d\n", Print_Var_x86 (start_flag), left_res);
                            ret_flag = all_done;
                            return 0;
                        }
                        else
                        {
                            fprintf (f_x86, "\tmov rax, qword %s\n", Print_Var_x86 (right_res));
                            fprintf (f_x86, "\tadd rax, %d\n", left_res);
                            ret_flag = return_register;
                            return 0;
                        }
                        
                    }
                    else
                    {
                        fprintf (f_x86, "\tmov rax, qword %s\n", Print_Var_x86 (right_res));
                        fprintf (f_x86, "\tlea %s, [rax+%d]\n", calc_registers [register_count], left_res);
                        ++register_count;
                        ret_flag = return_register;
                        return register_count - 1;
                    }
                    
                }
                else if ((right_state == return_var) && (left_state == return_var))
                {
                    fprintf (f_x86, "\tmov rax, qword %s\n", Print_Var_x86 (right_res));

                    if (!on_top)
                    {
                        fprintf (f_x86, "\tmov %s, qword %s\n", calc_registers [register_count], Print_Var_x86 (left_res));
                        fprintf (f_x86, "\tadd %s, rax\n", calc_registers [register_count], calc_registers [register_count]);
                        ++register_count;
                        ret_flag = return_register;
                        return register_count - 1;
                    }
                    else
                    {
                        fprintf (f_x86, "\tadd rax, qword %s\n", Print_Var_x86 (left_res)); 
                        ret_flag = return_register;
                        return 0;
                    }
                    
                }
                else if (right_state == return_register && left_state == return_number || left_state == return_register && right_state == return_number)
                {
                    int the_register = right_state == return_register? right_res : left_res;
                    int the_number   = right_state == return_number?   right_res : left_res;

                    if (on_top)
                    {
                        fprintf (f_x86, "\tlea rax, [%s+%d]\n", calc_registers [the_register], the_number);
                        ret_flag = return_register;
                        return 0;
                    }

                    fprintf (f_x86, "\tadd %s, qword %d\n", calc_registers [the_register], the_number);
                    ret_flag = return_register;
                    return the_register;

                }
                else if (left_state == return_register && right_state == return_var || left_state == return_var && right_state == return_register)
                {
                    int the_register = right_state == return_register? right_res : left_res;
                    int the_var      = right_state == return_var?      right_res : left_res;
                    
                    if (on_top)
                    {
                        fprintf (f_x86, "\tadd %s, qword %s\n", calc_registers [the_register], Print_Var_x86 (the_var));
                        fprintf (f_x86, "\tmov rax, %s\n", calc_registers [the_register]);
                        ret_flag = return_register;
                        return 0;
                    }

                    fprintf (f_x86, "\tadd %s, qword %s\n", calc_registers [the_register], Print_Var_x86 (the_var));
                    ret_flag = return_register;
                    return the_register;
                }
                else if (left_state == return_register && right_state == return_register)
                {
                    if (on_top)
                    {
                        fprintf (f_x86, "\tlea rax, [%s+%s]\n", calc_registers [left_res], calc_registers [right_res]);
                        ret_flag = return_register;
                        return 0;
                    }   

                    fprintf (f_x86, "\tadd %s, %s\n", calc_registers [left_res], calc_registers [right_res]);
                    register_count = left_res + 1;
                    ret_flag = return_register;
                    return left_res;
                }
                
                
                break;
            }
            case MIN:
            {
                
                break;
            }
            case MUL:
            {
                fprintf (f_x86, "mul\n");
                break;
            }
            case DIV:
            {
                fprintf (f_x86, "div\n");
                break;
            }
            case SQRT:
            {
                fprintf (f_x86, "sqrt\n");
                break;
            }
        }
    }
        else if (start->node_type == VAR)
        {
            ret_flag = return_var;
            return start->data;
        }

        ret_flag = return_register;
        
        return 0;
}


char* Print_Var_x86 (int var_index)
{
    if (var_index >= func_args)
        sprintf (print_var, "[rbp-%d]", (var_index - func_args + 1) * 8);                   //Take a local var
    
    else if (var_index >= MAX_ARG_REG_NUM)
        sprintf (print_var, "[rbp+%d]", (var_index - MAX_ARG_REG_NUM + 2) * 8);             //Take an argument from stack
    
    else 
        sprintf (print_var, "%s", arg_registers [var_index]);                               //Take an argument from a register

    return print_var;
}

int Push_Expr_x86 (Node* start, int answer_type, int answer_index)
{
    int register_counter = 1;
    int ret_flag = -1;
    int result = Push_Expr_x86 (start, ret_flag, register_counter, 1);

    bool push_auxilary_reg = 0;


    if (answer_type == __ARG)
    {                                                                            //answer_index >= 1
        if (answer_index <= func_args && answer_index <= MAX_ARG_REG_NUM)
            fprintf (f_x86, "\tpush %s\n", arg_registers [answer_index - 1]);           //remember register before calling a function

        answer_type  = __REG;
        
        if (answer_index > MAX_ARG_REG_NUM)                //Put arguments into stack
        {
            push_auxilary_reg = 1;
            answer_index = arg_rax + 1;
        }

        answer_index--;                                     //answer_index >= 0
    }


    switch (ret_flag)
    {
        case return_number:
            if (answer_type == __REG)
                fprintf (f_x86, "\tmov %s, %d\n", arg_registers [answer_index], result);
            else
                fprintf (f_x86, "\tmov qword [rbp-%d], %d\n", (answer_index + 1) * 8, result);
        break;

        case (return_register):
            if (answer_type == __REG)
            {
                if (answer_index != arg_rax)
                    fprintf (f_x86, "\tmov %s, rax\n", arg_registers [answer_index]);
            }
            else
                fprintf (f_x86, "\tmov qword [rbp-%d], rax\n", (answer_index + 1) * 8);
        break;

        case (return_var):
            if (answer_type == __REG)
                fprintf (f_x86, "\tmov %s, qword %s\n", arg_registers [answer_index], Print_Var_x86 (result));
            else
            {
                fprintf (f_x86, "\tmov rax, qword %s\n", Print_Var_x86 (result));
                fprintf (f_x86, "\tmov qword [rbp-%d], rax\n", (answer_index + 1) * 8);
            }
        break;

        default:
            printf ("looser!\n");
        break;
    }

    if (push_auxilary_reg)
        fprintf (f_x86, "\tpush rax\n");

}

void Push_Print_x86 (Node* start)
{
    int var = start->right->data;
    fprintf (f_x86, "push [ax+%d]\n", var);
    fprintf (f_x86, "out\n");
}

void Push_Read_x86 (Node* start)
{
    int var = start->right->data;
    fprintf (f_x86, "in\n");
    fprintf (f_x86, "pop [ax+%d]\n", var);
}

void Push_One_Cond_x86 (Node* start, int comp)
{
    Node* left = start->left->left;
    Node* right = start->left->right;

    Node* condi = start->left;

    int in_while = 0;

    if (start->data == CYCLE)
    {
        in_while = 1;
        fprintf (f_x86, "while_comp%d_back:\n", comp);
    }

    //Push_Expr_x86 (left);
    //Push_Expr_x86 (right);


    switch ((int)(condi->data))
    {
        case MORE:
        {
            fprintf (f_x86, "jg ");
            break;
        }
        case EMORE:
        {
            fprintf (f_x86, "jge ");
            break;
        }
        case LESS:
        {
            fprintf (f_x86, "jl ");
            break;
        }
        case ELESS:
        {
            fprintf (f_x86, "jle ");
            break;
        }
        case EQUAL:
        {
            fprintf (f_x86, "je ");
            break;
        }
        case UNEQUAL:
        {
            fprintf (f_x86, "jne ");
            break;
        }
    }

    fprintf (f_x86, "comp%d:\n", comp);
    fprintf (f_x86, "jmp comp%d_else:\n", comp);
    fprintf (f_x86, "comp%d:\n", comp);

    int temp_comp = comp;

    if (strcmp (start->right->sym, "C")== 0)
        Push_Func_x86 (start->right->right, comp1_x86);
    else Push_Func_x86 (start->right, comp1_x86);

    if (in_while)
        fprintf (f_x86, "jmp while_comp%d_back:\n", temp_comp);


    if (!in_while) fprintf (f_x86, "jmp comp%d_out:\n", comp);
    fprintf (f_x86, "comp%d_else:\n\n", temp_comp);

    if (strcmp (start->right->sym, "C") == 0)
    {
        if (start->right->left->right->right->node_type == OPERATOR)
        {
            comp1_x86++;
            Push_One_Cond_x86 (start->right->left->right->right, comp1_x86);
        }
        else
            Push_Func_x86 (start->right->left->right, comp1_x86);
    }
}

void Push_Invade_x86 (Node* start, int comp)
{
    comp1_x86++;
    int temp = comp1_x86;
    Push_One_Cond_x86 (start, comp1_x86);

    fprintf (f_x86, "comp%d_out:\n\n\n", temp);
}

void Push_Assign_x86 (Node* start)
{
    int var_num = start->left->data;

    int state = var_num;
    int reg_count = 1;                  //ret_flag = var_num - means: a = a + 5 -> mov [rbp-...], 5. 0 means: a = a + 5 -> mov rax, [rbp-...], add rax, 5
    int result = Push_Expr_x86 (start->right, state, reg_count, 1);

    switch (state)
    {
        case return_number:
            fprintf (f_x86, "\tmov qword %s, %d", Print_Var_x86 (var_num), result);
        break;

        case (return_register):
            fprintf (f_x86, "\tmov qword %s, rax\n", Print_Var_x86 (var_num));
        break;

        default:
        break;
    }
    
    
    fprintf (f_x86, "\n");

}


void Push_Return_x86 (Node* start)
{
    Push_Expr_x86 (start->right, __REG, arg_rax);
}


void Push_Call_Args_x86 (Node* start)
{
    if (start->left)
    {
        ++param_num_call;
        Push_Call_Args_x86 (start->left);
    }
    else
        param_num_call_cur = param_num_call;

    Push_Expr_x86 (start->right, __ARG, param_num_call_cur);
    --param_num_call_cur;

}

void Push_Call_Func_x86 (Node* start)
{
    char* func = start->right->sym;

    param_num_call = 0;

    if (start->left)
    {
        param_num_call = 1;
        Push_Call_Args_x86 (start->left);
    }

    fprintf (f_x86, "\n\tcall %s\n\n", func);

    if (param_num_call > MAX_ARG_REG_NUM)
        fprintf (f_x86, "\tadd rsp, %d\n\n", (param_num_call - MAX_ARG_REG_NUM) * 8);

    printf ("func %d, call %d\n", func_args, param_num_call);
    for (int i = 0; i < func_args && i < param_num_call && i < MAX_ARG_REG_NUM; ++i)
        fprintf (f_x86, "\tpop %s\n", arg_registers [i]);
    
    fprintf (f_x86, "\n");

}

void Push_Func_x86 (Node* start, int comp)
{
    assert (start);

    if (strcmp (start->sym, "B") == 0)
        Push_Func_x86 (start->right, comp);
    
    if (start->right == 0)
            return;
    
    if (start->right->node_type == CALL)
        Push_Call_Func_x86 (start->right);
    
    else switch ((int)(start->right->data))
    {
        case ASSIGN:
        {
            Push_Assign_x86 (start->right);
            break;
        }
        case PRINT:
        {
            Push_Print_x86 (start->right);
            break;
        }
        case READ:
        {
            Push_Read_x86 (start->right);
            break;
        }
        case CYCLE:
        case IF:
        {
            Push_Invade_x86 (start->right, comp);
            break;
        }
        case RETURN:
        {
            Push_Return_x86 (start->right);
            break;
        }
        case WRITE:
        {
            fprintf (f_x86, "\noutf \"%s\"\n\n", start->right->right->sym);
            break;
        }
    }

    fprintf (f_x86, "\n");

    if (start->left)
        Push_Func_x86 (start->left, comp);

}


void Push_Func_Args_x86 (Node* start)
{
    if (start->left)
        Push_Func_Args_x86 (start->left);

    ++func_args;
}

void Push_Func_Up_x86 (Node* start, int comp)
{

    fprintf (f_x86, "\t%s:\n\n", start->right->sym);
    fprintf (f_x86, "\tpush rbp\n");
    fprintf (f_x86, "\tmov rbp, rsp\n");

    func_args = 0;

    if (start->right->left->data)
        fprintf (f_x86, "\tsub rsp, %lg\n\n", start->right->left->data * 8);

    if (start->left)
        Push_Func_Args_x86 (start->left);
    

    fprintf (f_x86, " \n");


    if (start->right)
    {
        Push_Func_x86 (start->right->right, comp);
    }

    fprintf (f_x86, "\tmov rsp, rbp\n");
    fprintf (f_x86, "\tpop rbp\n");
    fprintf (f_x86, "\tret\n");


}

void Back_End_Cycle_x86 (Node* start, int comp)
{
    switch ((int)(start->right->node_type))
    {
        case K_WORD:                    // Global assignation
        {
            Push_Assign_x86 (start->right);
            break;
        }
        case LINK:                      // Function declaration
        {
            Push_Func_Up_x86 (start->right, comp);
            break;
        }
    }

    fprintf (f_x86, "\n");

    if (start->left)
        Back_End_Cycle_x86 (start->left, comp);
}


void Back_End_x86 (Node* start, Hash* hash1, int comp)
{
    const char* file_asm = "res_x86.asm";
    f_x86 = fopen (file_asm, "w");
    hash_x86 = hash1;

    fprintf (f_x86, "section .text\n\n");
    fprintf (f_x86, "global _start\n\n\n");
    fprintf (f_x86, "_start:\n");
    fprintf (f_x86, "\ncall main\n\n");
    fprintf (f_x86, "\tmov rax, 1\n");
    fprintf (f_x86, "\txor rbx, rbx\n"); 
    fprintf (f_x86, "\tint 0x80\n");
    fprintf (f_x86, "ret\n");

    Back_End_Cycle_x86 (start->right, comp);

    fclose (f_x86);
}