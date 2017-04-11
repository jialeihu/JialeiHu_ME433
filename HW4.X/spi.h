#ifndef SPI_H
#define SPI_H

void initSPI1(void);
char SPI1_IO(char write);
void setVoltage(char channel, unsigned char voltage);
#endif