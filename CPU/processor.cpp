#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "../common/stack/stack_cd.h"
#include "../common/asm_file.h"
#include "../CPU/processor.h"
#include "../common/isa.h"

const size_t width = 100;
const size_t height = 30;

void VideoMemory (Elem_t ram[])
{
    printf ("\n");
    for (int i = 0; i < width * height; i++)
    {
        if (i % width == 0)
            printf ("\n");
        printf ("%f", ram[i]);
    }
}

long LoadBinary (Elem_t** binary_name, const int cmd_line_status)
{
    FILE* file = NULL;
    if (cmd_line_status == RUN_SQR)
    {
        file = fopen ("CPU/assembler_file.bin", "rb");
    }
    else if (cmd_line_status == RUN_FCT)
    {
        file = fopen ("CPU/factorial_file.bin", "rb");
    }
    else
    {
        return -1;
    }
    long file_size = FileSize (file);
    *binary_name = ReadProcessorText (file_size, file);
    fclose (file);
    return file_size;
}

void CpuExecute (CPU* cpu, Elem_t* binary_name, long file_size, Elem_t* ram)
{
    int cmd_size = 4;
    stack rets = {};
    StackCtor (&(cpu->stk), 5);
    StackCtor (&rets, 5);
    for (int i = 0; i < file_size / cmd_size; i++)
    {
        switch ((int)binary_name[i])
        {
            #define DEF_CMD(cmd_code, ...) \
                case cmd_code:             \
                {                          \
                    __VA_ARGS__            \
                    break;                 \
                }

            #include "cpu_commands.h"
            #undef DEF_CMD
        }
    }
}
