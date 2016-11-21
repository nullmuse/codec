#include <stdio.h>


float float_ritual(int object) {
//Below is to make compiler happy
object = object;
__asm__(".intel_syntax noprefix");
__asm__("fld DWORD PTR [rbp-0x4]");
__asm__("fst DWORD PTR [rbp-0x32]");
__asm__("movd xmm0, DWORD PTR [rbp-0x32]");
__asm__("pop rbp");
__asm__("ret");
__asm__(".att_syntax noprefix");
//Below is to make compiler happy
return object;
}

double double_ritual(unsigned long object) {
//Below is to make compiler happy
object = object;
__asm__(".intel_syntax noprefix");
__asm__("fld QWORD PTR [rbp-0x8]");
__asm__("fst QWORD PTR [rbp-0x64]");
__asm__("movq xmm0, QWORD PTR [rbp-0x64]");
__asm__("pop rbp");
__asm__("ret");
__asm__(".att_syntax noprefix");
//Below is to make compiler happy
return object;
}



int byte_ritual(int object) {
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

unsigned long long_ritual(unsigned long object) {
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

