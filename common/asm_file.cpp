#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "asm_file.h"

int HandleCommandLine (int argc, const char *argv[])
{
    if (argc > 1)
    {
        if (strcmp ("sqr", argv [1]) == 0)
        {
            return RUN_SQR;
        }
        else if (strcmp ("fct", argv [1]) == 0)
        {
            return RUN_FCT;
        }
    }
    return 0;
}

char* ReadText (long file_size, FILE* file)
{
    char* buf = (char*)calloc (file_size + 1, sizeof (char));
    fread (buf, sizeof (char), file_size, file);
    return buf;
}

Elem_t* ReadProcessorText (long file_size, FILE* file)
{
    Elem_t* buf = (Elem_t*)calloc (file_size + 1, sizeof (char));
    fread (buf, sizeof (char), file_size, file);
    return buf;
}

long FileSize (FILE* file)
{
    fseek (file, 0, SEEK_END);
    long size = ftell (file);
    fseek (file, 0, SEEK_SET);
    return size;
}
