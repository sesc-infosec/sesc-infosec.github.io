#ifndef PET_CIPHER_DIFF_H
#define PET_CIPHER_DIFF_H



#include "diff_builder.h"
#include "pet_cipher.h"

// Набор методов для дифференциального анализа шифра.
class PetCipherDiff
{
public:

    PetCipherDiff();

    void analize();

private:

    PetCipher _cipher;

    void _step1();

    void _step2();

    void _step3();

    void _step4();

    // Определяет долю успешно расшифрованных блоков для указанного
    // количества раундов шифрования.
    void _check_on_rounds_number(
        int rounds_number
        );

    // В указанной таблице дифференциалов находит цепочку дифференциалов,
    // вероятность которой максимальна.
    // Работает полным перебором.
    void _find_max_probability_chain_brute(
        const Table& diff_table,
        int rounds_number,
        vector<int>& chain,
        double& probability
        );

    // В указанной таблице дифференциалов находит цепочку дифференциалов,
    // вероятность которой максимальна.
    // Работает по упрощенному поиску.
    void _find_max_probability_chain_quick(
        const Table& diff_table,
        int rounds_number,
        vector<int>& chain,
        double& probability
        );

    // Возвращает пары текстов с указанным дифференциалом
    // для указанного s-блока.
    vector<BitVector> _get_proper_block_pairs(
        const BitVector& diff,
        int sbox_index
        );

    // Перебирает tries_number случайных s-блоков и устанавливает те,
    // вероятность выходного дифференциала в которых максимальна.
    void _set_weak_sboxes(
        int unique_sbox_number,
        int tries_number
        );
};


PetCipherDiff::PetCipherDiff()
{
    _cipher.set_key(convert_int_to_bitvector(3001, _cipher.get_block_size()));
}


void PetCipherDiff::analize()
{
    _step1();
    _step2();
    //_step3();
    _step4();

//    for (int i = 2; i < 10; i++)
//        _check_on_rounds_number(i);

}


void PetCipherDiff::_step1(
    )
{
    int sblock_size = _cipher.get_sblock_size();

    vector<int> sbox = _cipher.generate_sbox();
    _cipher.set_sbox(sbox, 0);

    Table diff_table = build_diff_table(_cipher, sblock_size, sblock_size, 0);

    int mx, my, mval;

    find_max_element_coordinates(diff_table, mx, my);
    mval = diff_table[my][mx];

    diff_table.clear();
}


void PetCipherDiff::_step2(
    )
{
    _cipher.set_rounds_number(2);

    int sblock_size = _cipher.get_sblock_size();

    vector<int> sbox = _cipher.generate_sbox();
    _cipher.set_sbox(sbox, 0);

    Table diff_table = build_diff_table(_cipher, sblock_size, sblock_size, 0);

    vector<int> chain;
    double probability;

    _find_max_probability_chain_brute(diff_table, _cipher.get_rounds_number() - 1, chain, probability);

    diff_table.clear();
}


void PetCipherDiff::_step3(
    )
{
    _cipher.set_rounds_number(2);

    int sblock_size = _cipher.get_sblock_size();

    _set_weak_sboxes(1, 10000);

    Table diff_table = build_diff_table(_cipher, sblock_size, sblock_size, 0);

    vector<int> chain;
    double probability;

    _find_max_probability_chain_brute(diff_table, _cipher.get_rounds_number() - 1, chain, probability);

    diff_table.clear();

}


