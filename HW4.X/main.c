#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include"spi.h"
#include<math.h>

static volatile unsigned char WaveSinform[100];
static volatile unsigned char WaveDelform[200];


void makeWaveform();


// DEVCFG0
#pragma config DEBUG = OFF // no debugging
#pragma config JTAGEN = OFF // no jtag
#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
#pragma config PWP = OFF // no write protect
#pragma config BWP = OFF // no boot write protect
#pragma config CP = OFF // no code protect

// DEVCFG1
#pragma config FNOSC = PRIPLL // use primary oscillator with pll
#pragma config FSOSCEN = OFF // turn off secondary oscillator
#pragma config IESO = OFF // no switching clocks
#pragma config POSCMOD = HS // high speed crystal mode
#pragma config OSCIOFNC = OFF // free up secondary osc pins
#pragma config FPBDIV = DIV_1 // divide CPU freq by 1 for peripheral bus clock
#pragma config FCKSM = CSDCMD // do not enable clock switch
#pragma config WDTPS = PS1048576 // slowest wdt
#pragma config WINDIS = OFF // no wdt window
#pragma config FWDTEN = OFF // wdt off by default
#pragma config FWDTWINSZ = WINSZ_25 // wdt window at 25%

// DEVCFG2 - get the CPU clock to 48MHz
#pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz
#pragma config FPLLMUL = MUL_24 // multiply clock after FPLLIDIV
#pragma config FPLLODIV = DIV_2 // divide clock after FPLLMUL to get 48MHz
#pragma config UPLLIDIV = DIV_2 // divider for the 8MHz input clock, then multiply by 12 to get 48MHz for USB
#pragma config UPLLEN = ON // USB clock on

// DEVCFG3
#pragma config USERID = 0 // some 16bit userid, doesn't matter what
#pragma config PMDL1WAY = OFF // allow multiple reconfigurations
#pragma config IOL1WAY = OFF // allow multiple reconfigurations
#pragma config FUSBIDIO = ON // USB pins controlled by USB module
#pragma config FVBUSONIO = ON // USB BUSON controlled by USB module

#define CS LATBbits.LATB15  // chip select pin


int main() {

    // do your TRIS and LAT commands here
	TRISAbits.TRISA4 = 0;	  // RA4 as output
	TRISBbits.TRISB4 = 1;     // RB4 as input
	LATAbits.LATA4 = 1;      // RA4 is high
	initSPI1();
    makeWaveform();
    int test1 = 0;
    int test2 = 0;
    while(1){
        _CP0_SET_COUNT(0);
        while( _CP0_GET_COUNT() <= 12000){
          ;
        }
        setVoltage(0, WaveSinform[test1]);
        setVoltage(1, WaveDelform[test2]);
        test1 = test1 + 1;
        test2 = test2 + 1;
        if(test1 == 100){
            test1 = 0;
        }
        if(test2 == 200){
            test2 = 0;
        }
    }

}

void makeWaveform(){
	int i = 0;
	for(i = 0 ; i < 100;++i){
       WaveSinform[i] = 255*sin((double)(i/100*2*3.14));
	}
	int j = 0;
	for(j = 0 ; j < 200;++j){
       WaveDelform[j] = 255*(double)(j/200);
	}
}