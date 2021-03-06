// Patch.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

char PATCH[100] = {
    0x8b, 0xcf,
    0x8d, 0x55, 0xec,
    0xbe, 0x05, 0x00, 0x00, 0x00, 0x8a, 0x01, 0x3c,
    0x39, 0x77, 0x04, 0x2c, 0x30, 0xeb, 0x02, 0x2c,
    0x57, 0xc0, 0xe0, 0x04, 0x41, 0x8a, 0x19, 0x80,
    0xfb, 0x39, 0x77, 0x05, 0x80, 0xeb, 0x30, 0xeb,
    0x03, 0x80, 0xeb, 0x57, 0x02, 0xc3, 0x88, 0x02,
    0x41, 0x42, 0x4e, 0x75, 0xd8
};

int PATCH_SIZE = 50;
int PATCH_OFFSET = 0xcf2;

int _tmain(int argc, _TCHAR* argv[])
{
    FILE *f;

    fopen_s(&f, "WeakCrypto.exe", "r+");

    if (f == nullptr)
    {
        printf("Can not open file 'WeakCrypto.exe'");
        return 0;
    }

    fseek(f, PATCH_OFFSET, SEEK_SET);
    fwrite(PATCH, 1, PATCH_SIZE, f);

    fclose(f);
    
	return 0;
}

