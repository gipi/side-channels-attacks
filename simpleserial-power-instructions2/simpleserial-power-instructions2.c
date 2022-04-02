/*
    This file is part of the ChipWhisperer Example Targets
    Copyright (C) 2012-2017 NewAE Technology Inc.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define TTOSTRING(x) TOSTRING(x)

#define NOPS \
    asm volatile(            \
    "nop"       "\n\t"       \
    "nop"       "\n\t"       \
    "nop"       "\n\t"       \
    "nop"       "\n\t"       \
    "nop"       "\n\t"       \
    "nop"       "\n\t"       \
    "nop"       "\n\t"       \
    "nop"       "\n\t"       \
    "nop"       "\n\t"       \
    "nop"       "\n\t"       \
    : /* output operands */            \
    : /* input operands */             \
    : /* clobbered registers */        \
    )



#include "hal.h"
#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>

int main(void)
{
    platform_init();
	init_uart();
	trigger_setup();
	
 	/* Uncomment this to get a HELLO message for debug */
	putch('h');
	putch('e');
	putch('l');
	putch('l');
	putch('o');
    _delay_ms(100);
		
    /*
     * please check the resulting compiled binary in order to avoid compiler
     * magic optimizations.
     */
    asm volatile(
        "call input_ch_0" "\n\t"
        "mov r16, r24" "\n\t"
        "call input_ch_0" "\n\t"
        "mov r17, r24" "\n\t"
        "call input_ch_0" "\n\t"
        "mov r18, r24" "\n\t"
        "call input_ch_0" "\n\t"
        "mov r19, r24" "\n\t"
        : /* output operands */
        : /* input operands */
        : "r16", "r17", "r18", "r19"/* clobbered registers */
    );

    trigger_high();
    //_delay_ms(10);
#ifdef RETRIGGER
    trigger_low(); // if we want to retrigger later we must set it to low
#endif
    NOPS;
    asm volatile(
        "and r16, r17" "\n\t"
    );
    NOPS;
    asm volatile(
        "or r16, r18" "\n\t"
    );
    NOPS;
    asm volatile(
        "eor r16, r19" "\n\t"
    );

    while(1);

}
