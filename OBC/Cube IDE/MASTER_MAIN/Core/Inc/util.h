/*
 * util.h
 *
 *  Created on: Jul 21, 2023
 *      Author: Alif
 */

#ifndef INC_UTIL_H_
#define INC_UTIL_H_

#include "main.h"

// SUBSYSTEM ADDRESS
const uint8_t ADDR_MAIN	= 90;
const uint8_t ADDR_RDT	= 92;
const uint8_t ADDR_TTC	= 94;
const uint8_t ADDR_EPS	= 96;

// PAYLOAD ADDRESS
const uint8_t ADDR_ADSB	= 98;
const uint8_t ADDR_LORA	= 100;
const uint8_t ADDR_AIS	= 102;

// HEADER
const uint8_t HEAD_MAIN	= 0x7D; // OBC to ALL
const uint8_t HEAD_RDT	= 0x7E; // RDT to OBC
const uint8_t HEAD_TTC	= 0x7F; // TTC to OBC
const uint8_t HEAD_EPS	= 0x80; // EPS to OBC
const uint8_t HEAD_ADSB	= 0x81; // ADSB to OBC
const uint8_t HEAD_LORA	= 0x82; // LORA to OBC
const uint8_t HEAD_AIS	= 0x83; // AIS to OBC

// BUFFER
uint8_t ADDR_I2C		=	0xFF;	// BUFFER to store Address to send
uint8_t I2C_RX_BUF[55]	= {0xFF}; // BUFFER to Receive Data from other Payloads/Boards
uint8_t I2C_TX_BUF[5]	= {0xFF}; // BUFFER to Transmit Command to other Payloads/Boards
uint8_t TX_BUF[191]		= {0xFF}; // BUFFER to be Transmitted to Ground Station

// Status
HAL_StatusTypeDef RES_TX_I2C;
HAL_StatusTypeDef RES_RX_I2C;

void ParseCommand();
void ParseRes();
void Clr_I2C_TX();

#endif /* INC_UTIL_H_ */
