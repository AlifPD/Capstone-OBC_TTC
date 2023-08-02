/*
 * util.c
 *
 *  Created on: Jul 25, 2023
 *      Author: Alif
 */

#include "main.h"
#include "util.h"

void ParseCMD(){
	if(I2C_TX_BUF[5] == 0x80) ADDR_I2C = ADDR_EPS;
	else if(I2C_TX_BUF[5] == 0x7F) ADDR_I2C = ADDR_TTC<<1;
	else if(I2C_TX_BUF[5] == 0x7E) ADDR_I2C = ADDR_RDT;
	else if(I2C_TX_BUF[5] == 0x81) ADDR_I2C = ADDR_ADSB;

	I2C_TX_BUF[0] = HEAD_MAIN;
	I2C_TX_BUF[1] = I2C_RX_BUF[6];
	I2C_TX_BUF[2] = I2C_RX_BUF[7];
	I2C_TX_BUF[3] = I2C_RX_BUF[8];
	I2C_TX_BUF[4] = I2C_RX_BUF[9];
}

void ParseRes(){

}

void Clr_I2C_TX(){
	ADDR_I2C = 0xFF;
	for(int i=0; i<5; i++){
		I2C_TX_BUF[i] = 0xFF;
	}
}
