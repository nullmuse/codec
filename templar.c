#include <stdio.h>
#include <string.h>

int transmuteChar(char *subject) {
   int len = strlen(subject) - 1;
   int i,new = 0;

   for(i = 1;len >= 0;--len,i *= 10) {
      //built in handling of non-integer input
      if(subject[len] > 0x39 || subject[len] < 0x30) {
         return 0;
      }
      new += (subject[len] - 0x30) * i;
  }
   return new;
 }


float floatRitual(int object) {
//Below is to make compiler happy
object = object;
__asm__(".intel_syntax noprefix");
__asm__("finit");
__asm__("fld DWORD PTR [rbp-0x4]");
__asm__("fst DWORD PTR [rbp-0x32]");
__asm__("movd xmm0, DWORD PTR [rbp-0x32]");
__asm__("pop rbp");
__asm__("ret");
__asm__(".att_syntax noprefix");
//Below is to make compiler happy
return object;
}

double doubleRitual(unsigned long object) {
//Below is to make compiler happy
object = object;
__asm__(".intel_syntax noprefix");
__asm__("finit");
__asm__("fld QWORD PTR [rbp-0x8]");
__asm__("fst QWORD PTR [rbp-0x64]");
__asm__("movq xmm0, QWORD PTR [rbp-0x64]");
__asm__("pop rbp");
__asm__("ret");
__asm__(".att_syntax noprefix");
//Below is to make compiler happy
return object;
}



int byteRitual(int object) {
//Below is to make compiler happy
object = object;
__asm__(".intel_syntax noprefix");
__asm__("mov eax, DWORD PTR [rbp-0x4]");
__asm__("bswap eax");
__asm__("pop rbp");
__asm__("ret");
__asm__(".att_syntax noprefix");
//Below is to make compiler happy
return object;
}



unsigned long longRitual(unsigned long object) {
//Below is to make compiler happy
object = object;
__asm__(".intel_syntax noprefix");
__asm__("mov rax, QWORD PTR [rbp-0x8]");
__asm__("bswap rax");
__asm__("pop rbp");
__asm__("ret");
__asm__(".att_syntax noprefix");
//Below is to make compiler happy
return object;
}

