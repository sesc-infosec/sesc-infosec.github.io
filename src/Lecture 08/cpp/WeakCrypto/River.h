#pragma once

#include <stdint.h>

class River
{
public:

    uint8_t generate_byte();

    void set_parameters(
        uint32_t initional_value,
        uint32_t additional_value,
        uint32_t module
        );

private:

    uint32_t _additional_value;
    uint32_t _initional_value;
    uint32_t _last_value;
    uint32_t _module;
};


uint8_t River::generate_byte(
    )
{
    _last_value = (_last_value + _additional_value) % _module;

    uint8_t res = (_last_value + (_last_value >> 8)) & 0xff;;

    return res;
}


void River::set_parameters(
    uint32_t initional_value,
    uint32_t additional_value,
    uint32_t module
    )
{
    _additional_value = additional_value;
    _initional_value = initional_value;
    _last_value = initional_value;
    _module = (module == 0) ? 101 : module;
}
