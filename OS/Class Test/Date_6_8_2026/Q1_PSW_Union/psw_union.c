#include <stdio.h>
#include <stdint.h>

/*
 * PSW union: access as whole byte or as individual flags.
 * Bit mapping (8051 standard):
 * 7 CY, 6 AC, 5 F0, 4 RS1, 3 RS0, 2 OV, 1 (unused), 0 P
 */

typedef union {
    uint8_t byte;
    struct {
        unsigned P  :1; /* bit 0 */
        unsigned    :1; /* bit 1 reserved */
        unsigned OV :1; /* bit 2 */
        unsigned RS0:1; /* bit 3 */
        unsigned RS1:1; /* bit 4 */
        unsigned F0 :1; /* bit 5 */
        unsigned AC :1; /* bit 6 */
        unsigned CY :1; /* bit 7 */
    } bits;
} PSW_t;

int main(void)
{
    PSW_t psw;

    psw.byte = 0x00;
    printf("Initial PSW : 0x%02X\n\n", psw.byte);

    printf("Set Carry\n");
    psw.bits.CY = 1;
    printf("PSW : 0x%02X\n\n", psw.byte);

    printf("Set Overflow\n");
    psw.bits.OV = 1;
    printf("PSW : 0x%02X\n\n", psw.byte);

    printf("Select Register Bank 2\n");
    /* Bank 2 corresponds to RS1=1 RS0=0 */
    psw.bits.RS1 = 1;
    psw.bits.RS0 = 0;
    printf("PSW : 0x%02X\n\n", psw.byte);

    printf("Clear Carry\n");
    psw.bits.CY = 0;
    printf("PSW : 0x%02X\n", psw.byte);

    return 0;
}
