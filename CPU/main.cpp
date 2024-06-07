#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "../common/stack/stack_cd.h"
#include "../common/asm_file.h"
#include "../CPU/processor.h"

int main (int argc, const char *argv[])
{
    CPU cpu = {};

    Elem_t* binary_name = NULL;

    long file_size = LoadBinary (&binary_name, HandleCommandLine (argc, argv));
    if (file_size == -1)
        printf ("%sIncorrect command line instruction\n", RED);

    Elem_t ram[10000] = {};

    CpuExecute (&cpu, binary_name, file_size, ram);

    //VideoMemory (ram);
}

