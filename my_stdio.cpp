#include <stdio.h>


constexpr const char* data_printf = 
{
    "\t\n\nsection .data\n"
    "\tdigit: times 10d db 0\n"				//A temporary buffer to write digitals.
    "\tprintf_digit_buf: times 10d db 0\n"
    "\ttemp_buf: times 10d db 0\n"
    "\tsqrt: dq 0\n"
    "\tsection .text\n\n"
};


constexpr const char* my_printf = 
{

    "\t\t;This is my printf function:\n\n"
    "\t_Printf_Digit:\n"

    "\tmov qword [digit], rax\n"
        
    "\tmov r15, rdx\n"							// Put the printf_buffer counter into r15 to use rdx in division.

    "\tmov rax, qword [digit]\n"
    "\tmov r10, printf_digit_buf\n"

    "\tand rax, rax\n"							//set sign flag

    "\tjs .signed\n"
    "\tjmp .Pos\n"

    "\t.signed:\n"
    "\tneg rax\n"
    "\tmov byte [r10], '-'\n"
    "\tinc r10\n"

    "\t.Pos:\n"

    "\tmov rbx, 10d\n"
    "\txor r12, r12\n"

    ".Cycle:\n"

    "\txor rdx, rdx\n"
    "\tdiv rbx\n"
    "\tadd rdx, 30h\n"                        //остаток
    "\tinc r12\n"
    "\tmov byte [temp_buf + r12], dl\n"

    "\tcmp rax, 0\n"
    "jg .Cycle\n" 

    "\tcall _Write_Reverse\n"

    "\tmov rdx, r15\n"

    ".End:\n"

    "\tmov rax, 4\n"                  		// The number of the system call "sys_write".
    "\tmov rbx, 1\n"               		 	// The file descriptor "stdout".
    "\tmov rcx, printf_digit_buf\n"          	// Pointer to the buffer.
    "\tmov rdx, 10\n"                    		// RDX = RDI - RAX = number of symbols written into the buffer.
    "\tint 0x80\n"                            // Call the interruption.
    "\tret\n"

    "_Write_Reverse:\n"

	"\tmov rbx, r14\n"
	"\tmov r13, 0\n"				            //Put a dot?

    ".print_zeros:\n"

	"\tcmp r14, r12\n"
	"jl .Write_buf\n"
	"\tmov byte [r10], \"0\"\n"
    "\tinc r10\n"

	"\tcmp r13, 0\n"
	"\tjne .no_dot\n"
	"\tmov byte [r10], \".\"\n"
    "\tinc r10\n"
	"\tmov r13, 1\n"

    ".no_dot:\n"
	"\tdec r14\n"
	"\tjmp .print_zeros\n"
 
    ".Write_buf:\n"

	"\tcmp r12, rbx\n"
	"\tjne .further\n"
	"\tcmp r13, 0\n"
	"\tjne .further\n"
	"\tmov byte [r10], \".\"\n"
    "\tinc r10\n"

    ".further:\n"

    "\tmov al, [temp_buf + r12]\n"
    "\tmov byte [r10], al\n"
    "\tinc r10\n"
	"\tdec r12\n"
	"\tcmp r12, 0\n"
	"\tjg .Write_buf\n"
    "\tret\n"

};

void Print_Digit_Call (FILE* f_x86)
{
    fprintf (f_x86, "\tcall _Printf_Digit\n");
}

void Print_SQRT_ASM (FILE* f_x86)
{
    fprintf (f_x86, 
    "\n\t\t;Make SQRT of the value:\n"
    "\tmov qword [sqrt], rax\n"
    "\tfinit\n"
    "\tfild qword [sqrt]\n"
    "\tfsqrt\n"
    "\tfistp qword [sqrt]\n"
    "\tmov rax, [sqrt]\n"
    );
}

void Print_Digit_Func (FILE* f_x86)    //Prints a number  
                                       //Requires result of expression in rax
                                       //Requires accuracy in r14
                                       //Spoils calc registers
{
    fprintf (f_x86, "%s", my_printf);
}

void Print_Data (FILE* f_x86)
{
    fprintf (f_x86, "%s", data_printf);
}