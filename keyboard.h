#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "value_types.h"

extern byte keyboard_row_54321;
extern byte keyboard_row_trewq;

#define keyboard_is_key_pressed_1() (!(keyboard_row_54321 & 0x01))
#define keyboard_is_key_pressed_2() (!(keyboard_row_54321 & 0x02))
#define keyboard_is_key_pressed_3() (!(keyboard_row_54321 & 0x04))
#define keyboard_is_key_pressed_4() (!(keyboard_row_54321 & 0x08))
#define keyboard_is_key_pressed_5() (!(keyboard_row_54321 & 0x10))

#define keyboard_is_key_pressed_q() (!(keyboard_row_trewq & 0x01))
#define keyboard_is_key_pressed_w() (!(keyboard_row_trewq & 0x02))
#define keyboard_is_key_pressed_e() (!(keyboard_row_trewq & 0x04))
#define keyboard_is_key_pressed_r() (!(keyboard_row_trewq & 0x08))
#define keyboard_is_key_pressed_t() (!(keyboard_row_trewq & 0x10))

void keyboard_readrow_54321(void);
void keyboard_readrow_trewq(void);

#endif