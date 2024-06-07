#ifndef INIT
#define INIT

#include <stdio.h>
#include "../common/stack/stack_cd.h"

#define RED "\x1b[31;41m"

enum REGISTER_CODES
{
    RAX = 10,
    RBX = 11,
    RCX = 12,
    RDX = 13,
};

enum CMD_LINE_STATUS
{
    RUN_SQR = 1,
    RUN_FCT = 2,
    MEM = 3,
};

int HandleCommandLine (int argc, const char *argv[]);
Elem_t* ReadProcessorText (long file_size, FILE* file);
char* ReadText (long file_size, FILE* file);
long FileSize (FILE* file);

#endif
