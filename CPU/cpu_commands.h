#define HANDLE_JMP(arg)                 \
        Elem_t ref = 0;                 \
        Elem_t counter = 0;             \
        StackPop (&cpu->stk, &ref);     \
        StackPop (&cpu->stk, &counter); \
        if (ref arg counter)            \
        {                               \
            i = binary_name[i + 1] - 1; \
        }                               \
        else                            \
        {                               \
            i++;                        \
        }
#define HANDLE_ARITHMETIC(arg)                  \
        Elem_t tmp_1 = 0;                       \
        Elem_t tmp_2 = 0;                       \
        StackPop (&cpu->stk, &tmp_1);           \
        StackPop (&cpu->stk, &tmp_2);           \
        StackPush (&cpu->stk, tmp_2 arg tmp_1);

DEF_CMD(PUSH,       StackPush (&cpu->stk, binary_name[i + 1]);
                    i++; )
DEF_CMD(PUSHREG,    StackPush (&cpu->stk, cpu->gp_regs[(int)binary_name[i + 1]]);
                    i++; )
DEF_CMD(RET,        float tmp_i = (float)i;
                    StackPop (&rets, &tmp_i);
                    i = (int)tmp_i;
                    i--; )
DEF_CMD(CALL,       StackPush (&rets, i + 2);
                    i = binary_name[i + 1] - 1; )
DEF_CMD(PUSHRAM,    StackPush (&cpu->stk, ram[(int)binary_name[i + 1]]);
                    i++; )
DEF_CMD(PUSHRAMREG, StackPush (&cpu->stk, ram[(int)binary_name[i + 1]]);
                    i++; )
DEF_CMD(POPRAM,     StackPop (&cpu->stk, &ram[(int)binary_name[i + 1]]);
                    i++; )
DEF_CMD(POPRAMREG,  StackPop (&cpu->stk, &ram[(int)binary_name[i + 1]]);
                    i++; )
DEF_CMD(JB,         HANDLE_JMP(<) )
DEF_CMD(JBE,        HANDLE_JMP(<=) )
DEF_CMD(JA,         HANDLE_JMP(>) )
DEF_CMD(JAE,        HANDLE_JMP(>=) )
DEF_CMD(JE,         HANDLE_JMP(==) )
DEF_CMD(JNE,        HANDLE_JMP(!=) )
DEF_CMD(JMP,        i = binary_name[i + 1] - 1; )
DEF_CMD(DIV,        HANDLE_ARITHMETIC(/) )
DEF_CMD(SUB,        HANDLE_ARITHMETIC(-) )
DEF_CMD(OUT,        Elem_t out = 0;
                    StackPop (&cpu->stk, &out);
                    printf ("%f\n", out); )
DEF_CMD(ADD,        HANDLE_ARITHMETIC(+) )
DEF_CMD(MUL,        HANDLE_ARITHMETIC(*) )
DEF_CMD(SQRT,       Elem_t tmp = 0;
                    StackPop (&cpu->stk, &tmp);
                    StackPush (&cpu->stk, sqrt (tmp)); )
DEF_CMD(IN,         Elem_t value = 0;
                    scanf ("%f", &value);
                    StackPush (&cpu->stk, value); )
DEF_CMD(POP,        StackPop (&cpu->stk, &cpu->gp_regs[(int)binary_name[i+1]]);
                    i++; )
DEF_CMD(HLT,        StackDtor (&cpu->stk);
                    free (binary_name); )
#undef HANDLE_JMP
#undef HANDLE_ARITHMETIC

// #define POP() StackPop(&cpu->stk)
                // #define PUSH(elem) StackPush(&cpu->stk, elem)
                // PUSH(POP() * POP())
