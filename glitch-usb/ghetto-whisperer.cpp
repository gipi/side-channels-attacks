
/*
 * Code adapted from usb-descriptor-simple.cpp of the facewhisperer project.
 *
 * This uses a simple Arduino Uno with an USB Host Shield 2.0
 *
 */
#include <avr/wdt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <usbhub.h>
#include "pgmstrings.h"

#define hang() while(1)
#define led_error(v)
#define led_ok(v)

inline void led_off () {
    PORTB &= ~_BV(PB5);
}

inline void led_on () {
    PORTB |=  _BV(PB5);
}

inline void trigger_high() {
    PORTC |= _BV(PC5);
}

inline void trigger_low() {
    PORTC &= ~_BV(PC5);
}

USB     Usb;

void reset_me() {
    wdt_enable(WDTO_15MS);
    while (1) {}
}

void setup_hw() {
    /* setup led (useless) */
    DDRB  = _BV(PB5);
    PORTB = _BV(PB5);

    /* setup trigger */
    DDRC  = _BV(PC5);
}

void wait_for_usb_device() {
    // Wait until the device has been found and addressed
    do {
        led_error(1);
        Usb.Task();
        if (Usb.getUsbTaskState() == USB_STATE_ERROR ||
            Usb.getUsbTaskState() == USB_STATE_DETACHED) {
            reset_me();
        }
        led_error(0);
    } while (Usb.getUsbTaskState() != USB_STATE_RUNNING);
    led_ok(1);
}

uint8_t streaming_ctrl_read(uint8_t addr, uint8_t ep, uint8_t bmReqType, uint8_t bRequest,
                            uint8_t wValLo, uint8_t wValHi, uint16_t wInd, uint16_t wLength) {
    // Send an arbitrary SETUP packet, then keep issuing and logging INs until failure.
    // Note that there's a high-level function Usb.ctrlReq() we could use except that
    // it needs enough RAM to buffer the result. This usage is odd enough to warrant
    // poking at the MAX3421E more directly.

    uint8_t rcode;
    SETUP_PKT setup_pkt;

    // Use a shortish NAK limit, to fail faster
    uint16_t nak_limit = 1 << 5;

    Usb.regWr(rPERADDR, addr);

    setup_pkt.ReqType_u.bmRequestType = bmReqType;
    setup_pkt.bRequest = bRequest;
    setup_pkt.wVal_u.wValueLo = wValLo;
    setup_pkt.wVal_u.wValueHi = wValHi;
    setup_pkt.wIndex = wInd;
    setup_pkt.wLength = wLength;

    Usb.bytesWr(rSUDFIFO, 8, (uint8_t*) &setup_pkt);
    rcode = Usb.dispatchPkt(tokSETUP, ep, nak_limit);
    if (rcode)
        return rcode;

    Serial.println("IN");

    // Expect DATA1 next
    Usb.regWr(rHCTL, bmRCVTOG1);

    uint32_t total_bytes = 0;

    // Keep going until we fail, never finish the ctrl request successfully.
    while (1) {

        rcode = Usb.dispatchPkt(tokIN, ep, nak_limit);
        if (rcode) {
            Serial.print("rcode ");
            Serial.print(rcode);
            Serial.print(" total ");
            Serial.print(total_bytes);
            Serial.println("\n\n");
            return rcode;
        }

        uint8_t byte_count = Usb.regRd(rRCVBC);
        total_bytes += byte_count;

        for (uint8_t i = 0; i < byte_count; i++) {
            uint8_t byte = Usb.regRd(rRCVFIFO);
            Serial.print(byte >> 4, HEX);
            Serial.print(byte & 0xF, HEX);
        }
        // Serial.println();

        // Free this packet's buffer
        Usb.regWr(rHIRQ, bmRCVDAVIRQ);
    }
}

int main() {

    // this is an implementation detail of the setup() function
    // see <https://forum.arduino.cc/t/using-the-serial-port-from-c-code/79302/4>
    init(); 
    setup_hw();
    led_on();

    Serial.begin(115200);

    _delay_ms(200);

    if (Usb.Init() == -1) {
        Serial.println("fatal: OSC did not start.");
        hang();
    }
    Serial.println("GHETTO");
    //led_off();

    /* we are missing old the clock stuff since we have not clock to synch */

    /* wait for the connection of our target */
    wait_for_usb_device();

    // Try to do a descriptor read that we can glitch into something else, and stream each received IN packet back to the host
    trigger_high();
    streaming_ctrl_read(1, 0, bmREQ_GET_DESCR, USB_REQUEST_GET_DESCRIPTOR, 0, USB_DESCRIPTOR_CONFIGURATION, 0x0000, 0xffff);
    trigger_low();

    // Start over with a software reset
    reset_me();

    return 0;

}
