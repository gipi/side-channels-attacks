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

/*
 * Here I want to define a jump table for the various
 *
 *   ldi rx, $value
 *
 * via macro but I need some recursion. I'm going to improvise
 * using this
 * <https://bytes.com/topic/c/answers/213073-loop-equivalent-preprocessor#post821465>
 */
// each entry is sixteen instructions long (more human friendly to look at the
// offsets and moreover is simpler to jump by shifting right the input value
// by 4 + 1, since the instructions are two bytes long, but actually the
// indexing is done using word so we can drop the last addend). At the end there is an
// infinite loop with a nop just before to avoid pipeline noise.
#define _LDI(r,v)               \
    "nop\n\t"                   \
    "nop\n\t"                   \
    "nop\n\t"                   \
    "nop\n\t"                   \
    "nop\n\t"                   \
    "nop\n\t"                   \
    "nop\n\t"                   \
    "nop\n\t"                   \
    "nop\n\t"                   \
    "nop\n\t"                   \
    "nop\n\t"                   \
    "nop\n\t"                   \
    "ldi "#r", "#v"\n\t"        \
    "nop\n\t"                   \
    "jmp _end_jt""\n\t"
// jmp is 4 bytes long

// here we define the jump table via recursion
#define JT0(v)  _LDI(r16,v)
#define JT1(v) JT0(v) JT0(v+1)
#define JT2(v) JT1(v) JT1(v+2)
#define JT3(v) JT2(v) JT2(v+4)
#define JT4(v) JT3(v) JT3(v+8)
#define JT5(v) JT4(v) JT4(v+16)
#define JT6(v) JT5(v) JT5(v+32)
#define JT7(v) JT6(v) JT6(v+64)
#define JT8(v) JT7(v) JT7(v+128)
#define JUMP_TABLE JT8(0)

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

#ifdef USE_TABLE
    /*
     * please check the resulting compiled binary in order to avoid compiler
     * magic optimizations.
     */
    unsigned char c = getch();
    volatile uint16_t index = c << 4; /* */
#endif

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
#ifdef USE_TABLE
    /*
     * Get a char from the serial, intepret it as a byte and build into the
     * register Z (r30:r31) the final address for the jump table.
     *
     * NOTE: the ATMega use word data type to reference instruction addresses,
     * i.e. 15bit of Z, the pm() macro is used for this purpose.
     *
     * %A0 referse to the lsb of the `index` variable
     * %B0 referse to the msb of the `index` variable
     *
     * See <https://web.stanford.edu/class/ee281/projects/aut2002/yingzong-mouse/media/GCCAVRInlAsmCB.pdf>.
     */
    asm volatile(
    "ldi r30,lo8(pm(_jt))""\n\t"
    "ldi r31,hi8(pm(_jt))""\n\t"
    "add r30, %A0""\n\t"
    "adc r31, %B0""\n\t"
    "ijmp""\n\t"
    "_jt:""\n\t"
    JUMP_TABLE
    "_end_jt:""\n\t"
    "mov r24, r16""\n\t"
    "call output_ch_0""\n\t"
    : /* output operands */
    : "r"(index) /* input operands */
    : "r30","r31", "r16"/* clobbered registers */
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
