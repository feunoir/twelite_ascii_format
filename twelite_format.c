/*
 * twelite_format.c
 *
 *  Created on: 2018/02/24
 *      Author: feunoir
 */

#include "twelite_format.h"
#include <string.h>

TweLite_Result_t twelite_encode_ascii(
		TweLite_LogicID_t dest_id,
		uint8_t cmd,
		char* str_in,
		char* str_out,
		uint16_t len_out
		)
{
	int i = 0;
	char hex_c[2];
	int len_sentence = strlen(str_in);

	if( len_out < (len_sentence*2+6) ) {
		return TweLite_Result_Err;
	}

	memset(str_out, '\0', len_out);

	str_out[0] = ':';

	sprintf(hex_c, "%02X", dest_id);
	strncat(str_out, hex_c, 2);
	sprintf(hex_c, "%02X", cmd);
	strncat(str_out, hex_c, 2);

	for(i = 0; i < len_sentence; i++) {
		sprintf(hex_c, "%02X", str_in[i]);
		strncat(str_out, hex_c, 2);
	}

	strncat(str_out, "X", 1);

	return TweLite_Result_OK;
}


static uint8_t twelite_getchar(char* str_in, uint16_t pos) {
	char hex_c[2];
	unsigned int result;

	if( (pos > (strlen(str_in)-4)) || pos == 0 ) {
		return 0xff;	//	Error
	}

	hex_c[0] = str_in[pos];
	hex_c[1] = str_in[pos + 1];

	sscanf(hex_c, "%2X", &result);

	return (uint8_t)result;
}


TweLite_LogicID_t twelite_getid_ascii(char* str_in)
{
	uint8_t id = twelite_getchar(str_in, 1);

	if( !((id >= 0 && id <= 100) || id == 120) ) {
		return TweLite_LogicID_Unknown;
	}

	return id;
}


uint8_t twelite_getcmd_ascii(char* str_in)
{
	return twelite_getchar(str_in, 3);
}


TweLite_Result_t twelite_chksum_ascii(char* str_in)
{
	uint8_t chksum;
	uint8_t sum = 0;

	int i;
	int len_sentence = strlen(str_in);

	chksum = twelite_getchar(str_in, len_sentence-4);


	for(i = 1; i < (len_sentence-4); i += 2) {
		sum += twelite_getchar(str_in, i);
	}

	if( (uint8_t)(sum + chksum) != 0 ) {
		return TweLite_Result_Err;
	}

	return TweLite_Result_OK;
}


TweLite_Result_t twelite_decode_ascii(char* str_in, char* str_out, uint16_t len_out)
{
	int i = 5;
	int len_sentence = strlen(str_in);

	if( len_out < ((len_sentence-6)>>1) ) {
		return TweLite_Result_Err;
	}

	if(!(str_in[0] == ':')) {
		return TweLite_Result_Err;
	}

	if(twelite_chksum_ascii(str_in) == TweLite_Result_Err) {
		return TweLite_Result_Err;
	}

	memset(str_out, '\0', len_out);
	for(i = 0; i < ((len_sentence-8)>>1);i++) {
		str_out[i] = twelite_getchar(str_in, (i*2+5));
	}
	return TweLite_Result_OK;

}
