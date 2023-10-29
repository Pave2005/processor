#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/init_assemble_string.h"
#include "../common/stack/stack_cd.h"

size_t size = 10;
void CheckSize (AssemblerFile* info, size_t* size, int j);
void FileCtor (AssemblerFile* info);
void ExecuteAssembler (AssemblerFile* info, int* k);
void FileDtor (AssemblerFile* info);
int Hash (char* str);

int main ()
{
    AssemblerFile info = {};

    FileCtor (&info);
    int j = 0;
    ExecuteAssembler (&info, &j);
    fwrite (info.bin_code, sizeof (int), j, info.assembler_file);
    for (int i = 0; i < j; i++)
        printf ("%d\n", info.bin_code[i]);
    FileDtor (&info);
}


void FileDtor (AssemblerFile* info)
{
    free (info->bin_code);
    free (info->buf);
    fclose (info->assembler_file);
    fclose (info->file);
}

void CheckSize (AssemblerFile* info, size_t* size, int j)
{
    if (j >= *size)
    {
        size_t resize = *size * 2;
        info->bin_code = (int*)realloc (info->bin_code, resize);
        *size = resize;
    }
}

void FileCtor (AssemblerFile* info)
{
    info->file = fopen ("assembler/assemble.txt", "r");
    info->assembler_file = fopen ("CPU/assembler_file.bin", "wb");
    info->file_size = FileSize (*info);
    info->buf = ReadText (*info);
    info->bin_code = (int*)calloc (size, sizeof (int));
    Lines (info);
}

