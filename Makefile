FNAME=result
CFLAGS= -c -Wall
LFLAGS= -o

FNAME_ASM=res_x86
CFLAGS_ASM= -f elf64
LFLAGS_ASM= -s -o

TEST_FILE=Back_end_x86

all: clean_$(TEST_FILE) $(TEST_FILE).o $(FNAME)

clean_$(TEST_FILE):
	@rm -rf $(TEST_FILE).o

execute: all asm_exe

run: asm_exe
	@edb --run $(FNAME_ASM)

asm_exe:
	@nasm $(CFLAGS_ASM) $(FNAME_ASM).asm
	@ld $(LFLAGS_ASM) $(FNAME_ASM) $(FNAME_ASM).o
	@./$(FNAME_ASM)

alll: clean additional_files.o all


$(FNAME): $(FNAME).o
	@g++ $(LFLAGS) $(FNAME) Back_end.o Back_end_x86.o Derive.o Expression_Develop.o \
File_Reader.o Frontend-1.o my_stdio.o Hash.o R_num.o Recursive_Descend.o Stack_Dyn_Func.o Standart_Tree.o Tokenization.o Tree_Funcs.o Txt_Files.o main.o 
	@./$(FNAME)


$(TEST_FILE).o:
	@g++ $(CFLAGS) $(TEST_FILE).cpp

$(FNAME).o:
	@g++ $(CFLAGS)  main.cpp

additional_files.o:
	@g++ $(CFLAGS) Back_end.cpp
	@g++ $(CFLAGS) Derive.cpp
	@g++ $(CFLAGS) Expression_Develop.cpp
	@g++ $(CFLAGS) File_Reader.cpp
	@g++ $(CFLAGS) Frontend-1.cpp
	@g++ $(CFLAGS) my_stdio.cpp
	@g++ $(CFLAGS) Hash.cpp
	@g++ $(CFLAGS) R_num.cpp
	@g++ $(CFLAGS) Recursive_Descend.cpp
	@g++ $(CFLAGS) Stack_Dyn_Func.cpp
	@g++ $(CFLAGS) Standart_Tree.cpp
	@g++ $(CFLAGS) Tokenization.cpp
	@g++ $(CFLAGS) Tree_Funcs.cpp
	@g++ $(CFLAGS) Txt_Files.cpp

clean:
	@rm -rf $(FNAME) *.o