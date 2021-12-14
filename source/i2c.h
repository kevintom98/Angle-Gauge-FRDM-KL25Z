/*
 * i2c.h
 *
 *  Created on: 08-Dec-2021
 *      Author: Kevin Tom
 */

#ifndef I2C_H_
#define I2C_H_



#include <stdint.h>

//MACROS for I2C initlaization
#define I2C_M_START 	I2C0->C1 |= I2C_C1_MST_MASK
#define I2C_M_STOP  	I2C0->C1 &= ~I2C_C1_MST_MASK
#define I2C_M_RSTART 	I2C0->C1 |= I2C_C1_RSTA_MASK

#define I2C_TRAN			I2C0->C1 |= I2C_C1_TX_MASK
#define I2C_REC				I2C0->C1 &= ~I2C_C1_TX_MASK

#define BUSY_ACK 	    while(I2C0->S & 0x01)
#define TRANS_COMP		while(!(I2C0->S & 0x80))
#define I2C_WAIT 			i2c_wait();

#define NACK 	        I2C0->C1 |= I2C_C1_TXAK_MASK
#define ACK           I2C0->C1 &= ~I2C_C1_TXAK_MASK


/*
 * This function sets initializes i2c on the device to accelerometer
 *
 * Parameters : None
 *
 * Returns : None
 *
 * */
void i2c_init(void);


/*
 * This function sends the i2c start sequence
 *
 * Parameters : None
 *
 * Returns : None
 *
 * */
void i2c_start(void);

/*
 * This function sets the line for read
 *
 * Parameters : None
 *
 * Returns : None
 *
 * */
void i2c_read_setup(uint8_t dev, uint8_t address);

/*
 * This function reads from i2c line and ack/nack as required
 *
 * Parameters : uint8_t isLastRead - is this last read or not
 *
 * Returns : uint8_t - value read
 *
 * */
uint8_t i2c_repeated_read(uint8_t);



/*
 * This function is for reading and writing a single byte using
 * 7 bit addressing reads a byte from dev:address
 *
 * Parameters : uint8_t dev - device
 * 				uint8_t address - device address
 *
 * Returns : uint8_t - value read
 *
 * */
uint8_t i2c_read_byte(uint8_t dev, uint8_t address);

/*
 * This function is for writing a single byte using
 * 7 bit addressing write a byte to dev:address
 *
 * Parameters : uint8_t dev - device
 * 				uint8_t address - device address
 * 				uint8_t data - data to be written
 *
 * Returns : None
 *
 * */
void i2c_write_byte(uint8_t dev, uint8_t address, uint8_t data);

#endif /* I2C_H_ */
