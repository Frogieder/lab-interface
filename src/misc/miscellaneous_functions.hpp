//
// Created by martin on 11/13/22.
//

#ifndef LAB_INTERFACE_MISCELLANEOUS_FUNCTIONS_H
#define LAB_INTERFACE_MISCELLANEOUS_FUNCTIONS_H

#include "pico/stdlib.h"
#include <cmath>

/**
 * Converts a number into a character array.
 *
 * ASSUMES THAT out_array IS BIG ENOUGH!
 *
 * @tparam T type of num
 * @param num number to convert
 * @param out_array char array to output into
 */
void to_char_array(int32_t num, char *out_array);

//
//template <class T>
//void to_char_array(T num, char* out_array);

#endif //LAB_INTERFACE_MISCELLANEOUS_FUNCTIONS_H
