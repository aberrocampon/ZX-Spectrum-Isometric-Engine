#include "keyboard.h"

byte keyboard_row_54321; // puerto 0xF7FE, row 0x8
byte keyboard_row_trewq; // puerto 0xFBFE, row 0x4

void keyboard_readrow_54321(void)
{
    #asm
        ld bc, 0xF7FE
        in a, (bc)
        ld (_keyboard_row_54321), a
    #endasm
}

void keyboard_readrow_trewq(void)
{
    #asm
        ld bc, 0xFBFE
        in a, (bc)
        ld (_keyboard_row_trewq), a
    #endasm
}