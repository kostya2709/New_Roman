#include "Front_Head.h"

#include <algorithm>
//#include "Calc_tree_handler.cpp"

void Back_End_Cycle_x86(Node* start);

void Push_Call_Func_x86 (Node* start);


//I HATE GITHUB

void Push_Func_x86 (Node* start, int comp);

void Call_Func_x86 (Node* start);

char* Print_Var_x86 (int var_index);

void Print_Digit_Func (FILE* f_x86);

void Print_Digit_Call (FILE* f_x86);

void Print_SQRT_ASM (FILE* f_x86);

void Print_Data (FILE* f_x86);

FILE* f_x86;

Hash* hash_x86;

int comp1_x86 = 0;

int func_args          = 0;         //Number of arguments while workng in a function
int param_num_call     = 0;         //Number of arguments while calling
int param_num_call_cur = 0;

char print_var [10] = {};
int MAX_ARG_REG_NUM = 2;

constexpr uint Accuracy = 2;
const uint Acc_pow = pow (10, Accuracy);

int need_print = 0;

inline void (*Print_Str_x86 [10]) (void);

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
    "rdi", "rsi", "rdx", "rcx", "r8", "r9", "rax", "r15"
};

enum
{
    rdi, rsi, rdx, rcx, r8, r9, arg_rax, cmp_r15
};

enum
{
    __IMM,  __REG, __VAR, __ARG
};

enum 
{
    __ADD, __SUB, __MUL, __DIV, __MOV, __NUL, __TO_RAX, __FROM_RAX, __FREE_RDX, __BACK_RDX, __SQRT
};

char* the_register          = NULL;
char* next_register         = NULL;
char str_template [2][10]   = {};

inline void Print_SUB (void)
{
    fprintf (f_x86, "\tsub %s, qword %s\n", the_register, str_template [1]);
}


inline void Print_MOV (void)
{
    fprintf (f_x86, "\tmov %s, qword %s\n", the_register, str_template [0]);
}

inline void Print_TO_RAX (void)
{
    fprintf (f_x86, "\tmov rax, qword %s\n", str_template [0]);
}

inline void Print_FROM_RAX (void)
{
    fprintf (f_x86, "\tmov %s, rax\n", the_register);
}

inline void Print_FREE_RDX (void)
{
    fprintf (f_x86, "\tmov %s, rdx\n", the_register);
}

inline void Print_BACK_RDX (void)
{
    fprintf (f_x86, "\tmov rdx, %s\n", the_register);
}

inline void Print_SQRT (void)
{
    fprintf (f_x86, "\tmov r15, %u\n", Acc_pow);
    fprintf (f_x86, "\timul r15\n");
    Print_SQRT_ASM (f_x86);
}

inline void Print_MUL (void)
{
    fprintf (f_x86, "\tmov r15, %s\n", str_template [1]);
    fprintf (f_x86, "\timul r15\n");
    fprintf (f_x86, "\tmov r15, %u\n", Acc_pow);
    fprintf (f_x86, "\tcqo\n");
    fprintf (f_x86, "\tidiv r15\n");
}

inline void Print_DIV (void)
{
    fprintf (f_x86, "\tmov r15, %u\n", Acc_pow);
    fprintf (f_x86, "\timul r15\n");
    fprintf (f_x86, "\tmov r15, %s\n", str_template [1]);
    fprintf (f_x86, "\tcqo\n");
    fprintf (f_x86, "\tidiv r15\n");
}

inline void Print_NUL (void)
{
    return;
}

char* ((*String_Maker [3]))(int result, int temp_num);

char* Make_IMM (int result, int temp_num)
{
    sprintf (str_template [temp_num], "%d", result);
}

char* Make_VAR (int result, int temp_num)
{
    sprintf (str_template [temp_num], "%s", Print_Var_x86 (result));
}

char* Make_REG (int result, int temp_num)
{
    sprintf (str_template [temp_num], "%s", calc_registers [result]);
}

void Make_Func_Array (void)
{
    String_Maker [__IMM] = Make_IMM;
    String_Maker [__REG] = Make_REG;
    String_Maker [__VAR] = Make_VAR;


    Print_Str_x86 [__SUB]      = Print_SUB;
    Print_Str_x86 [__MOV]      = Print_MOV;
    Print_Str_x86 [__NUL]      = Print_NUL;
    Print_Str_x86 [__TO_RAX]   = Print_TO_RAX;
    Print_Str_x86 [__FROM_RAX] = Print_FROM_RAX;
    Print_Str_x86 [__MUL]      = Print_MUL;
    Print_Str_x86 [__DIV]      = Print_DIV;
    Print_Str_x86 [__FREE_RDX] = Print_FREE_RDX;
    Print_Str_x86 [__BACK_RDX] = Print_BACK_RDX;
    Print_Str_x86 [__SQRT]     = Print_SQRT;

}


