#include "i2c.h"

// I2C Master utilities, 100 kHz, using polling rather than interrupts
// The functions must be callled in the correct order as per the I2C protocol
// Change I2C2 to the I2C channel you are using
// I2C pins need pull-up resistors, 2k-10k

void i2c_master_setup(void) {
    I2C2BRG = 233;            // I2CBRG = [1/(2*Fsck) - PGD]*Pblck - 2 
                                    // look up PGD for your PIC32
    I2C2CONbits.ON = 1;               // turn on the I2C2 module
}

// Start a transmission on the I2C bus
void i2c_master_start(void) {
    I2C2CONbits.SEN = 1;            // send the start bit
    while(I2C2CONbits.SEN) { ; }    // wait for the start bit to be sent
}

void i2c_master_restart(void) {     
    I2C2CONbits.RSEN = 1;           // send a restart 
    while(I2C2CONbits.RSEN) { ; }   // wait for the restart to clear
}

void i2c_master_send(unsigned char byte) { // send a byte to slave
    I2C2TRN = byte;                   // if an address, bit 0 = 0 for write, 1 for read
    while(I2C2STATbits.TRSTAT) { ; }  // wait for the transmission to finish
    if(I2C2STATbits.ACKSTAT) {        // if this is high, slave has not acknowledged
		LATAbits.LATA4 = 1;
	    //("I2C2 Master: failed to receive ACK\r\n");
    }
}

unsigned char i2c_master_recv(void) { // receive a byte from the slave
    I2C2CONbits.RCEN = 1;             // start receiving data
    while(!I2C2STATbits.RBF) { ; }    // wait to receive the data
    return I2C2RCV;                   // read and return the data
}

void i2c_master_ack(int val) {        // sends ACK = 0 (slave should send another byte)
                                      // or NACK = 1 (no more bytes requested from slave)
    I2C2CONbits.ACKDT = val;          // store ACK/NACK in ACKDT
    I2C2CONbits.ACKEN = 1;            // send ACKDT
    while(I2C2CONbits.ACKEN) { ; }    // wait for ACK/NACK to be sent
}

void i2c_master_stop(void) {          // send a STOP:
    I2C2CONbits.PEN = 1;                // comm is complete and master relinquishes bus
    while(I2C2CONbits.PEN) { ; }        // wait for STOP to complete
}

void i2c_read_multiple(unsigned char address, unsigned char regis, unsigned char * data, int length){
    unsigned char * temp;
    int j;
    temp = data;
    i2c_master_start(); // make the start bit
    i2c_master_send(address << 1 | 0b0); // write the address, shifted left by 1, or'ed with a 0 to indicate writing
    i2c_master_send(regis); // the register to read from
    i2c_master_restart(); // make the restart bit
    i2c_master_send(address << 1 | 0b1); // write the address, shifted left by 1, or'ed with a 1 to indicate reading  
    for (j = 0; j < (length - 1); j++) {
        * (temp + j) = i2c_master_recv();
        i2c_master_ack(0);
    }
    * (temp + j)= i2c_master_recv();
    i2c_master_ack(1); // make the ack so the slave knows we got it
    i2c_master_stop(); // make the stop bit
}

void i2c2_init(void){
	ANSELBbits.ANSB2 = 0;    // turn off analog
	ANSELBbits.ANSB3 = 0;
	i2c_master_setup();
	i2c_master_start();
	/*i2c_master_send(0xd4);  // (SAD+) write mode
	i2c_master_send(0x0f);  // want to read WHOAMI address
	i2c_master_restart();
	i2c_master_send(0xd5);  // (SAD+) read mode
	char who_am_i = i2c_master_recv(); // receive
	i2c_master_ack(1);
	i2c_master_stop();
		
	char message[100];
	sprintf(message,"0x%0x",who_am_i);
	print_string(message,48,32,BLACK);   // should be 0x69 or 0b01101001*/	

	
    i2c_master_send(0b11010100); //for write
    i2c_master_send(0b00010000); //CTRL1_XL
    i2c_master_send(0b10000010);// 
    i2c_master_restart();
    i2c_master_send(0b11010100); //for write
    i2c_master_send(0b00010001); //CTRL2_G
    i2c_master_send(0b10001000);// 
    i2c_master_restart();
    i2c_master_send(0b11010100); //for write
    i2c_master_send(0b00010010); //CTRL3_C
    i2c_master_send(0b00000100);//  
    i2c_master_stop();
}