//
// Created by martin on 11/13/22.
//

#include "miscellaneous_functions.hpp"

void to_char_array(int32_t num, char *out_array) {
    uint_fast8_t i = floor(log10(num));
    out_array[i] = 48;
    while (num > 0) {
        out_array[i--] = num % 10 + 48;
        num /= 10;
    }
}
