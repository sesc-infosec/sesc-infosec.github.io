#ifndef IO_TOOLS_H
#define IO_TOOLS_H

#include <iostream>
#include <string.h>
#include <vector>

#include "types.h"

using namespace std;


// Загружает таблицу из файла.
Table load_table(
    const char* file_name,
    int y_size,
    int x_size
    )
{
    Table table(y_size);

    FILE *fi = fopen(file_name, "r");

    if (fi == nullptr)
        return vector<vector<int>>();

    int v;

    for (int y = 0; y < y_size; y++)
        for (int x = 0; x < x_size; x++)
        {
            fscanf(fi, "%d", &v);
            table[y].push_back(v);
        }

    fclose(fi);

    return table;
}


// Сохраняет таблицу в файл.
void save_table(
    const Table& table,
    const char* file_name,
    int digits_per_number = 5 // выравнивание
    )
{
    FILE *f = fopen(file_name, "w");

    if (f == nullptr)
        return;

    for (auto line : table)
    {
        for (auto val : line)
        {
            char str[100];

            sprintf(str, "%d", val);

            if (strlen(str) < digits_per_number)
            {
                int len = strlen(str);
                for (int i = 0; i < len; i++)
                    str[digits_per_number - i - 1] = str[len - i - 1];
                for (int i = 0; i < digits_per_number - len; i++)
                    str[i] = ' ';
                str[digits_per_number] = '\0';
            }

            fprintf(f, "%s ", str);
        }
        fprintf(f, "\n");
    }

    fclose(f);
}


// Сохраняет таблицу в файл.
void save_table(
    const Table& table,
    int digits_per_number = 5 // выравнивание
    )
{
    save_table(table, "/home/user/Work/Solutions/Sssc/DiffCrypt/last_table.txt", digits_per_number);
}


#endif // IO_TOOLS_H