void PetCipherDiff::_step4(
    )
{
    _cipher.set_rounds_number(3);

    int sblock_size = _cipher.get_sblock_size();

    _set_weak_sboxes(1, 100);

    Table diff_table;
    vector<int> chain;
    double probability;

    diff_table = build_diff_table(_cipher, sblock_size, sblock_size, 0);
    _find_max_probability_chain_brute(diff_table, _cipher.get_rounds_number() - 1, chain, probability);

    BitVector diff_bit = convert_int_to_bitvector(chain[0], sblock_size);
    vector<BitVector> proper_block_pairs = _get_proper_block_pairs(diff_bit, 0);

    BitVector key_bit(_cipher.get_block_size());

    int max_ok_count = 0;
    vector<int> max_ok_keys;

    for (int key = 0; key < 1 << sblock_size; key++)
    {
        convert_int_to_bitvector(key, key_bit, 0, sblock_size);

        int ok_pairs_count = 0;

        for (int i_pair = 0; i_pair < proper_block_pairs.size(); i_pair += 2)
        {
            BitVector block1 = proper_block_pairs[i_pair];
            BitVector block2 = proper_block_pairs[i_pair + 1];

            _cipher.encrypt_block(block1);
            _cipher.encrypt_block(block2);
            xor_bitvectors(block1, key_bit, block1);
            xor_bitvectors(block2, key_bit, block2);
            _cipher.apply_inv_sboxes(block1);
            _cipher.apply_inv_sboxes(block2);

            xor_bitvectors(block1, block2, block1);

            int diff_int = convert_bitvector_to_int(block1, 0, sblock_size);
            if (diff_int == chain.back())
                ok_pairs_count++;
        }

        if (max_ok_count == ok_pairs_count)
            max_ok_keys.push_back(key);

        if (max_ok_count < ok_pairs_count)
        {
            max_ok_count = ok_pairs_count;
            max_ok_keys.clear();
            max_ok_keys.push_back(key);
        }
    }

    BitVector last_key = _cipher.get_keys().back();
    int true_key = convert_bitvector_to_int(last_key, 0, _cipher.get_sblock_size());

    diff_table.clear();
}


void PetCipherDiff::_check_on_rounds_number(
    int depth
    )
{
    _cipher.set_rounds_number(depth);

    BitVector last_key = _cipher.get_keys().back();
    int true_value = convert_bitvector_to_int(last_key, 0, _cipher.get_sblock_size());
    int true_count = 0;

    for (int i_glob = 0; i_glob < 200; i_glob++)
    {
        _set_weak_sboxes(1, 100);

        int sbox_number = _cipher.get_sbox_number();
        int sblock_size = _cipher.get_sblock_size();

        vector<Table> diff_tables(sbox_number);
        Table chains(sbox_number);
        vector<double> probabilities(sbox_number);

        for (int i = 0; i < sbox_number; i++)
        {
            diff_tables[i] = build_diff_table(_cipher, sblock_size, sblock_size, i);
            _find_max_probability_chain_quick(diff_tables[i], _cipher.get_rounds_number() - 1, chains[i], probabilities[i]);
        }

        BitVector diff_bit = convert_int_to_bitvector(chains[0][0], sblock_size);
        vector<BitVector> proper_block_pairs = _get_proper_block_pairs(diff_bit, 0);
        BitVector key_bit(_cipher.get_block_size());
        int max_ok_count = 0;
        vector<int> max_ok_keys;

        for (int key = 0; key < 1 << sblock_size; key++)
        {
            convert_int_to_bitvector(key, key_bit, 0, sblock_size);

            int ok_pairs_count = 0;

            for (int i_pair = 0; i_pair < proper_block_pairs.size(); i_pair += 2)
            {
                BitVector block1 = proper_block_pairs[i_pair];
                BitVector block2 = proper_block_pairs[i_pair + 1];

                _cipher.encrypt_block(block1);
                _cipher.encrypt_block(block2);
                xor_bitvectors(block1, key_bit, block1);
                xor_bitvectors(block2, key_bit, block2);
                _cipher.apply_inv_sboxes(block1);
                _cipher.apply_inv_sboxes(block2);

                xor_bitvectors(block1, block2, block1);

                int diff_int = convert_bitvector_to_int(block1, 0, sblock_size);
                if (diff_int == chains[0].back())
                    ok_pairs_count++;
            }

            if (max_ok_count == ok_pairs_count)
                max_ok_keys.push_back(key);

            if (max_ok_count < ok_pairs_count)
            {
                max_ok_count = ok_pairs_count;
                max_ok_keys.clear();
                max_ok_keys.push_back(key);
            }
        }

        for (auto k : max_ok_keys)
            if (k == true_value)
                true_count++;
    }
    printf("depth=%d (%d : %d)\n", depth, true_count, 200);

}


