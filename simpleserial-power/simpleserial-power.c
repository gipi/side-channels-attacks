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
	/*
	putch('h');
	putch('e');
	putch('l');
	putch('l');
	putch('o');
	putch('\n');
	*/
		
    _delay_ms(5000);

    trigger_high();
    //_delay_ms(10);

#ifdef NOP

    asm volatile(
    "nop"       "\n\t"
    "nop"       "\n\t"
    "nop"       "\n\t"
    "nop"       "\n\t"
    "nop"       "\n\t"
    "nop"       "\n\t"
    "nop"       "\n\t"
    "nop"       "\n\t"
    "nop"       "\n\t"
    "nop"       "\n\t"
    ::
    );
#endif
#ifdef MUL
    asm volatile(
    "mul r0,r1" "\n\t"
    "mul r0,r1" "\n\t"
    "mul r0,r1" "\n\t"
    "mul r0,r1" "\n\t"
    "mul r0,r1" "\n\t"
    "mul r0,r1" "\n\t"
    "mul r0,r1" "\n\t"
    "mul r0,r1" "\n\t"          
    "mul r0,r1" "\n\t"
    "mul r0,r1" "\n\t"
    ::
    );
#endif
#ifdef MUL2
    asm volatile(
    "mul r0,r1" "\n\t"
    "mul r0,r1" "\n\t"
    "mul r0,r1" "\n\t"
    "mul r0,r1" "\n\t"
    "mul r0,r1" "\n\t"
    "mul r0,r1" "\n\t"
    "mul r0,r1" "\n\t"
    "mul r0,r1" "\n\t"          
    "mul r0,r1" "\n\t"
    "mul r0,r1" "\n\t"
    ::
    );
#endif

    //_delay_ms(5000);
    //trigger_low();

    while(1);

}
