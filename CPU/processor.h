#ifndef PROCESSOR
#define PROCESSOR
// -I../ for makefile
#include "../common/stack/stack_cd.h"
#include "../common/asm_file.h"

struct CPU
{
    stack stk;
    Elem_t gp_regs[20];
};

long LoadBinary (Elem_t** binary_name, const int cmd_line_status);
void CpuExecute (CPU* cpu, Elem_t* binary_name, long file_size, Elem_t* ram);
void VideoMemory (Elem_t ram[]);

#endif
