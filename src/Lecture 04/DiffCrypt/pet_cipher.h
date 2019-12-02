#ifndef PET_H
#define PET_H

#include <cstdlib>
#include <ctime>
#include <vector>

#include "io_tools.h"
#include "math_tools.h"
#include "types.h"

using namespace std;

// Исследуемый шифр.
// Представляет собой последовательность подстановок
// и наложений раундовых ключей. Подблоки шифруются независимо.
class PetCipher
{
public:

    PetCipher();

    // Делает исследуемое в дифференциальном анализе преобразование.
    // Необходима для build_diff_talbe() из diff_builder.h.
    void apply_diff_crypt(
        const BitVector& input_data,
        BitVector& output_data,
        int sbox_index
        ) const;

    // Обратное преобразование на s-блоках.
    void apply_inv_sboxes(
        BitVector& block
        ) const;

    // Расшифровывает блок на текущем ключе.
    void decrypt_block(
        BitVector& block
        ) const;

    // Шифрует блок на текущем ключе.
    void encrypt_block(
        BitVector& block
        ) const;

    // Генерирует таблицу подстановок s-блока.
    vector<int> generate_sbox() const;

    // Генерирует новые таблицы подстановок для s-боксов и сохраняет в файл.
    void generate_sboxes();

    // Возвращает размер шифруемого блока.
    int get_block_size() const;

    // Возвращает расширенный ключ.
    vector<BitVector> get_keys() const;

    // Возвращает количество раундов шифрования.
    int get_rounds_number() const;

    // Возвращает длину подблока, преобразуемого одним s-блоком.
    int get_sblock_size() const;

    // Возвращает s-блок c указанным индексом.
    vector<int> get_sbox(
        int sbox_index
        ) const;

    // Возвращает количество s-блоков.
    int get_sbox_number() const;

    // Возвращает количество элементов подстановки в s-блоке.
    int get_sbox_size() const;

    // Устанавливает новый ключ шифрования.
    void set_key(
        const BitVector& key
        );

    // Устанавливает количество раундов шифрования.
    void set_rounds_number(
        int rounds_number
        );

    // Заменяет указанный s-блок на переданный.
    void set_sbox(
        const vector<int>& sbox,
        int sbox_index
        );

private:

    int _BLOCK_SIZE = 16; // размер блока шифрования
    int _ROUNDS_NUMBER = 2; // количество раундов шифрования
    int _SBLOCK_SIZE = 4; // количество бит, шифруемых одним s-блоком
    int _SBOX_NUMBER = _BLOCK_SIZE / _SBLOCK_SIZE; // количество s-блоков
    int _SBOX_SIZE = 1 << _SBLOCK_SIZE; // размер таблицы подстановки s-блока

    Table _sboxes; // таблицы подстановок
    vector<BitVector> _keys; // расширенный ключ шифрования

    // Обратное преобразование части блока на указанном s-блоке.
    void _apply_inv_sbox(
        BitVector& block,
        int sbox_index
        ) const;

    // Прямое преобразование части блока на указанном s-блоке.
    void _apply_sbox(
        BitVector& block,
        int sbox_index
        ) const;

    // Применение s-блоков к блоку шифрования.
    void _apply_sboxes(
        BitVector& block
        ) const;
};


PetCipher::PetCipher()
{
    _sboxes = load_table("/home/Dim4a/Work/Solutions/Sesc/DiffCrypt/pet_blocks.txt", _SBOX_NUMBER, _SBOX_SIZE);
}


void PetCipher::apply_diff_crypt(
    const BitVector& input_data,
    BitVector& output_data,
    int sbox_index
    ) const
{
    int index = convert_bitvector_to_int(input_data);
    int value = _sboxes[sbox_index][index];

    convert_int_to_bitvector(value, output_data);
}


void PetCipher::apply_inv_sboxes(
    BitVector& block
    ) const
{
    for (int i = 0; i < _SBOX_NUMBER; i++)
        _apply_inv_sbox(block, i);
}


