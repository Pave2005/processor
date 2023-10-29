#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

// -I../ for makefile
#include "../common/stack/stack_cd.h"
#include "../common/init_assemble_string.h"

void CpuExecute (CPU* cpu, Elem_t* ram);
void VideoMemory (Elem_t ram[]);

int main ()
{
    CPU cpu = {};

    Elem_t ram[1000] = {};

    StackCtor (&(cpu.stk), 5);
    cpu.info.file = fopen ("CPU/assembler_file.bin", "rb");
    cpu.info.file_size = FileSize (cpu.info);
    cpu.info.bin_code = (int*)ReadText (cpu.info);

    CpuExecute(&cpu, ram);

    VideoMemory (ram);
}

void VideoMemory (Elem_t ram[])
{
    printf ("\n");

    for (int i = 0; i < 1000; i++)
    {
        if (i % 169 == 0)
            printf ("\n");
        if (ram[i] != 0)
            printf ("*");
        else
            printf (".");
    }
}

void CpuExecute (CPU* cpu, Elem_t* ram)
{
    int cmd_size = 4;
    stack rets = {};
    //regs.gp_regs = (Elem_t*)calloc (20, sizeof (Elem_t));
    for (int i = 0; i < cpu->info.file_size / cmd_size; i++)
    {
        if (cpu->info.bin_code[i] == PUSH)
        {
            //StackPush (&cpu->stk, regs.gp_regs[cpu->info.bin_code[i+1]]);
            // reg
            if (cpu->info.bin_code[i + 1] == RAX)
                StackPush (&cpu->stk, cpu->reg.rax);
            else if (cpu->info.bin_code[i + 1] == RBX)
                StackPush (&cpu->stk, cpu->reg.rbx);
            else if (cpu->info.bin_code[i + 1] == RCX)
                StackPush (&cpu->stk, cpu->reg.rcx);
            else if (cpu->info.bin_code[i + 1] == RDX)
                StackPush (&cpu->stk, cpu->reg.rdx);
            else
                StackPush (&cpu->stk, cpu->info.bin_code[i + 1]);
            i++;
        }
        else if (cpu->info.bin_code[i] == RET)
        {
            StackPop (&rets, &i);
        }
        else if (cpu->info.bin_code[i] == CALL)
        {
            StackPush (&rets, cpu->info.bin_code[i + 2]);
            i = cpu->info.bin_code[i + 1];
        }
        else if (cpu->info.bin_code[i] == PUSHRAM)
        {
            if (cpu->info.bin_code[i + 1] == RAX)
                StackPush (&cpu->stk, ram[cpu->reg.rax]);
            else if (cpu->info.bin_code[i + 1] == RBX)
                StackPush (&cpu->stk, ram[cpu->reg.rbx]);
            else if (cpu->info.bin_code[i + 1] == RCX)
                StackPush (&cpu->stk, ram[cpu->reg.rcx]);
            else if (cpu->info.bin_code[i + 1] == RDX)
                StackPush (&cpu->stk, ram[cpu->reg.rdx]);
            else
                StackPush (&cpu->stk, ram[cpu->info.bin_code[i + 1]]);
            i++;
        }
        else if (cpu->info.bin_code[i] == POPRAM)
        {
            if (cpu->info.bin_code[i + 1] == RAX)
                StackPop (&cpu->stk, &ram[cpu->reg.rax]);
            else if (cpu->info.bin_code[i + 1] == RBX)
                StackPop (&cpu->stk, &ram[cpu->reg.rbx]);
            else if (cpu->info.bin_code[i + 1] == RCX)
                StackPop (&cpu->stk, &ram[cpu->reg.rcx]);
            else if (cpu->info.bin_code[i + 1] == RDX)
                StackPop (&cpu->stk, &ram[cpu->reg.rdx]);
            else
                StackPop (&cpu->stk, &ram[cpu->info.bin_code[i + 1]]);
            i++;
        }
        if (cpu->info.bin_code[i] == SKIPER)
        {
            ;
        }
        else if (cpu->info.bin_code[i] == JB)
        {
            Elem_t ref = 0;
            Elem_t counter = 0;
            StackPop (&cpu->stk, &ref);
            StackPop (&cpu->stk, &counter);
            if (ref < counter)
            {
                i = cpu->info.bin_code[i + 1];
            }
        }
        else if (cpu->info.bin_code[i] == JMP)
        {
            i = cpu->info.bin_code[i + 1];
        }
        else if (cpu->info.bin_code[i] == DIV)
        {
            Elem_t tmp_1 = 0;
            Elem_t tmp_2 = 0;
            StackPop (&cpu->stk, &tmp_1);
            StackPop (&cpu->stk, &tmp_2);
            StackPush (&cpu->stk, tmp_2 / tmp_1);
        }
        else if (cpu->info.bin_code[i] == SUB)
        {
            Elem_t tmp_1 = 0;
            Elem_t tmp_2 = 0;
            StackPop (&cpu->stk, &tmp_1);
            StackPop (&cpu->stk, &tmp_2);
            StackPush (&cpu->stk, tmp_2 - tmp_1);
        }
        else if (cpu->info.bin_code[i] == OUT)
        {
            Elem_t out = 0;
            StackPop (&cpu->stk, &out);
            printf ("%d", out);
        }
        else if (cpu->info.bin_code[i] == ADD)
        {
            Elem_t tmp_1 = 0;
            Elem_t tmp_2 = 0;
            StackPop (&cpu->stk, &tmp_1);
            StackPop (&cpu->stk, &tmp_2);
            StackPush (&cpu->stk, tmp_2 + tmp_1);
        }
        else if (cpu->info.bin_code[i] == MUL)
        {
            Elem_t tmp_1 = 0;
            Elem_t tmp_2 = 0;
            StackPop (&cpu->stk, &tmp_1);
            StackPop (&cpu->stk, &tmp_2);
            StackPush (&cpu->stk, tmp_2 * tmp_1);
        }
        else if (cpu->info.bin_code[i] == SQRT)
        {
            Elem_t tmp = 0;
            StackPop (&cpu->stk, &tmp);
            StackPush (&cpu->stk, sqrt (tmp));
        }
        else if (cpu->info.bin_code[i] == IN)
        {
            int value = 0;
            scanf ("%d", &value);
            StackPush (&cpu->stk, value);
        }
        else if (cpu->info.bin_code[i] == POP)
        {
            if (cpu->info.bin_code[i + 1] == RAX)
                StackPop (&cpu->stk, &(cpu->reg.rax));
            else if (cpu->info.bin_code[i + 1] == RBX)
                StackPop (&cpu->stk, &(cpu->reg.rbx));
            else if (cpu->info.bin_code[i + 1] == RCX)
                StackPop (&cpu->stk, &(cpu->reg.rcx));
            else if (cpu->info.bin_code[i + 1] == RDX)
                StackPop (&cpu->stk, &(cpu->reg.rdx));
            i++;
        }
        else if (cpu->info.bin_code[i] == HLT)
        {
            StackDtor (&cpu->stk);
            free (cpu->info.buf);
            fclose (cpu->info.file);
        }
    }
}

