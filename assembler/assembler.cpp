#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../common/asm_file.h"
#include "../common/stack/stack_cd.h"
#include "../assembler/assembler.h"
#include "../common/isa.h"

const size_t labels_count = 100;
const size_t max_cmd_len = 21;

void CountOfLines (AssemblerFile* self)
{
    size_t n_lines = 0;
    self->buf[self->file_size] = '\0';
    for (size_t i = 0; i < self->file_size; i++)
    {
        if (self->buf[i] == '\n')
        {
            n_lines++;
            self->buf[i] = '\0';
        }
    }
    self->n_lines = n_lines;
}

void CheckSize (AssemblerFile* asm_file, int cmd_count)
{
    if (cmd_count >= asm_file->size)
    {
        ReallocUpBinCode (asm_file, cmd_count);
    }
}

void ReallocUpBinCode (AssemblerFile* asm_file, int cmd_count)
{
    asm_file->size *= 2;
    asm_file->bin_code = (Elem_t*)realloc (asm_file->bin_code, asm_file->size);
    for (int i = cmd_count + 1; i < asm_file->size; i++)
    {
        asm_file->bin_code[i] = 0;
    }
}

void AssemblerFileCtor (AssemblerFile* self, const int cmd_line_status)
{
    if (cmd_line_status == RUN_SQR)
    {
        self->file = fopen ("assembler/assemble.txt", "r");
        self->assembler_file = fopen ("CPU/assembler_file.bin", "wb");
    }
    else if (cmd_line_status == RUN_FCT)
    {
        self->file = fopen ("assembler/factorial.txt", "r");
        self->assembler_file = fopen ("CPU/factorial_file.bin", "wb");
    }
    else
    {
        self->status |= (1 << ERR_INSTRUCTION);
        return;
    }
    self->file_size = FileSize (self->file);
    self->buf = ReadText (self->file_size, self->file);
    self->status = 0;
    self->size = 100;
    self->bin_code = (Elem_t*)calloc (self->size, sizeof (int));
    CountOfLines (self);
}

void AssemblerFileDtor (AssemblerFile* self)
{
    free (self->bin_code);
    fclose (self->assembler_file);
    fclose (self->file);
}

void ExecuteAssembler (AssemblerFile* self, int* count)
{
    Labels* labels = (Labels*)calloc (labels_count + 1, sizeof (Labels));
    for (size_t i = 0; i < labels_count; i++)
    {
        labels[i].adress = NOT_FOUND;
        labels[i].hash = NOT_FOUND;
    }
    TranslateAsmToBin (FirstPass, self, labels, count);
    if (self->status == 0)
    {
        TranslateAsmToBin (SecondPass, self, labels, count);
    }
}

