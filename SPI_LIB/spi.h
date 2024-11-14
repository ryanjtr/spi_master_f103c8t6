/*
 * spi.h
 *
 *  Created on: Nov 13, 2024
 *      Author: dell
 */

#ifndef SPI_H_
#define SPI_H_

#include "main.h"
#include "stdbool.h"



bool spi_transmit(uint8_t *pData, uint8_t len, uint32_t timeout);
bool spi_transmit(uint8_t *pData, uint8_t len, uint32_t timeout);
bool spi_transmit_receive(uint8_t *txdata, uint8_t *rxdata, uint8_t len, uint32_t timeout);
void read_id_manufacter(uint8_t rxdata[4]);
void reset_device(void);
void write_data(uint32_t Address, uint8_t *pdata, uint16_t size);
#endif /* SPI_H_ */