int Print_Manager (int command, int left_type, int right_type, int left_val, int right_val, bool on_top, int& register_count, int& ret_flag)
{
    bool is_reg = (left_type == __REG);
    bool move_rax = (command == __MUL) || (command == __DIV);
    bool move_rdx = (func_args >= rdx && (command == __MUL || command == __DIV));

    int the_reg = (is_reg)? left_val * (!on_top): register_count * (!on_top);       //rax = 0 - is used if on_top
    the_register = const_cast <char*> (calc_registers [the_reg]);
    next_register = const_cast <char*> (calc_registers [register_count + 1]);   
    register_count += (!on_top) && (!is_reg);

    String_Maker [left_type]  (left_val, 0);
    String_Maker [right_type] (right_val, 1);

    Print_Str_x86 [((__MOV * (!is_reg) + is_reg * __NUL) * (!on_top) + (on_top) * __NUL) * (!move_rax) + (move_rax) * __NUL]();
    Print_Str_x86 [__TO_RAX * (on_top || move_rax) + (!(on_top || move_rax)) * __NUL]();
    Print_Str_x86 [__FREE_RDX * (move_rdx) + (!move_rdx) * __NUL]();    

    Print_Str_x86 [command]();

    Print_Str_x86 [__FROM_RAX * (move_rax && !on_top) + !(move_rax && !on_top) * __NUL]();
    Print_Str_x86 [__BACK_RDX * (move_rdx) + (!(move_rdx)) * __NUL]();

    register_count = (is_reg) && (right_type == __REG)? std::min (left_val, right_val) : register_count;
    
    ret_flag = return_register;
    return the_reg;
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
        return start->data * Acc_pow;
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
                return Print_Manager (__SUB, left_state, right_state, left_res, right_res, on_top, register_count, ret_flag);
                break;
            }
            case MUL:
            {
                return Print_Manager (__MUL, left_state, right_state, left_res, right_res, on_top, register_count, ret_flag);
                break;
            }
            case DIV:
            {
                return Print_Manager (__DIV, left_state, right_state, left_res, right_res, on_top, register_count, ret_flag);
                break;
            }
            case SQRT:
            {
                return Print_Manager (__SQRT, right_state, left_state, right_res, left_res, on_top, register_count, ret_flag);
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
    Push_Expr_x86 (start->right, __REG, arg_rax);
    fprintf (f_x86, "\tmov r14, %d\n\n", Accuracy);
    Print_Digit_Call (f_x86);
    need_print = 1;
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

    Push_Expr_x86 (left, __REG, cmp_r15);
    Push_Expr_x86 (right, __REG, arg_rax);

    fprintf (f_x86, "\tcmp r15, rax\n");

    switch ((int)(condi->data))
    {
        case MORE:
        {
            fprintf (f_x86, "\tjg ");
            break;
        }
        case EMORE:
        {
            fprintf (f_x86, "\tjge ");
            break;
        }
        case LESS:
        {
            fprintf (f_x86, "\tjl ");
            break;
        }
        case ELESS:
        {
            fprintf (f_x86, "\tjle ");
            break;
        }
        case EQUAL:
        {
            fprintf (f_x86, "\tje ");
            break;
        }
        case UNEQUAL:
        {
            fprintf (f_x86, "\tjne ");
            break;
        }
    }

    fprintf (f_x86, "comp%d\n", comp);
    fprintf (f_x86, "\tjmp comp%d_else\n", comp);
    fprintf (f_x86, "comp%d:\n", comp);

    int temp_comp = comp;

    if (strcmp (start->right->sym, "C")== 0)
        Push_Func_x86 (start->right->right, comp1_x86);
    else Push_Func_x86 (start->right, comp1_x86);

    if (in_while)
        fprintf (f_x86, "\tjmp while_comp%d_back\n", temp_comp);


    if (!in_while) 
        fprintf (f_x86, "\tjmp comp%d_out\n", comp);
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

    fprintf (f_x86, "\tmov rsp, rbp\n");
    fprintf (f_x86, "\tpop rbp\n");
    fprintf (f_x86, "\tret\n");

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
    Make_Func_Array();

    const char* file_asm = "res_x86.asm";
    f_x86 = fopen (file_asm, "w");
    hash_x86 = hash1;

    Print_Data (f_x86);

    fprintf (f_x86, "section .text\n\n");
    fprintf (f_x86, "global _start\n\n\n");
    fprintf (f_x86, "_start:\n");
    fprintf (f_x86, "\ncall main\n\n");
    fprintf (f_x86, "\tmov rax, 1\n");
    fprintf (f_x86, "\txor rbx, rbx\n"); 
    fprintf (f_x86, "\tint 0x80\n");
    fprintf (f_x86, "ret\n");

    Back_End_Cycle_x86 (start->right, comp);

    if (need_print == 1)
        Print_Digit_Func (f_x86);

    fclose (f_x86);
}