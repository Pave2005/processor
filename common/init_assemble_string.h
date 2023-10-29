#include <stdio.h>
#include "/Users/pavelsumilo/processor/common/stack/stack_cd.h"
#ifndef INIT
#define INIT

// hash = 15257125 % 128 == 70
//        8124812851 % 128 == 70

//
// hash address
// for ( i .. )
// labels[i].hash = 125215

enum ISA
{
    SKIPER = -8,
    HLT = -1,
    PUSH = 1,
    DIV = 2,
    SUB = 3,
    OUT = 4,
    ADD = 5,
    MUL = 6,
    SQRT = 7,
    IN = 8,
    POP = 9,
    JMP = 14,
    LB = 15,
    JB = 16,
    PUSHRAM = 17,
    POPRAM = 18,
    CALL = 19,
    RET = 20,
};

enum REGISTER_CODES
{
    RAX = 10,
    RBX = 11,
    RCX = 12,
    RDX = 13,
};

struct AssemblerFile
{
    long file_size;
    FILE* file;
    FILE* assembler_file;
    char* buf;
    size_t n_lines;
    int* lines;
    int* bin_code;
};

struct Registers
{
    //Elem_t* gp_regs;
    Elem_t rax;
    Elem_t rbx;
    Elem_t rcx;
    Elem_t rdx;
};

struct CPU
{
    stack stk;
    Registers reg;
    AssemblerFile info;
};

struct Labels
{
    int hash;
    int adress;
};

char* ReadText (AssemblerFile info);
long FileSize (AssemblerFile info);
void Lines (AssemblerFile* info);

#endif
