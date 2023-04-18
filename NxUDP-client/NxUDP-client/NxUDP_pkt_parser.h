#ifndef NxUDP_PKT_H
#define NxUDP_PKT_H
#include <stdint.h>


#define OFFSET_session_id		0
#define OFFSET_protocol_ver		OFFSET_session_id+4 
#define OFFSET_sw_ver 			OFFSET_protocol_ver+2	
#define OFFSET_timestamp 		OFFSET_sw_ver+2
#define OFFSET_FC				OFFSET_timestamp+4
#define OFFSET_DFC				OFFSET_FC+2
#define OFFSET_TDFC				OFFSET_DFC+2
#define OFFSET_FT				OFFSET_TDFC+2
#define OFFSET_PT				OFFSET_FT+1
#define OFFSET_dev_id			OFFSET_PT+1
#define OFFSET_error_flag 		OFFSET_dev_id+2
#define OFFSET_reserved 		OFFSET_error_flag+1
#define OFFSET_payload_size 	OFFSET_reserved+2
#define OFFSET_cy_key			OFFSET_payload_size+2
#define OFFSET_CRC				OFFSET_cy_key+4
#define OFFSET_payload			OFFSET_CRC+4



void nxudp_update_ft(uint16_t frame_type);
void nxudp_update_tdfc(uint16_t total_pkts);
void nxudp_parse_packet(uint8_t *data_buff, uint16_t data_size);


#pragma pack(push, 1)
 struct NxUDP_pkt
{
	uint32_t	session_id;
	uint16_t	protocol_ver;
	uint16_t	sw_ver;
	uint32_t	timestamp;
	uint16_t	FC;			//  frame_count
	uint16_t	DFC;		//	Data Fragment Count
	uint16_t	TDFC;		// (Total data fragment count)	
	uint8_t		FT;			// Frame Type(SOF, EOF, DIAGF..)	
	uint8_t		PT;			// Packet Type(SPF, MPF, ..)	
	uint16_t	dev_id;		//device ID
	uint16_t		error_flag;
	uint16_t	reserved;
	uint16_t	payload_size;
	uint32_t	cy_key;		// cyber encryption key
	uint32_t	CRC;
	uint8_t		payload[ENCRYPTION_BUFF_SIZE];	// varies in runtime
}nxudp_pkt, *Pnxudp_pkt;
#pragma pack(pop)
enum Frame_type
{
	SOFR = 0,		//start of frame
	EOFR,			// End of frame
	FRAGFR,			// fragmented frame
	DIAGFR			//Diag frame
}FT;

enum Packet_type
{
	SPF = 0,
	MPF
}PT;

#endif