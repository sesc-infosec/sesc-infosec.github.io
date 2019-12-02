#include <iostream>
#include <vector>

using namespace std;

const int BASE = 0x100;

vector<int> find_coefficients(
    const vector<int>& arguments,
    int different_arguments_number,
    int total_sum,
    int total_arguments_number
    )
{
    if (different_arguments_number == 1)
    {
        if ((arguments[0] * total_arguments_number) % BASE == total_sum)
            return vector<int>(1, total_arguments_number);
        else
            return vector<int>();
    }

    for (int i = 0; i <= total_arguments_number; i++)
    {
        int current_sum = (arguments[different_arguments_number - 1] * i) % BASE;
        int residual_sum = (BASE + total_sum - current_sum) % BASE;
        vector<int> current_coefficients = find_coefficients(
                    arguments, different_arguments_number - 1, residual_sum, total_arguments_number - i);

        if (current_coefficients.empty())
            continue;

        current_coefficients.push_back(i);
        return current_coefficients;
    }

    return vector<int>();
}


int main()
{
    vector<int> arguments = {0x20, 0x09, 0x0d, 0x0a};
    vector<int> sums = {0x5e, 0x60, 0xd5, 0xef, 0xdb, 0xe9, 0xf9, 0xd3};
    vector<vector<int>> coefficients(sums.size());
    int arguments_number = 0;
    bool isFound = false;

    while (!isFound)
    {
        isFound = true;

        for (int i = 0; i < sums.size(); i++)
        {
            coefficients[i] = find_coefficients(arguments, arguments.size(), sums[i], arguments_number);

            if (coefficients[i].empty())
            {
                isFound = false;
                break;
            }
        }
        printf("%d-%s\n", arguments_number, isFound ? "yes\n" : "no");
        arguments_number++;
    }

    for (int i = 0; i < coefficients.size(); i++)
    {
        printf("%2x: ", sums[i]);
        for (int j = 0; j < coefficients[i].size(); j++)
        {
            printf("%2x-%2d; ", arguments[j], coefficients[i][j]);
        }
        printf("\n");
    }

    return 0;
}