void TranslateAsmToBin (enum pass_num pass, AssemblerFile* self, Labels* labels, int* count)
{
    int cmd_count = 0;
    int num_arg = 0;

    char str_arg[max_cmd_len] = "";
    char command[max_cmd_len] = "";

    int count_read = 0;

    if (pass == FirstPass)
    {
        int label_elem = 0;
        for (int line_num = 0; line_num < self->n_lines; line_num++)
        {
            int scaned_count = 0;
            CheckSize (self, cmd_count);
            if (sscanf (self->buf + count_read, "%20s %d%n", command, &num_arg, &scaned_count) == 2)
            {
                count_read += scaned_count + 1;
                cmd_count += 2;
            }
            else if (sscanf (self->buf + count_read, "%20s %20s%n", command, str_arg, &scaned_count) == 2)
            {
                count_read += scaned_count + 1;
                cmd_count += 2;
            }
            // %[^:]
            else if (sscanf (self->buf + count_read, "%20s%n", command, &scaned_count) == 1)
            {
                if (command[0] == ':')
                {
                    PushLabel (labels, &label_elem, command, cmd_count);
                }
                count_read += scaned_count + 1;
                cmd_count++;
            }
            else
            {
                count_read++;
            }
        }
        CheckLabels (labels, self);
    }
    if (pass == SecondPass)
    {
        for (int line_num = 0; line_num < self->n_lines; line_num++)
        {
            int scaned_count = 0;
            CheckSize (self, cmd_count);
            if (sscanf (self->buf + count_read, "%20s [%d]%n", command, &num_arg, &scaned_count) == 2)
            {
                if (TranslateImmBasedAddr (self, command, cmd_count, num_arg) == 1)
                    return;
                count_read += scaned_count + 1;
                cmd_count += 2;
            }
            else if (sscanf (self->buf + count_read, "%20s %d%n", command, &num_arg, &scaned_count) == 2)
            {
                if (strcmp (command, "push") == 0)
                {
                    self->bin_code[cmd_count] = PUSH;
                    self->bin_code[cmd_count + 1] = num_arg;
                }
                else
                {
                    self->status |= (1 << ERR_INPUT);
                    return;
                }
                count_read += scaned_count + 1;
                cmd_count += 2;
            }
            else if (sscanf (self->buf + count_read, "%20s [%20[^]]%n", command, str_arg, &scaned_count) == 2)
            {
                if (TranslateRegBasedAddr (self, command, cmd_count, str_arg) == 1)
                    return;
                count_read += scaned_count + 1;
                cmd_count += 2;
            }
            else if (sscanf (self->buf + count_read, "%20s %20s%n", command, str_arg, &scaned_count) == 2)
            {
                if (TranslateControlFlowInstr (self, command, cmd_count) == 1)
                {
                    if (str_arg[0] != ':')
                    {
                        self->status |= (1 << ERR_LABEL);
                        return;
                    }
                    self->bin_code[cmd_count + 1] = SearchLabelAddr (labels, str_arg);
                }
                else
                {
                    if (TranslateRegArgInstr (self, command, cmd_count, str_arg) == 1)
                        return;
                }
                count_read += scaned_count + 1;
                cmd_count += 2;
            }
            else if (sscanf (self->buf + count_read, "%20s%n", command, &scaned_count) == 1)
            {
                if (TranslateNoArgInstr (self, command, &cmd_count) == 1)
                    return;
                count_read += scaned_count + 1;
                cmd_count++;
            }
            else
            {
                count_read++;
            }
        }
        *count = cmd_count;
        free (self->buf);
    }
}

void PushLabel (Labels* labels, int* label_elem, char* command, int cmd_count)
{
    labels[*label_elem].hash = Hash (command + 1);
    labels[*label_elem].adress = cmd_count + 1 - *label_elem - 1;
    (*label_elem)++;
}

int TranslateImmBasedAddr (AssemblerFile* self, char command[], int cmd_count, int num_arg)
{
    self->bin_code[cmd_count + 1] = num_arg;
    if (strcmp (command, "push") == 0)
    {
        self->bin_code[cmd_count] = PUSHRAM;
    }
    else if (strcmp (command, "pop") == 0)
    {
        self->bin_code[cmd_count] = POPRAM;
    }
    else
    {
        self->status |= (1 << ERR_RAM_CMD);
        return 1;
    }
    return -1;
}

int TranslateRegBasedAddr (AssemblerFile* self, char command[], int cmd_count, char str_arg[])
{
    if (TranslateRegisters (str_arg, self, cmd_count) == 1)
        return 1;
    if (strcmp (command, "push") == 0)
    {
        self->bin_code[cmd_count] = PUSHRAMREG;
    }
    else if (strcmp (command, "pop") == 0)
    {
        self->bin_code[cmd_count] = POPRAMREG;
    }
    else
    {
        self->status |= (1 << ERR_RAM_CMD);
        return 1;
    }
    return -1;
}

int TranslateNoArgInstr (AssemblerFile* self, char command[], int* cmd_count)
{
    if (command[0] == ':')
    {
        (*cmd_count)--;
    }

    #define NO_ARG_COMMANDS(name, code)     \
    else if (strcmp (command, name) == 0)   \
    {                                       \
        self->bin_code[*cmd_count] = code;  \
    }

    #include "asm_commands.h"
    #undef NO_ARG_COMMANDS

    else
    {
        self->status |= (1 << ERR_INPUT);
        return 1;
    }
    return -1;
}

