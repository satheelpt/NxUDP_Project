#include <stdio.h>
#include <winsock2.h>
#include <time.h>
#include "NxUDP-server.h"
#include "TEA.h"
#include "NxUDP_pkt_parser.h"

#define PROTOCOL_VER	12
#define SOFTWARE_VER	458
#define DEVICE_ID		6895



//most of parameters are set to static values as it is just a prototype expains the model with single image tranfer
uint8_t buff[10000];

void load_32bit(uint32_t val, uint16_t pos);
void load_16bit(uint16_t val, uint16_t pos);
void load_8bit(uint8_t val, uint16_t pos);
void load_payload(uint8_t *payload, uint16_t payload_size, uint16_t pos);
void load_data_buffer();

void init_nxudp_pkt()
{
	nxudp_pkt.session_id = 1;
	nxudp_pkt.protocol_ver = PROTOCOL_VER;
	nxudp_pkt.sw_ver = SOFTWARE_VER;
	nxudp_pkt.dev_id = DEVICE_ID;
	nxudp_pkt.FC = 1;		// sending only one image frame for testing 
	nxudp_pkt.PT = SPF;		//using only single packet frame for testing
	nxudp_pkt.cy_key = CY_KEY;
}

void nxudp_build_packet(uint8_t *payload, uint16_t curr_pkt_cnt, uint16_t pkt_size)
{	
	nxudp_pkt.timestamp = (unsigned)time;
	nxudp_pkt.DFC = curr_pkt_cnt;
	nxudp_pkt.payload_size = pkt_size;
	memcpy(nxudp_pkt.payload, payload, pkt_size);

	load_data_buffer();


}

void load_data_buffer()
{
	load_32bit(nxudp_pkt.session_id, OFFSET_session_id);
	load_16bit(nxudp_pkt.protocol_ver, OFFSET_protocol_ver);
	load_16bit(nxudp_pkt.sw_ver, OFFSET_sw_ver);
	load_32bit(nxudp_pkt.timestamp, OFFSET_timestamp);
	load_16bit(nxudp_pkt.FC, OFFSET_FC);
	load_16bit(nxudp_pkt.DFC, OFFSET_DFC);
	load_16bit(nxudp_pkt.TDFC, OFFSET_TDFC);
	load_8bit(nxudp_pkt.FT, OFFSET_FT);
	load_8bit(nxudp_pkt.PT, OFFSET_PT);
	load_16bit(nxudp_pkt.dev_id, OFFSET_dev_id);
	load_8bit(nxudp_pkt.error_flag, OFFSET_error_flag);
	load_16bit(nxudp_pkt.reserved, OFFSET_reserved);
	load_16bit(nxudp_pkt.payload_size, OFFSET_payload_size);
	load_32bit(nxudp_pkt.cy_key, OFFSET_cy_key);
	load_32bit(nxudp_pkt.CRC, OFFSET_CRC);
	load_payload(nxudp_pkt.payload, nxudp_pkt.payload_size, OFFSET_payload);
}
void load_32bit(uint32_t val, uint16_t pos)
{
	buff[pos] = ((val>> 24) & 0xFF);
	buff[pos + 1] = ((val >> 16) & 0xFF);
	buff[pos + 2] = ((val >> 8) & 0XFF);
	buff[pos + 3] = (val & 0XFF);
}
void load_16bit(uint16_t val, uint16_t pos)
{
	buff[pos] = ((val >> 8) & 0XFF);
	buff[pos + 1] = (val & 0XFF);
}
void load_8bit(uint8_t val, uint16_t pos)
{
	buff[pos] = val;
	
}
void load_payload(uint8_t *payload, uint16_t payload_size, uint16_t pos)
{
	memcpy(&buff[pos], payload, payload_size);

}
void nxudp_update_tdfc(uint16_t total_pkts)
{
	nxudp_pkt.TDFC = total_pkts;
}

void nxudp_update_ft(uint16_t frame_type)
{
	nxudp_pkt.FT = frame_type;
}

