#ifndef MATH_TOOLS_H
#define MATH_TOOLS_H

#include "types.h"

// Рассматривает вектор как двоичное представление числа.
// Переводит в число указанный участок вектора.
int convert_bitvector_to_int(
    const BitVector& vec,
    int start_position = -1,
    int final_position = -1
    )
{
    if (start_position == -1)
        start_position = 0;
    if (final_position == -1)
        final_position = vec.size();

    int res = 0;

    for (int i = start_position; i < final_position; i++)
        res = res * 2 + vec[i];

    return res;
}


// Записывает двоичное представление числа в указанную часть вектора.
void convert_int_to_bitvector(
    int val,
    BitVector& vec,
    int start_position = -1,
    int final_position = -1
    )
{
    if (start_position == -1)
        start_position = 0;
    if (final_position == -1)
        final_position = vec.size();

    for (int i = final_position - 1; i >= start_position; i--)
    {
        vec[i] = val & 1;
        val >>= 1;
    }
}


// Создает новый двоичный вектор для указанного числового значения.
BitVector convert_int_to_bitvector(
    int val,
    int vector_size
    )
{
    BitVector vec(vector_size);

    for (int i = vector_size - 1; i >= 0; i--)
    {
        vec[i] = val & 1;
        val >>= 1;
    }

    return vec;
}


// Находит координаты самого большого элемента в массиве.
template <class T>
void find_max_element_coordinates(
    const vector<vector<T>>& table,
    int& el_x,
    int& el_y
    )
{
    T max_value = table[0][0];
    el_x = 0;
    el_y = 0;

    for (int y = 0; y < table.size(); y++)
        for (int x = 0; x < table[y].size(); x++)
            if (max_value < table[y][x])
            {
                max_value = table[y][x];
                el_x = x;
                el_y = y;
            }
}


// Находит индекс самого большого элемента в векторе.
template <class T>
int find_max_element_position(
    const vector<T>& vec
    )
{
    T max_value = vec[0];
    int max_pos = 0;

    for (int i = 1; i < vec.size(); i++)
        if (max_value < vec[i])
        {
            max_value = vec[i];
            max_pos = i;
        }

    return max_pos;
}


// Циклически сдвигает значения в двоичном векторе.
BitVector rotate_bitvector(
    const BitVector& vec,
    int step
    )
{
    BitVector res(vec.size());

    for (int i = 0; i < vec.size(); i++)
    {
        res[(i + step) % vec.size()] = vec[i];
    }

    return res;
}


// Возвращает следующее значение вектора.
// Значения элементов находятся в указанном диапазоне.
// Если переполнения не происходит, возвращает true, иначе - false.
template <class T>
bool set_next_vector_value(
    vector<T>& vec,
    T element_min_value,
    T element_max_value
    )
{
    int i = vec.size() - 1;

    while ((i >= 0) && (vec[i] == element_max_value))
    {
        vec[i] = element_min_value;
        i--;
    }

    if (i < 0)
        return false;

    vec[i]++;

    return true;
}


// Двоичный вектор выражает некоторое число.
// Возвращает двоичный вектор, который выражает число на один больше текущего.
// Если переполнения не происходит, возвращает true, иначе - false.
bool set_next_bitvector_value(
    BitVector& vec
    )
{
    return set_next_vector_value(vec, (uint8_t)0, (uint8_t)1);
}


// Поэлементное сложение двоичных векторов.
BitVector xor_bitvectors(
    const BitVector& vec1,
    const BitVector& vec2
    )
{
    BitVector res(vec1.size());

    for (int i = 0; i < vec1.size(); i++)
        res[i] = vec1[i] ^ vec2[i];

    return res;
}


// Поэлементное сложение двоичных векторов.
void xor_bitvectors(
    const BitVector& v1,
    const BitVector& v2,
    BitVector& res
    )
{
    for (int i = 0; i < v1.size(); i++)
        res[i] = v1[i] ^ v2[i];
}


#endif // MATH_TOOLS_H
