#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/asm_file.h"
#include "../common/stack/stack_cd.h"
#include "../assembler/assembler.h"

int main (int argc, const char *argv[])
{
    AssemblerFile self = {};

    AssemblerFileCtor (&self, HandleCommandLine (argc, argv));
    if (self.status != 0)
    {
        AssemblerVerifier (&self);
        return 0;
    }
    int cmd_count = 0;
    ExecuteAssembler (&self, &cmd_count);
    if (self.status != 0)
    {
        AssemblerVerifier (&self);
        return 0;
    }
    fwrite (self.bin_code, sizeof (float), cmd_count, self.assembler_file);
    for (size_t i = 0; i < cmd_count; i++)
        printf ("%f\n", self.bin_code[i]);
    AssemblerFileDtor (&self);
}