void ExecuteAssembler (AssemblerFile* info, int* k)
{
    int num_arg = 0;
    char str_arg[5] = "";
    char command[5] = {};
    Labels labels[100] = {};
    for (size_t i = 0; i < 100; i++)
    {
        labels[i].adress = -1;
        labels[i].hash = -1;
    }
    int j = 0;
    int label_elem = 0;
    for (int time = 0; time < 2; time++)
    {
        int i = 0;
        for (; i < info->n_lines; i++)
        {
            CheckSize (info, &size, j);
            if (sscanf (info->buf + info->lines[i], "%s %d", command, &num_arg) == 2)
            {
                if (time != 0)
                {
                    if (strcmp (command, "push") == 0)
                    {
                        info->bin_code[j] = PUSH;
                        info->bin_code[j + 1] = num_arg;
                    }
                }
                j += 2;
            }
            else if (sscanf (info->buf + info->lines[i], "%s %s", command, str_arg) == 2)
            {
                if (strcmp (command, "call") == 0)
                {
                    if (time != 0)
                    {
                        info->bin_code[j] = CALL;
                        size_t elem = 0;
                        while (labels[elem].hash != Hash (str_arg + 1))
                        {
                            elem++;
                            if (elem == 100)
                            {
                                info->bin_code[j + 1] = -1;
                                break;
                            }
                        }
                        if (elem != 100)
                        {
                            info->bin_code[j + 1] = labels[elem].adress;
                        }
                        info->bin_code[j + 2] = j + 3;
                    }
                    j++;
                }
                else if (time != 0)
                {
                    if (strcmp (command, "jb") == 0)
                    {
                        info->bin_code[j] = JB;
                        size_t elem = 0;
                        while (labels[elem].hash != Hash (str_arg + 1))
                        {
                            elem++;
                            if (elem == 100)
                            {
                                info->bin_code[j + 1] = -1;
                                break;
                            }
                        }
                        if (elem != 100)
                        {
                            info->bin_code[j + 1] = labels[elem].adress;
                        }
                    }
                    else if (strcmp (command, "jmp") == 0)
                    {
                        info->bin_code[j] = JMP;
                        size_t elem = 0;
                        while (labels[elem].hash != Hash (str_arg + 1))
                        {
                            elem++;
                            if (elem == 100)
                            {
                                info->bin_code[j + 1] = -1;
                                break;
                            }
                        }
                        if (elem != 100)
                        {
                            info->bin_code[j + 1] = labels[elem].adress;
                        }
                    }
                    else if (str_arg[0] == '[')
                    {
                        if (strcmp (command, "push") == 0)
                        {
                            info->bin_code[j] = PUSHRAM;
                        }
                        else if (strcmp (command, "pop") == 0)
                        {
                            info->bin_code[j] = POPRAM;
                        }
                        size_t g = 1;
                        char* arg = (char*)calloc (3, sizeof (char));
                        while (str_arg[g] != ']')
                        {
                            arg[g - 1] = str_arg[g];
                            g++;
                        }
                        if (strcmp (arg, "rax") == 0)
                        {
                            info->bin_code[j + 1] = RAX;
                        }
                        else if (strcmp (arg, "rbx") == 0)
                        {
                            info->bin_code[j + 1] = RBX;
                        }
                        else if (strcmp (arg, "rcx") == 0)
                        {
                            info->bin_code[j + 1] = RCX;
                        }
                        else if (strcmp (arg, "rdx") == 0)
                        {
                            info->bin_code[j + 1] = RDX;
                        }
                        else
                        {
                            info->bin_code[j + 1] = atoi (arg);
                        }
                        free (arg);
                    }
                    else if (strcmp (command, "pop") == 0)
                    {
                        info->bin_code[j] = POP;
                        if (strcmp (str_arg, "rax") == 0)
                        {
                            info->bin_code[j + 1] = RAX;
                        }
                        else if (strcmp (str_arg, "rbx") == 0)
                        {
                            info->bin_code[j + 1] = RBX;
                        }
                        else if (strcmp (str_arg, "rcx") == 0)
                        {
                            info->bin_code[j + 1] = RCX;
                        }
                        else if (strcmp (str_arg, "rdx") == 0)
                        {
                            info->bin_code[j + 1] = RDX;
                        }
                    }
                    else
                    {
                        info->bin_code[j] = PUSH;
                        if (strcmp (str_arg, "rax") == 0)
                        {
                            info->bin_code[j + 1] = RAX;
                        }
                        else if (strcmp (str_arg, "rbx") == 0)
                        {
                            info->bin_code[j + 1] = RBX;
                        }
                        else if (strcmp (str_arg, "rcx") == 0)
                        {
                            info->bin_code[j + 1] = RCX;
                        }
                        else if (strcmp (str_arg, "rdx") == 0)
                        {
                            info->bin_code[j + 1] = RDX;
                        }
                    }
                }
                j += 2;
            }
            else
            {
                if (command[0] == ':')
                {
                    if (time == 0)
                    {
                        labels[label_elem].hash = Hash (command + 1);
                        labels[label_elem].adress = j + 1;
                        label_elem++;
                    }
                    info->bin_code[j] = SKIPER;
                }
                else if (time != 0)
                {
                    if (strcmp (command, "div") == 0)
                    {
                        info->bin_code[j] = DIV;
                    }
                    else if (strcmp (command, "ret") == 0)
                    {
                        info->bin_code[j] = RET;
                    }
                    else if (strcmp (command, "sub") == 0)
                    {
                        info->bin_code[j] = SUB;
                    }
                    else if (strcmp (command, "out") == 0)
                    {
                        info->bin_code[j] = OUT;
                    }
                    else if (strcmp (command, "hlt") == 0)
                    {
                    info->bin_code[j] = HLT;
                    }
                    else if (strcmp (command, "add") == 0)
                    {
                        info->bin_code[j] = ADD;
                    }
                    else if (strcmp (command, "mul") == 0)
                    {
                        info->bin_code[j] = MUL;
                    }
                    else if (strcmp (command, "sqrt") == 0)
                    {
                        info->bin_code[j] = SQRT;
                    }
                    else if (strcmp (command, "in") == 0)
                    {
                        info->bin_code[j] = IN;
                    }
                }
                j++;
            }
        }
        if (time == 0)
        {
            label_elem = 0;
            j = 0;
        }
    }
    *k = j;
}

int Hash (char* str)
{
    int sum = 0;
    for (int i = 0; str[i] != '\0'; i++)
    {
        sum += (int)str[i];
    }
    return sum;
}
