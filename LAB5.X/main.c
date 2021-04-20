///////////////////////////////////// LIBRARIES/HEADERs ///////////////////////////////
#include "device_config.h"
#include <stdint.h>
#include <math.h>

//////////////////////////////7 DIRECTIVES //////////////////////////
#define _XTAL_FREQ 8000000
#define SWEEP_FREQ 20

///////////////////////////DATA TYPES /////////////////////////////////
enum por_ACDC {digital, analog};          // digital = 0, analog = 1



//////////////////////////////// FUNCTION DECLARATIONS ////////////////////////////////////
void portsInit(void);
uint8_t char_to_seg(uint8_t);
void send_to_disp(uint32_t);
char key_scanner(void);
////////////////////////////////////////// MAIN /////////////////////////////////////
void main(void){
    int num_to_disp = 0;
    portsInit();
    while(1){
        char key = key_scanner();
        send_to_disp(key);
        
    }
}

///////////////////////////////////// FUNCTIONS ///////////////////////////////////////
void portsInit(void){
    ANSELA = digital;   // Set port A as Digital for keypad driving
    TRISA  = 0xF0;      // For Port A, set pins 4 to 7 as inputs (columns), and pins 0 to 3 as outputs (rows)
    ANSELB = digital;   // Set port B as Digital for 7 segment cathode selection (only 4 pins used)  
    TRISB  = 0x00;      // For Port B, set pins as outputs for cathode selection
    ANSELD = digital;   // Set port D as Digital for 7 segment anodes
    TRISD  = 0x00;      // for Port D, set all pins as outputs for 7 segment anodes
    OSCCON = 0x74;      // Set the internal oscillator to 8MHz and stable
}


char key_scanner(void){
    LATAbits.LA0 = 0;
    LATAbits.LA1 = 1;
    LATAbits.LA2 = 1;
    LATAbits.LA3 = 1;
    __delay_ms(SWEEP_FREQ);
    if      (PORTAbits.RA4 == 0) {__delay_ms(SWEEP_FREQ); return 1;}
    else if (PORTAbits.RA5 == 0) {__delay_ms(SWEEP_FREQ); return 2;}
    else if (PORTAbits.RA6 == 0) {__delay_ms(SWEEP_FREQ); return 3;}
    else if (PORTAbits.RA7 == 0) {__delay_ms(SWEEP_FREQ); return 10;}
    LATAbits.LA0 = 1;
    LATAbits.LA1 = 0;
    LATAbits.LA2 = 1;
    LATAbits.LA3 = 1;
    __delay_ms(SWEEP_FREQ);
    if      (PORTAbits.RA4 == 0) {__delay_ms(SWEEP_FREQ); return 4;}
    else if (PORTAbits.RA5 == 0) {__delay_ms(SWEEP_FREQ); return 5;}
    else if (PORTAbits.RA6 == 0) {__delay_ms(SWEEP_FREQ); return 6;}
    else if (PORTAbits.RA7 == 0) {__delay_ms(SWEEP_FREQ); return 11;}
    LATAbits.LA0 = 1;
    LATAbits.LA1 = 1;
    LATAbits.LA2 = 0;
    LATAbits.LA3 = 1;
    __delay_ms(SWEEP_FREQ);
    if      (PORTAbits.RA4 == 0) {__delay_ms(SWEEP_FREQ); return 7;}
    else if (PORTAbits.RA5 == 0) {__delay_ms(SWEEP_FREQ); return 8;}
    else if (PORTAbits.RA6 == 0) {__delay_ms(SWEEP_FREQ); return 9;}
    else if (PORTAbits.RA7 == 0) {__delay_ms(SWEEP_FREQ); return 12;}
    LATAbits.LA0 = 1;
    LATAbits.LA1 = 1;
    LATAbits.LA2 = 1;
    LATAbits.LA3 = 0;
    __delay_ms(SWEEP_FREQ);
    if      (PORTAbits.RA4 == 0) {__delay_ms(SWEEP_FREQ); return 14;}
    else if (PORTAbits.RA5 == 0) {__delay_ms(SWEEP_FREQ); return 0;}
    else if (PORTAbits.RA6 == 0) {__delay_ms(SWEEP_FREQ); return 15;}
    else if (PORTAbits.RA7 == 0) {__delay_ms(SWEEP_FREQ); return 13;}   
    else return 'x';
}

void send_to_disp(uint32_t disp_word){
        LATB = (char) 0xFF;
        char Lock = disp_word;
        char x;
        char y;
        char w;
        
        if (disp_word != 'x'){
        disp_word = 0x000000FF & disp_word;
        disp_word = disp_word | (x>>24);
        x=y;
        disp_word = disp_word | (y>>16);
        y=w;
        disp_word = disp_word | (w>>8);
        w=Lock;
        uint8_t num_disp = disp_word;
        LATD = char_to_seg(num_disp);
        }
        else ();
        

            
        __delay_ms(SWEEP_FREQ);
    }

uint8_t char_to_seg(uint8_t num){
    uint8_t segments;
    switch(num){
        case 0:  segments = 0b00111111; break;
        case 1:  segments = 0b00000110; break;
        case 2:  segments = 0b01011011; break;
        case 3:  segments = 0b01001111; break;
        case 4:  segments = 0b01100110; break;
        case 5:  segments = 0b01101101; break;
        case 6:  segments = 0b01111101; break;
        case 7:  segments = 0b00000111; break;
        case 8:  segments = 0b01111111; break;
        case 9:  segments = 0b01100111; break;
        case 10: segments = 0b01110111; break;
        case 11: segments = 0b01111100; break;
        case 12: segments = 0b01011000; break;
        case 13: segments = 0b01011110; break;
        case 14: segments = 0b01111001; break;
        default: segments = 0b01110001; break;
    }
    return segments;
}