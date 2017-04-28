#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include<math.h>
#include"i2c.h"
#include "ILI9163C.h"

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

#define BARWIDTH 5
void PlotAccelerometer(short gx, short gy);


int main() {


    
    SPI1_init();
	LCD_init();
	LCD_clearScreen(BLACK);
    i2c2_init();
    unsigned char datatemp[100];
	short data[100];
    int i = 0;
    
    
    
	while (1){
		_CP0_SET_COUNT(0);
		while (_CP0_GET_COUNT()<4800000) {;}	
        i2c_read_multiple(0b1101010, 0b00100000, datatemp, 14);
        for(i=0; i<7; i++){
			data[i] = datatemp[2*i+1]<<8 | datatemp[2*i];
		}
		PlotAccelerometer(data[4],data[5]);
        
        
	}

}


void PlotAccelerometer(short gx, short gy){
	int x_length, y_length;
	x_length = (int)((float)gx/16500*64);
	y_length = (int)((float)gy/16500*64);
	int abs_x_length = x_length, abs_y_length = y_length;
	
	if(x_length<0){
		abs_x_length = -x_length;
	}
	if(y_length<0){
		abs_y_length = -y_length;
	}
	int i,j;
	clearBar(0,128,64-BARWIDTH,64+BARWIDTH,BLACK);
	clearBar(64-BARWIDTH,64+BARWIDTH,0,128,BLACK);
	for (i=0; i<abs_x_length; i++){
		for(j=0; j<BARWIDTH; j++){
			LCD_drawPixel((x_length/abs_x_length)*i+64,j+64-BARWIDTH/2,RED);
		}
	}
	for (i=0; i<abs_y_length; i++){
		for(j=0; j<BARWIDTH; j++){
			LCD_drawPixel(j+64-BARWIDTH/2,(y_length/abs_y_length)*i+64,BLUE);
		}
	}
}

