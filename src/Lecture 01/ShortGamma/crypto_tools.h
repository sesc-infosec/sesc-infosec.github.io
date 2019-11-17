#pragma once

#include <algorithm>
#include <stdio.h>
#include <stdint.h>
#include <vector>

using namespace std;

const int ABC_SIZE = 0x20;


class CryptoTools
{
public:

    // Накладывает гамму из одного символа на текст.
    static vector<uint8_t> apply_gamma(
        const vector<uint8_t>& text,
        uint8_t gamma
        );


    // Накладывает гамму на текст.
    static vector<uint8_t> apply_gamma(
        const vector<uint8_t>& text,
        const vector<uint8_t>& gamma
        );


    // Вычисляет индекс совпадений для текста, если
    // длина наложенной гаммы равна gamma_length.
    // (делит на строки и вычисляет для них средний
    // индекс совпадений)
    static double calc_match_index(
        const vector<uint8_t>& text,
        uint8_t gamma_length
        );


    // Вычисляет индекс совпадений для строки.
    static double calc_match_index(
        const vector<uint8_t>& line
        );


    // Вычисляет взаимный индекс совпадений для двух строк.
    static double calc_self_match_index(
        const vector<uint8_t>& line1,
        const vector<uint8_t>& line2
        );


    // Считает встречаемость букв в строке.
    // Возвращает вектор длины размера алфавита, элементы
    // которого показывают сколько раз в строке встертилась
    // соответствующая буква.
    static vector<int> calc_symbols_counts(
        const vector<uint8_t>& line
        );


    // Определяет длину короткой гаммы, наложенной на текст.
    static uint8_t find_gamma_length(
        const vector<uint8_t>& text,
        uint8_t max_gamma_length
        );


    // Определяет на сколько сдвинуты относительно
    // друг друга алфавиты переданных строк.
    static uint8_t find_lines_shift(
        const vector<uint8_t>& line1,
        const vector<uint8_t>& line2
        );


    // Снимает гамму с текста.
    static vector<uint8_t> remove_gamma(
        const vector<uint8_t>& text,
        const vector<uint8_t>& gamma
        );


    // Разделяет текст на gamma_length строк.
    // Буквы первой строки зашифрованы на первом символе гаммы.
    // Буквы второй строки зашифрованы на втором символе гаммы и т.д.
    static vector<vector<uint8_t>> split_text_into_gamma_lines(
        const vector<uint8_t>& text,
        uint8_t gamma_length
        );
};


vector<uint8_t> CryptoTools::apply_gamma(
    const vector<uint8_t>& text,
    uint8_t gamma
    )
{
    return apply_gamma(text, vector<uint8_t>(1, gamma));
}


vector<uint8_t> CryptoTools::apply_gamma(
    const vector<uint8_t>& text,
    const vector<uint8_t>& gamma
    )
{
    vector<uint8_t> result(text.size());

    for (unsigned int i = 0; i < text.size(); i++)
        result[i] = (text[i] + gamma[i % gamma.size()]) % ABC_SIZE;

    return result;
}


double CryptoTools::calc_match_index(
    const vector<uint8_t>& text,
    uint8_t gamma_length
    )
{
    vector<vector<uint8_t>> lines = split_text_into_gamma_lines(text, gamma_length);

    double match_index = 0;

    for (auto line : lines)
        match_index += calc_match_index(line);

    return 1.0 * match_index / gamma_length;
}


double CryptoTools::calc_match_index(
    const vector<uint8_t>& line
    )
{
    vector<int> sym_numbers = calc_symbols_counts(line);

    double match_index = 0;

    for (int n : sym_numbers)
        match_index += 1.0 * n / line.size() * (n - 1) / (line.size() - 1);

    return match_index;
}


double CryptoTools::calc_self_match_index(
    const vector<uint8_t>& line1,
    const vector<uint8_t>& line2
    )
{
    vector<int> sym_numbers1 = calc_symbols_counts(line1);
    vector<int> sym_numbers2 = calc_symbols_counts(line2);

    double match_index = 0;

    for (unsigned int i = 0; i < sym_numbers1.size(); i++)
        match_index += 1.0 * sym_numbers1[i] / line1.size() * sym_numbers2[i] / line2.size();

    return match_index;
}


vector<int> CryptoTools::calc_symbols_counts(
    const vector<uint8_t>& line
    )
{
    vector<int> sym_numbers(ABC_SIZE);

    for (uint8_t c : line)
        sym_numbers[c]++;

    return sym_numbers;
}


uint8_t CryptoTools::find_gamma_length(
    const vector<uint8_t>& text,
    uint8_t max_gamma_length
    )
{
    for (uint8_t i = 1; i <= max_gamma_length; i++)
        if (calc_match_index(text, i) > 0.05)
            // критерий очень простой, возможны ошибки
            return i;

    return 0;
}


uint8_t CryptoTools::find_lines_shift(
    const vector<uint8_t>& line1,
    const vector<uint8_t>& line2
    )
{
    vector<double> match_indexes;

    for (uint8_t i = 0; i < ABC_SIZE; i++)
        match_indexes.push_back(calc_self_match_index(line1, apply_gamma(line2, i)));

    return distance(match_indexes.begin(), max_element(match_indexes.begin(), match_indexes.end()));
}


vector<uint8_t> CryptoTools::remove_gamma(
    const vector<uint8_t>& text,
    const vector<uint8_t>& gamma
    )
{
    vector<uint8_t> result(text.size());

    for (unsigned int i = 0; i < text.size(); i++)
        result[i] = (text[i] + ABC_SIZE - gamma[i % gamma.size()]) % ABC_SIZE;

    return result;
}


vector<vector<uint8_t>> CryptoTools::split_text_into_gamma_lines(
    const vector<uint8_t>& text,
    uint8_t gamma_length
    )
{
    vector<vector<uint8_t>> lines(gamma_length);

    for (unsigned int i = 0; i < text.size(); i++)
        lines[i % gamma_length].push_back(text[i]);

    return lines;
}
