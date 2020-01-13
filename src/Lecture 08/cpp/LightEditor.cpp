// LightEditor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdint.h>
#include <string.h>


void load_from_file(
    char* current_text
    )
{
    char file_name[200];

    printf("Enter file name.\n");
    scanf_s("%s", file_name, 200);

    FILE *f;

    fopen_s(&f, file_name, "rb");
    if (f == nullptr)
    {
        printf("Can not open file %s.\n", file_name);
        return;
    }

    char magic[4];

    fread(magic, 1, 4, f);
    if (memcmp(magic, "weak", 4))
    {
        printf("Wrong file format.");
        return;
    }

    uint64_t text_size;

    fread(&text_size, 8, 1, f);
    fread(current_text, 1, text_size, f);
    current_text[text_size] = '\0';

    fclose(f);
}


void save_to_file(
    char* current_text
    )
{
    char file_name[200];

    printf("Enter file name.\n");
    scanf_s("%s", file_name, 200);

    FILE *f;

    fopen_s(&f, file_name, "wb");
    if (f == nullptr)
    {
        printf("Can not open file %s.\n", file_name);
        return;
    }

    uint64_t text_size = strlen(current_text);

    fwrite("weak", 1, 4, f);
    fwrite(&text_size, 8, 1, f);
    fwrite(current_text, 1, text_size, f);

    fclose(f);
}


int _tmain(int argc, _TCHAR* argv[])
{
    char current_text[1000] = { '\0' };

    while (true)
    {
        printf("\nCurrent text: %s\n\n", current_text);
        printf("1: Load from file.\n");
        printf("2: Save to file.\n");
        printf("3: Enter text (1 word).\n");
        printf("4: Exit.\n");

        int choice = 0;

        scanf_s("%d", &choice);

        if (choice == 1)
            load_from_file(current_text);

        if (choice == 2)
            save_to_file(current_text);

        if (choice == 3)
        {
            printf("Enter text:\n");
            scanf_s("%s", current_text, 1000);
        }

        if (choice == 4)
            break;
    }
	return 0;
}

