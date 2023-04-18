#include <stdio.h>
#include <winsock2.h>
#include <time.h>
#include "NxUDP-client.h"
#include "TEA.h"
#include "NxUDP_pkt_parser.h"

#define PROTOCOL_VER	12
#define SOFTWARE_VER	458
#define DEVICE_ID		6895



//most of parameters are set to static values as it is just a prototype expains the model with single image tranfer
uint32_t parse_32bit(uint8_t *buff, uint16_t pos);
uint16_t parse_16bit(uint8_t *buff, uint16_t pos);
uint8_t parse_8bit(uint8_t *buff, uint16_t pos);
void parse_payload(uint8_t *buff, uint8_t *payload, uint16_t payload_size, uint16_t pos);


void nxudp_parse_packet(uint8_t *data_buff, uint16_t data_size)
{
	
	nxudp_pkt.session_id = parse_32bit(data_buff, OFFSET_session_id);
	nxudp_pkt.protocol_ver = parse_16bit(data_buff, OFFSET_protocol_ver);
	nxudp_pkt.sw_ver = parse_16bit(data_buff, OFFSET_sw_ver);
	nxudp_pkt.timestamp = parse_32bit(data_buff, OFFSET_timestamp);
	nxudp_pkt.FC = parse_16bit(data_buff, OFFSET_FC);
	nxudp_pkt.DFC = parse_16bit(data_buff, OFFSET_DFC);
	nxudp_pkt.TDFC = parse_16bit(data_buff, OFFSET_TDFC);
	nxudp_pkt.FT = parse_8bit(data_buff, OFFSET_FT);
	nxudp_pkt.PT = parse_8bit(data_buff, OFFSET_PT);
	nxudp_pkt.dev_id = parse_16bit(data_buff, OFFSET_dev_id);
	nxudp_pkt.error_flag = parse_8bit(data_buff, OFFSET_error_flag);
	nxudp_pkt.reserved = parse_16bit(data_buff, OFFSET_reserved);
	nxudp_pkt.payload_size = parse_16bit(data_buff, OFFSET_payload_size);
	nxudp_pkt.cy_key = parse_32bit(data_buff, OFFSET_cy_key);
	nxudp_pkt.CRC = parse_16bit(data_buff, OFFSET_CRC);
	parse_payload(data_buff, nxudp_pkt.payload, nxudp_pkt.payload_size, OFFSET_payload);
	
}


uint32_t parse_32bit(uint8_t *buff, uint16_t pos)
{
	uint32_t val = 0;
	val = buff[pos+3];
	val = val | (buff[pos + 2] << 8);
	val = val | (buff[pos + 1] << 16);
	val = val | (buff[pos] << 24);
	return val;
}
uint16_t parse_16bit(uint8_t *buff, uint16_t pos)
{
	uint16_t val = 0;
	val = buff[pos+1];
	val = val | (buff[pos] << 8);
	
	return val;
}
uint8_t parse_8bit(uint8_t *buff, uint16_t pos)
{
	uint8_t val = 0;
	val = buff[pos];
	
	return val;
}
void parse_payload(uint8_t *buff, uint8_t *payload,uint16_t payload_size, uint16_t pos)
{
	memcpy(payload, &buff[pos], payload_size);
}

void nxudp_get_tdfc(uint16_t total_pkts)
{
	nxudp_pkt.TDFC = total_pkts;
}

void nxudp_get_ft(uint16_t frame_type)
{
	nxudp_pkt.FT = frame_type;
}

