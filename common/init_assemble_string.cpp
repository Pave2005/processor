#include <stdio.h>
#include <stdlib.h>
#include "init_assemble_string.h"

char* ReadText (AssemblerFile info)
{
    char* buf = (char*)calloc (info.file_size + 1, sizeof (char));
    fread (buf, sizeof (char), info.file_size, info.file);
    return buf;
}

long FileSize (AssemblerFile info)
{
    fseek (info.file, 0, SEEK_END);
    long size = ftell (info.file);
    fseek (info.file, 0, SEEK_SET);
    return size;
}

void Lines (AssemblerFile* info)
{
    size_t n_lines = 0;
    int* lines = (int*)calloc (2, sizeof (int));
    *lines = 0;
    info->buf[info->file_size] = '\0';
    for (size_t i = 0; i < info->file_size; i++)
    {
        if (info->buf[i] == '\n')
        {
            *(lines + n_lines + 1) = i + 1;
            n_lines++;
            info->buf[i] = '\0';
            lines = (int*)realloc (lines, n_lines + 2);
        }
    }
    info->n_lines = n_lines;
    info->lines = lines;
}
