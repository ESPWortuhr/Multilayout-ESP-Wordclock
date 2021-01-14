#pragma once

#include <stdint.h>

// Icons to display on the front

// WiFi
//      | - - - - : - - - - |
//   0: . . . O O O O O . . . : 10
//  21: . O O O O O O O O O . : 11
//  22: O O O . . . . . O O O : 32
//  43: O . . O O O O O . . O : 33
//  44: . . O O O O O O O . . : 54
//  65: . . O . . . . . O . . : 55
//  66: . . . . . O . . . . . : 76
//  87: . . . . O O O . . . . : 77
//  88: . . . . . O . . . . . : 98
// 109: . . . . . . . . . . . : 99
//      | - - - - : - - - - |

void show_icon_wlan(uint8_t r0, uint8_t r1, uint8_t r2);
void show_icon_wlan(int strength);
