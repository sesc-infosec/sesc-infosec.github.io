#include "crypto_tools.h"
#include "io_tools.h"

using namespace std;

vector<uint8_t> find_gamma(
    const vector<uint8_t>& data
    )
{
    uint8_t gamma_length;
    vector<vector<uint8_t>> lines;
    vector<uint8_t> shifts(1, 0);

    gamma_length = CryptoTools::find_gamma_length(data, 20);
    lines = CryptoTools::split_text_into_gamma_lines(data, gamma_length);

    for (uint8_t i = 1; i < gamma_length; i++)
        shifts.push_back(CryptoTools::find_lines_shift(lines[0], lines[i]));

    return shifts;
}


int main()
{
    vector<uint8_t> gamma = {0x0c, 0x00, 0x18, 0x08, 0x0d, 0x00}; // "машина"
    vector<uint8_t> data = IoTools::read_file("/home/user/Work/Solutions/Sssc/ShortGamma/data.txt");

    data = CryptoTools::apply_gamma(data, gamma);
    vector<uint8_t> shifts = find_gamma(data);

    return 0;
}
