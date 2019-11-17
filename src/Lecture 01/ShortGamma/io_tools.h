#pragma once

#include <stdio.h>
#include <stdint.h>
#include <vector>

using namespace std;


class IoTools
{
public:

    // Переводит символы из значений 0..31 в коды cp-1251.
    static vector<uint8_t> convert_int_to_str(
        const vector<uint8_t>& int_data
        );


    // Переводит из кодировки cp-1251 в значения 0..31.
    static vector<uint8_t> convert_str_to_int(
        const vector<uint8_t>& str_data
        );


    // Считывает текст из файла, оставляет только буквы,
    // переводит заглавные в строчные и из кодировки cp-1251
    // переводит в значения 0..31.
    static vector<uint8_t> read_file(
        const char* file_name
        );


    // Переводит текст из значений 0..31 в кодировку cp-1251
    // и сохраняет в файл.
    static void save_file(
        const char* file_name,
        const vector<uint8_t>& data
        );
};


vector<uint8_t> IoTools::convert_int_to_str(
    const vector<uint8_t>& int_data
    )
{
    vector<uint8_t> str_data;

    for (uint8_t c : int_data)
        str_data.push_back(c + 0xe0);

    return str_data;
}


vector<uint8_t> IoTools::convert_str_to_int(
    const vector<uint8_t>& str_data
    )
{
    vector<uint8_t> int_data;

    for (uint8_t c : str_data)
        int_data.push_back(c - 0xe0);

    return int_data;
}


vector<uint8_t> IoTools::read_file(
    const char* file_name
    )
{
    FILE *f = fopen(file_name, "r");

    if (f == NULL)
        return vector<uint8_t>();

    vector<uint8_t> row_data;
    int c = fgetc(f);

    while (!feof(f))
    {
        row_data.push_back(c);
        c = fgetc(f);
    }

    fclose(f);

    vector<uint8_t> sym_data;

    for (uint8_t c : row_data)
    {
        if ((c >= 0xe0) && (c <= 0xff))
            sym_data.push_back(c);
        if ((c >= 0xc0) && (c <= 0xdf))
            sym_data.push_back(c + 0x20);
    }

    return convert_str_to_int(sym_data);
}


void IoTools::save_file(
    const char* file_name,
    const vector<uint8_t>& data
    )
{
    FILE *f = fopen(file_name, "w");

    if (f == NULL)
        return;

    vector<uint8_t> str_data = convert_int_to_str(data);

    fwrite(str_data.data(), 1, str_data.size(), f);

    fclose(f);
}
