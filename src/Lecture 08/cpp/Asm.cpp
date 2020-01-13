// Asm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
    char src[100] = "6b15ac8f", dst[100];

    __asm
    {
            pusha
            lea ecx, src
            lea edx, dst

            mov esi, 5
        label_main_loop:
            mov al, [ecx]
            cmp al, 0x39
            ja label_non_digit1
        label_digit1:
            sub al, 0x30
            jmp label_value1
        label_non_digit1:
            sub al, 0x57
        label_value1:
            shl al, 4
            inc ecx
            mov bl, [ecx]
            cmp bl, 0x39
            ja label_non_digit2
        label_digit2:
            sub bl, 0x30
            jmp label_value2
        label_non_digit2:
            sub bl, 0x57
        label_value2:
            add al, bl
            mov [edx], al
            inc ecx
            inc edx
            dec esi
            jnz label_main_loop

            popa
    }
	return 0;
}

