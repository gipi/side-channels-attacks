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
#include <string.h>
#include <stdlib.h>
#include <hmac-sha256.h>

#include "simpleserial.h"

uint8_t get_key(uint8_t* k)
{
	// Load key here
	return 0x00;
}

uint8_t reset(uint8_t* x)
{
	// Reset key here if needed
	return 0x00;
}

uint8_t signature(uint8_t* mac, uint8_t* msg, uint32_t msglength) {
    uint8_t key[] = { 
        0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
        0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
        0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 
        0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
        0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 
        0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
        0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 
        0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
    hmac_sha256(mac, key, 512, msg, msglength);

    return 0x00;
}

uint8_t echo_signature(uint8_t* input) {
    uint8_t mac[HMAC_SHA256_BYTES];
    signature(mac, input, 64 * 8); /* NOTE: message length is in BITS!!! */

    simpleserial_put('s', HMAC_SHA256_BYTES, mac);

    return 0;
}

uint8_t memcmp_costant(uint8_t* a, uint8_t* b, size_t size) {
    size_t counter;
    uint8_t result = 0;

    for (counter = 0 ; counter < size ; counter ++) {
        result |= a ^ b;
    }

    return result;
}

uint8_t do_upgrade(uint8_t* firmware) {
    uint8_t SIGNATURE[] = {
        111,  77,  82,  21, 132,  15,  4, 54,
        189, 218, 152, 253, 211, 129, 62, 50,
         35,  14, 196, 198, 121,  27, 72, 207,
        158,  14,   7,  42, 177,  16, 65, 143
    };
    uint8_t mac[HMAC_SHA256_BYTES];
    trigger_high();
    signature(mac, firmware, 64 * 8); /* NOTE: message length is in BITS!!! */


    int check = memcmp(SIGNATURE, mac, HMAC_SHA256_BYTES);

    trigger_low();
    if (check) {
        while(1);
    }

    return 0;
}

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
		
	simpleserial_init();
	simpleserial_addcmd('s', 64, echo_signature);
	simpleserial_addcmd('u', 64, do_upgrade);
	while(1)
		simpleserial_get();
}
