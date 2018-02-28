/*
 * twelite_asciimode.h
 *
 *  Created on: 2018/02/24
 *      Author: feunoir
 */

#ifndef TWELITE_FORMAT_H_
#define TWELITE_FORMAT_H_

#include "stm32f3xx_hal.h"
#include "twelite_logicid.h"

typedef enum {
	TweLite_Result_OK,
	TweLite_Result_Err
}TweLite_Result_t;

TweLite_Result_t twelite_encode_ascii(TweLite_LogicID_t dest_id, uint8_t cmd, char* str_in, char* str_out, uint16_t len_out);

TweLite_LogicID_t twelite_getid_ascii(char* str_in);

uint8_t twelite_getcmd_ascii(char* str_in);

TweLite_Result_t twelite_chksum_ascii(char* str_in);

TweLite_Result_t twelite_decode_ascii(char* str_in, char* str_out, uint16_t len_out);


#endif /* TWELITE_FORMAT_H_ */
