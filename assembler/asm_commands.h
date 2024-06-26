#ifdef CHECK_JMP
CHECK_JMP("call", CALL)
CHECK_JMP("jb", JB)
CHECK_JMP("ja", JA)
CHECK_JMP("jae", JAE)
CHECK_JMP("jbe", JBE)
CHECK_JMP("je", JE)
CHECK_JMP("jne", JNE)
CHECK_JMP("jmp", JMP)
#endif

#ifdef NO_ARG_COMMANDS
NO_ARG_COMMANDS("div", DIV)
NO_ARG_COMMANDS("ret", RET)
NO_ARG_COMMANDS("sub", SUB)
NO_ARG_COMMANDS("out", OUT)
NO_ARG_COMMANDS("hlt", HLT)
NO_ARG_COMMANDS("add", ADD)
NO_ARG_COMMANDS("mul", MUL)
NO_ARG_COMMANDS("sqrt", SQRT)
NO_ARG_COMMANDS("in", IN)
#endif

#ifdef REG_TRANS
REG_TRANS("rax", RAX)
REG_TRANS("rbx", RBX)
REG_TRANS("rcx", RCX)
REG_TRANS("rdx", RDX)
#endif