void PetCipher::decrypt_block(
    BitVector& block
    ) const
{
    for (int i = _ROUNDS_NUMBER; i > 0; i--)
    {
        xor_bitvectors(block, _keys[i], block);
        apply_inv_sboxes(block);
    }
    xor_bitvectors(block, _keys[0]);
}


void PetCipher::encrypt_block(
    BitVector& block
    ) const
{
    xor_bitvectors(block, _keys[0]);

    for (int i = 1; i <= _ROUNDS_NUMBER; i++)
    {
        _apply_sboxes(block);
        xor_bitvectors(block, _keys[i], block);
    }
}


vector<int> PetCipher::generate_sbox(
    ) const
{
    srand(time(nullptr));

    vector<int> sbox(_SBOX_SIZE);

    for (int i = 0; i < _SBOX_SIZE; i++)
        sbox[i] = i;

    for (int i = 0; i < _SBOX_SIZE; i++)
    {
        int i1 = rand() % _SBOX_SIZE;
        int i2 = rand() % _SBOX_SIZE;

        swap(sbox[i1], sbox[i2]);
    }

    return sbox;
}


void PetCipher::generate_sboxes(
    )
{
    srand(time(nullptr));
    _sboxes.resize(_SBOX_NUMBER);

    for (int i_box = 0; i_box < _sboxes.size(); i_box++)
        _sboxes[i_box] = generate_sbox();

    save_table(_sboxes, "/home/user/Work/Solutions/Sssc/DiffCrypt/pet_blocks.txt");
}


int PetCipher::get_block_size(
    ) const
{
    return _BLOCK_SIZE;
}


vector<BitVector> PetCipher::get_keys() const
{
    return _keys;
}


int PetCipher::get_rounds_number(
    ) const
{
    return _ROUNDS_NUMBER;
}


int PetCipher::get_sblock_size(
    ) const
{
    return _SBLOCK_SIZE;
}


vector<int> PetCipher::get_sbox(
    int sbox_index
    ) const
{
    return _sboxes[sbox_index];
}


int PetCipher::get_sbox_number(
    ) const
{
    return _SBOX_NUMBER;
}


int PetCipher::get_sbox_size(
    ) const
{
    return _SBOX_SIZE;
}


void PetCipher::set_key(
    const BitVector& key
    )
{
    _keys.resize(_ROUNDS_NUMBER + 1);
    _keys[0] = key;

    for (int i = 1; i < _keys.size(); i++)
        _keys[i] = rotate_bitvector(_keys[i - 1], 1);
}


void PetCipher::set_rounds_number(
    int rounds_number
    )
{
    BitVector key = _keys[0];

    _ROUNDS_NUMBER = rounds_number;
    set_key(key);
}


void PetCipher::set_sbox(
    const vector<int>& sbox,
    int sbox_index
    )
{
    _sboxes[sbox_index] = sbox;
}


void PetCipher::_apply_inv_sbox(
    BitVector& block,
    int sbox_index
    ) const
{
    int value = convert_bitvector_to_int(block, sbox_index * _SBLOCK_SIZE, (sbox_index + 1) * _SBLOCK_SIZE);

    for (int index = 0; index < _SBOX_SIZE; index++)
        if (_sboxes[sbox_index][index] == value)
        {
            convert_int_to_bitvector(index, block, sbox_index * _SBLOCK_SIZE, (sbox_index + 1) * _SBLOCK_SIZE);
            break;
        }
}


void PetCipher::_apply_sbox(
    BitVector& block,
    int sbox_index
    ) const
{
    int index = convert_bitvector_to_int(block, sbox_index * _SBLOCK_SIZE, (sbox_index + 1) * _SBLOCK_SIZE);
    int value = _sboxes[sbox_index][index];

    convert_int_to_bitvector(value, block, sbox_index * _SBLOCK_SIZE, (sbox_index + 1) * _SBLOCK_SIZE);
}


void PetCipher::_apply_sboxes(
    BitVector& block
    ) const
{
    for (int i = 0; i < _SBOX_NUMBER; i++)
        _apply_sbox(block, i);
}


#endif // PET_H
