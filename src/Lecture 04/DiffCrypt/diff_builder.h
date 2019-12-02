#ifndef DIFF_ATTACKER_H
#define DIFF_ATTACKER_H

#include <cstring>
#include <vector>

#include "io_tools.h"
#include "math_tools.h"
#include "types.h"

using namespace std;


// Заполняет матрицу соответствий входных и выходных дифференциалов.
template<class T>
Table build_diff_table(
    const T& cipher, // в классе T должна быть реализована функция apply_diff_crypt
    int input_block_size,
    int output_block_size,
    int crypt_index // индекс анализируемого s-блока
    )
{
    Table diff_table;
    int input_size = input_block_size;
    int output_size = output_block_size;

    diff_table.resize(1 << input_size);
    for (int i = 0; i < diff_table.size(); i++)
        diff_table[i].resize(1 << output_size);

    BitVector input_diff(input_size), input_text1(input_size), input_text2(input_size);
    BitVector output_diff(output_size), output_text1(output_size), output_text2(output_size);

    input_diff.back() = 1;

    for (int id_diff = 1; id_diff < 1 << input_size; id_diff++)
    {
        memset(input_text1.data(), 0, input_size);

        for (int id_text = 0; id_text < 1 << input_size; id_text++)
        {
            xor_bitvectors(input_text1, input_diff, input_text2);
            cipher.apply_diff_crypt(input_text1, output_text1, crypt_index);
            cipher.apply_diff_crypt(input_text2, output_text2, crypt_index);
            xor_bitvectors(output_text1, output_text2, output_diff);
            diff_table[id_diff][convert_bitvector_to_int(output_diff)]++;

            set_next_bitvector_value(input_text1);
        }

        set_next_bitvector_value(input_diff);
    }

    return diff_table;
}

#endif // DIFF_ATTACKER_H
