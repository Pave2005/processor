#ifndef ASSEM
#define ASSEM
#include "../common/asm_file.h"

struct Labels
{
    unsigned long long hash;
    unsigned int adress;
};

struct AssemblerFile
{
    long file_size;
    FILE* assembler_file;
    char* buf;
    size_t n_lines;

    FILE* file;
    Elem_t* bin_code;
    size_t size;

    unsigned long long status;
};

enum pass_num
{
    FirstPass = 111,
    SecondPass = 222,
};

enum LABELS
{
    NOT_FOUND = -2,
};

enum ASSEMBLER_ERROR
{
    ERR_INPUT = 0,              // неправильный ввод команды
    ERR_LABEL = 1,              // неправильные аргумент для меток, нет :
    ERR_RAM_ARG = 2,            // неправильно введенный аргумент в команды с оперативной памятью
    ERR_RAM_CMD = 3,            // некорректно введена команды работы с памятью
    ERR_INCORRECT_REG = 4,      // неправильное название регистра
    ERR_TWO_SAME_LABEL = 5,     // два одинаковых названия меток
    ERR_INSTRUCTION = 6,        // не правильная инструкция в командной строке
};

void PushLabel (Labels* labels, int* label_elem, char* command, int cmd_count);
int TranslateImmBasedAddr (AssemblerFile* self, char command[], int cmd_count, int num_arg);
int TranslateRegBasedAddr (AssemblerFile* self, char command[], int cmd_count, char str_arg[]);
int TranslateNoArgInstr (AssemblerFile* self, char command[], int* cmd_count);
int TranslateRegArgInstr (AssemblerFile* self, char command[], int cmd_count, char str_arg[]);
int TranslateControlFlowInstr (AssemblerFile* self, char command[], int cmd_count);
void CheckLabels (Labels* labels, AssemblerFile* self);
void ReallocUpBinCode (AssemblerFile* assem, int cmd_count);
void AssemblerVerifier (AssemblerFile* self);
int TranslateRegisters (char* str_arg, AssemblerFile* self, int cmd_count);
void CheckSize (AssemblerFile* assem, int cmd_count);
void AssemblerFileCtor (AssemblerFile* self, const int cmd_line_status);
void ExecuteAssembler (AssemblerFile* self, int* count);
void AssemblerFileDtor (AssemblerFile* self);
unsigned long long Hash (char* str);
void TranslateAsmToBin (enum pass_num pass, AssemblerFile* self, Labels* labels, int* count);
unsigned int SearchLabelAddr (Labels* labels, char str_arg[]);
void CountOfLines (AssemblerFile* self);

#endif
