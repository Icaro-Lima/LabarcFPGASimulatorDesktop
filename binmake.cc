/* How to use 
   ==========

Prepare a file called riscv.s with the following content:
.section .text
.globl main
main:
        nop
------- EOF ------------

Then run the following commands with inst.101 as input: */
//  make binmake
//  riscv-gcc -nostartfiles -T$RISCV/link.ld riscv.s -o riscv.out
//  grep ^[01] inst.101 | sed 's/;.*//g' | sed 's/ //g' | ./binmake
//  riscv32-unknown-elf-objcopy --update-section .text=a.bin --strip-symbol=_end riscv.out a.out

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
   char *line;
   size_t size;
   unsigned int b;
   FILE *fout;

   fout = fopen("a.bin","wb");
   while (getline(&line, &size, stdin) != -1) {
     b = strtol(line, NULL, 2);
     fwrite(&b,sizeof(b),1,fout);
   }
   fclose(fout);
   return 0;
}