int TranslateRegArgInstr (AssemblerFile* self, char command[], int cmd_count, char str_arg[])
{
    if (strcmp (command, "pop") == 0)
    {
        self->bin_code[cmd_count] = POP;
        if (TranslateRegisters (str_arg, self, cmd_count) == 1)
            return 1;
    }
    else if (strcmp (command, "push") == 0)
    {
        self->bin_code[cmd_count] = PUSHREG;
        if (TranslateRegisters (str_arg, self, cmd_count) == 1)
            return 1;
    }
    else
    {
        self->status |= (1 << ERR_INPUT);
        return 1;
    }
    return -1;
}

int TranslateControlFlowInstr (AssemblerFile* self, char command[], int cmd_count)
{
    #define CHECK_JMP(name, code)          \
    if (strcmp (command, name) == 0)       \
    {                                      \
        self->bin_code[cmd_count] = code;  \
        return 1;                          \
    }

    #include "asm_commands.h"
    #undef CHECK_JMP

    return -1;
}

void CheckLabels (Labels* labels, AssemblerFile* self)
{
    for (size_t i = 0; labels[i].adress != NOT_FOUND; i++)
    {
        for (size_t j = i + 1; labels[j].adress != NOT_FOUND; j++)
        {
            if (labels[i].hash == labels[j].hash)
            {
                self->status |= (1 << ERR_TWO_SAME_LABEL);
            }
        }
    }
}

unsigned int SearchLabelAddr (Labels* labels, char str_arg[])
{
    size_t elem = 0;
    while (labels[elem].hash != Hash(str_arg + 1) && elem < labels_count)
    {
        elem++;
    }
    if (elem == labels_count)
    {
        printf("%s label not found\n", str_arg);
        exit(-1);
    }
    return labels[elem].adress;
}

int TranslateRegisters (char* str_arg, AssemblerFile* self, int cmd_count)
{
    // DEF_REGS
    #define REG_TRANS(name, code)               \
    if (strcmp (str_arg, name) == 0)            \
    {                                           \
        self->bin_code[cmd_count + 1] = code;   \
        return -1;                              \
    }

    #include "asm_commands.h"
    #undef REG_TRANS

    else
    {
        self->status |= (1 << ERR_INCORRECT_REG);
        return 1;
    }
}

unsigned long long Hash (char* str)
{
    int number[10] = {};
    size_t i = 0;
    for (; str[i] != '\0'; i++)
        number[i] = (int)str[i];
    const unsigned int m = 0x5bd1e995;
    const unsigned int seed = 0;
    const int r = 24;
    unsigned long long len = (unsigned long long)(i);
    unsigned long long h = seed ^ len;
    const unsigned char* data = (const unsigned char*)(&number);
    unsigned int k = 0;

    while (len >= 4)
    {
        k  = data[0];
        k |= data[1] << 8;
        k |= data[2] << 16;
        k |= data[3] << 24;
        k *= m;
        k ^= k >> r;
        k *= m;
        h *= m;
        h ^= k;
        data += 4;
        len -= 4;
    }
    switch (len)
    {
        case 3:
        h ^= data[2] << 16;
        case 2:
          h ^= data[1] << 8;
        case 1:
          h ^= data[0];
          h *= m;
    };
    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;
    return h;
}

void AssemblerVerifier (AssemblerFile* self)
{
    if (self->status == 0)
        printf ("Programm is ok!\n");
    else
    {
        if ((self->status & 1) == 1)
            printf ("%sIncorrect command input\n", RED);
        if ((self->status & 2) == 2)
            printf ("%sIncorrect label designation in commands\n", RED);
        if ((self->status & 4) == 4)
            printf ("%sThe argument of the command working with RAM was entered incorrectly\n", RED);
        if ((self->status & 8) == 8)
            printf ("%sIncorrect command working with RAM\n", RED);
        if ((self->status & 16) == 16)
            printf ("%sIncorrect register name\n", RED);
        if ((self->status & 32) == 32)
            printf ("%sYou cannot name labels by the same name\n", RED);
        if ((self->status & 64) == 64)
            printf ("%sIncorrect command line instruction\n", RED);
    }
}