void PetCipherDiff::_find_max_probability_chain_brute(
    const Table& diff_table,
    int rounds_number,
    vector<int>& chain,
    double& probability
    )
{
    int diff_size = diff_table.size();
    vector<int> indexes(rounds_number, 1);

    probability = 0;

    for (int i_diff = 1; i_diff < diff_size; i_diff++)
    {
        do
        {
            double curr_probability = 1.0 * diff_table[i_diff][indexes[0]] / diff_size;

            for (int i = 0; i < rounds_number - 1; i++)
                curr_probability *= 1.0 * diff_table[indexes[i]][indexes[i + 1]] / diff_size;

            if (probability < curr_probability)
            {
                probability = curr_probability;
                chain = indexes;
                chain.insert(chain.begin(), i_diff);
            }
        }
        while (set_next_vector_value(indexes, 1, diff_size - 1));
    }
}


void PetCipherDiff::_find_max_probability_chain_quick(
    const Table& diff_table,
    int rounds_number,
    vector<int>& chain,
    double& probability
    )
{
    int diff_size = diff_table.size();

    chain.resize(2);
    find_max_element_coordinates(diff_table, chain[1], chain[0]);
    probability = 1.0 * diff_table[chain[0]][chain[1]] / diff_size;

    for (int i = 0; i < _cipher.get_rounds_number() - 1; i++)
    {
        int max_value_pos = find_max_element_position(diff_table[chain.back()]);
        probability *= 1.0 * diff_table[chain.back()][max_value_pos] / diff_size;
        chain.push_back(max_value_pos);
    }
}


vector<BitVector> PetCipherDiff::_get_proper_block_pairs(
    const BitVector& diff,
    int sbox_index
    )
{
    vector<BitVector> block_pairs;

    BitVector block1(_cipher.get_block_size());
    BitVector block2(_cipher.get_block_size());
    BitVector block_diff(_cipher.get_block_size());
    BitVector sblock1(_cipher.get_sblock_size());


    for (int i = 0; i < diff.size(); i++)
        block_diff[sbox_index * _cipher.get_sblock_size() + i] = diff[i];

    do
    {
        for (int i = 0; i < sblock1.size(); i++)
            block1[sbox_index * _cipher.get_sblock_size() + i] = sblock1[i];

        xor_bitvectors(block1, block_diff, block2);
        block_pairs.push_back(block1);
        block_pairs.push_back(block2);
    }
    while (set_next_bitvector_value(sblock1));

    return block_pairs;
}


void PetCipherDiff::_set_weak_sboxes(
    int unique_sbox_number,
    int tries_number
    )
{
    vector<int> weakest_sbox;

    for (int i_sbox = 0; i_sbox < unique_sbox_number; i_sbox++)
    {
        double max_prob = 0;
        Table diff_table;
        vector<int> chain, sbox;

        for (int i = 0; i < tries_number; i++)
        {
            double curr_prob;

            sbox = _cipher.generate_sbox();
            _cipher.set_sbox(sbox, i_sbox);
             diff_table = build_diff_table(_cipher, _cipher.get_sblock_size(), _cipher.get_sblock_size(), i_sbox);
            _find_max_probability_chain_quick(diff_table, _cipher.get_rounds_number() - 1, chain, curr_prob);

            if (max_prob < curr_prob)
            {
                max_prob = curr_prob;
                weakest_sbox = sbox;
            }
        }

        _cipher.set_sbox(weakest_sbox, i_sbox);
    }

    for (int i = unique_sbox_number; i < _cipher.get_sbox_number(); i++)
        _cipher.set_sbox(weakest_sbox, i);
}



#endif // PET_CIPHER_DIFF_H
