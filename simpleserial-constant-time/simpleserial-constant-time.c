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
#include <string.h>

#include "simpleserial.h"

uint8_t get_key(uint8_t* k)
{
	// Load key here
	return 0x00;
}

uint8_t get_pt(uint8_t* pt)
{
	/**********************************
	* Start user-specific code here. */
	trigger_high();
	
	//16 hex bytes held in 'pt' were sent
	//from the computer. Store your response
	//back into 'pt', which will send 16 bytes
	//back to computer. Can ignore of course if
	//not needed
	
	trigger_low();
	/* End user-specific code here. *
	********************************/
	simpleserial_put('r', 16, pt);
	return 0x00;
}

uint8_t reset(uint8_t* x)
{
	// Reset key here if needed
	return 0x00;
}

#define PASSWORD "armedilzo"
#define SIZE_INPUT 32

int main(void)
{

    volatile char password[] = PASSWORD;

    platform_init();
    init_uart();
    trigger_setup();

    /* banner requesting the password */
    putch('p');
    putch('a');
    putch('s');
    putch('s');
    putch('w');
    putch('o');
    putch('r');
    putch('d');
    putch(':');
    putch(' ');

    //_delay_ms(5000);/* this is needed to make the ADC capture correctly */

    /* taking the password, waiting for a newline or for SIZE_INPUT characters */
    char c;
    volatile char input[SIZE_INPUT];
    unsigned int input_index = 0;

    do {
        c = getch();
        input[input_index++] = c;
    } while(c != '\n' && input_index < SIZE_INPUT);
    trigger_high();

    volatile register unsigned char result asm("r16") = 0;

#ifdef UNROLL /* FIXME: the length of the password is hardcoded */
    result |= input[0] ^ password[0];
    result |= input[1] ^ password[1];
    result |= input[2] ^ password[2];
    result |= input[3] ^ password[3];
    result |= input[4] ^ password[4];
    result |= input[5] ^ password[5];
    result |= input[6] ^ password[6];
    result |= input[7] ^ password[7];
    result |= input[8] ^ password[8];
#elif defined(UNROLL_ASSEMBLY)

#if defined(UNROLL_W_NOP)
#define UNROLL_NOP "nop\n\t"
#else
#define UNROLL_NOP 
#endif

    asm volatile(
        "ldi    r16, 0\n\t" /* for some reason the declaration of "result" is optimized */
        "ldd	r13, Y+33\n\t"
        "ldd	r15, Y+34\n\t"
        "ldd	r17, Y+35\n\t"
        "ldd	r26, Y+36\n\t"
        "ldd	r30, Y+37\n\t"
        "ldd	r22, Y+38\n\t"
        "ldd	r20, Y+39\n\t"
        "ldd	r18, Y+40\n\t"
        "ldd	r24, Y+41\n\t"

        UNROLL_NOP
        UNROLL_NOP
        "ldd	r14, Y+1\n\t"
        UNROLL_NOP
        "eor	r13, r14\n\t"
        UNROLL_NOP
        "or	r16, r13\n\t"
        UNROLL_NOP

        "ldd	r14, Y+2\n\t"
        UNROLL_NOP
        "eor	r15, r14\n\t"
        UNROLL_NOP
        "or	r16, r15\n\t"
        UNROLL_NOP

        "ldd	r14, Y+3\n\t"
        UNROLL_NOP
        "eor	r17, r14\n\t"
        UNROLL_NOP
        "or	r16, r17\n\t"
        UNROLL_NOP
        
        "ldd	r14, Y+4\n\t"
        UNROLL_NOP
        "eor	r26, r14\n\t"
        UNROLL_NOP
        "or	r16, r26\n\t"
        UNROLL_NOP
        
        "ldd	r14, Y+5\n\t"
        UNROLL_NOP
        "eor	r30, r14\n\t"
        UNROLL_NOP
        "or	r16, r30\n\t"
        UNROLL_NOP
        
        "ldd	r14, Y+6\n\t"
        UNROLL_NOP
        "eor	r22, r14\n\t"
        UNROLL_NOP
        "or	r16, r22\n\t"
        UNROLL_NOP
        
        "ldd	r14, Y+7\n\t"
        UNROLL_NOP
        "eor	r20, r14\n\t"
        UNROLL_NOP
        "or	r16, r20\n\t"
        UNROLL_NOP
        
        "ldd	r14, Y+8\n\t"
        UNROLL_NOP
        "eor	r18, r14\n\t"
        UNROLL_NOP
        "or	r16, r18\n\t"
        UNROLL_NOP
        "ldd	r14, Y+9\n\t"
        UNROLL_NOP
        "eor	r24, r14\n\t"
        UNROLL_NOP
        "or	r16, r24\n\t"
        UNROLL_NOP
    :
    :
    : "r13","r14", "r15", "r16", "r17", "r18", "r20", "r22", "r24", "r26", "r30"/* clobbered registers */
    );
#else
    for (uint8_t index = 0 ; index < strlen(PASSWORD) ; index++) {
        result |= input[index] ^ password[index];
    }
#endif

    if (result)
        while(1);

    putch('W');
    putch('I');
    putch('N');

    while(1);
}
