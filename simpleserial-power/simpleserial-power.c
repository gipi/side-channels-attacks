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
#ifdef SET_ZERO
    asm volatile(
    "eor r0, r0" "\n\t"
    ::
    );
#endif

#ifdef SET_FF
    asm volatile(
    "ldi r16, 0xff" "\n\t"
    "mov r0, r16" "\n\t"
    ::
    );
#endif

    trigger_high();
    //_delay_ms(10);

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
#ifdef LDI
    asm volatile(
#ifdef LDI_AFTER_JMP
    "rjmp ." TOSTRING(LDI_AFTER_JMP)"\n\t"
#endif
    "ldi r16, " TOSTRING(LDI) "\n\t"
    //"mov r0, r16" "\n\t"
    ::
    );

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

#if defined SUM_A && defined SUM_B
    asm volatile(
    "ldi r17, " TOSTRING(SUM_A) "\n\t"
    "ldi r16, " TOSTRING(SUM_B) "\n\t"
#if defined ADC_AFTER_JMP
    "rjmp ." TOSTRING(ADC_AFTER_JMP) "\n\t"
#endif
    "adc r16, r17" "\n\t"
#ifdef FINAL_MOV
    "mov r7, r16" "\n\t"
#endif
    ::
    );

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
