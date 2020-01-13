// WeakCrypto.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "md5.h"
#include "river.h"


void decrypt_file(
    const char* file_name,
    const uint8_t* pwd
    )
{
    char res_file_name[200];

    strcpy_s(res_file_name, 200, file_name);
    strcat_s(res_file_name, 200, ".decrypt");

    FILE *fi, *fo;

    fopen_s(&fi, file_name, "rb");

    if (fi == nullptr)
    {
        printf("Can not open file %s.\n", file_name);
        return;
    }

    River river;
    uint8_t cache[16];

    Md5::calc_hash(pwd, strlen((const char*)pwd), cache);
    for (int i = 0; i < 10000; i++)
        Md5::calc_hash(cache, 16, cache);
    river.set_parameters(cache[4], *(uint16_t *)cache, *(uint16_t *)(cache + 2));

    for (int i = 0; i < 5; i++)
    {
        uint8_t c = getc(fi);
        if (c != (cache[i] ^ river.generate_byte()))
        {
            printf("Wrong password.\n");
            fclose(fi);
            return;
        }
    }

    fopen_s(&fo, res_file_name, "wb");

    if (fo == nullptr)
    {
        printf("Can not create file %s.\n", res_file_name);
        return;
    }

    int b = getc(fi);

    while (!feof(fi))
    {
        b ^= river.generate_byte();
        fputc(b, fo);
        b = getc(fi);
    }

    fclose(fi);
    fclose(fo);
}


void encrypt_file(
    const char* file_name,
    const uint8_t* pwd
    )
{
    char res_file_name[200];

    strcpy_s(res_file_name, 200, file_name);
    strcat_s(res_file_name, 200, ".encrypt");

    FILE *fi, *fo;

    fopen_s(&fi, file_name, "rb");
    fopen_s(&fo, res_file_name, "wb");

    if ((fi == nullptr) || (fo == nullptr))
        return;

    River river;
    uint8_t cache[16];

    Md5::calc_hash(pwd, strlen((const char*)pwd), cache);
    for (int i = 0; i < 10000; i++)
        Md5::calc_hash(cache, 16, cache);
    river.set_parameters(cache[4], *(uint16_t *)cache, *(uint16_t *)(cache + 2));

    fputc(cache[0] ^ river.generate_byte(), fo);
    fputc(cache[1] ^ river.generate_byte(), fo);
    fputc(cache[2] ^ river.generate_byte(), fo);
    fputc(cache[3] ^ river.generate_byte(), fo);
    fputc(cache[4] ^ river.generate_byte(), fo);

    int b = getc(fi);

    while (!feof(fi))
    {
        b ^= river.generate_byte();
        fputc(b, fo);
        b = getc(fi);
    }

    fclose(fi);
    fclose(fo);
}


int _tmain(int argc, _TCHAR* argv[])
{
    char file_name[200];
    uint8_t pwd[200];

    while (true)
    {
        int choice = 0;

        printf("1: Encrypt file.\n");
        printf("2: Decrypt file.\n");
        printf("3: Exit.\n");
        scanf_s("%d", &choice);

        if ((choice == 1) || (choice == 2))
        {
            printf("Enter file name.\n");
            scanf_s("%s", file_name, 200);
            printf("Enter password.\n");
            scanf_s("%s", pwd, 200);
        }

        if (choice == 1)
            encrypt_file(file_name, pwd);
        if (choice == 2)
            decrypt_file(file_name, pwd);
        if (choice == 3)
            break;
    }

	return 0;
}

