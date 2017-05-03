#ifndef I2C_H
#define I2C_H

#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include "ILI9163C.h"

void i2c_master_setup(void);
void i2c_master_start(void);
void i2c_master_restart(void);
void i2c_master_send(unsigned char byte);
unsigned char i2c_master_recv(void);
void i2c_master_ack(int val);
void i2c_master_stop(void);
void i2c_read_multiple(unsigned char address, unsigned char regis, unsigned char * data, int length);
void i2c2_init(void);

#endif