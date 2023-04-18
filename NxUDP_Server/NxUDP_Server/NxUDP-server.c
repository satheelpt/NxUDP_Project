#include <stdio.h>
#include <winsock2.h>
#include "NxUDP-server.h"
#include "TEA.h"
#include "NxUDP_pkt_parser.h"

#pragma comment(lib, "Ws2_32.lib")

#define SENDER_PORT_NUM 6000
#define SENDER_IP_ADDR "127.0.0.1"
#define RECEIVER_PORT_NUM 6001
#define RECEIVER_IP_ADDR "127.0.0.1"

void initialize_udp_server(void);

WSADATA wsaData;

char data_buff[sizeof(nxudp_pkt)];
uint16_t data_size = 0;
char encrypted_data[10000];
static int32_t sent_pkt_cnt = 0;

extern int32_t pkt_cnt;
extern int32_t resd_bytes;
extern uint32_t Encryptd_data[ENCRYPTION_BUFF_SIZE];
extern uint8_t buff[10000];
int32_t socket_fd, ra_len;
	struct sockaddr_in sa, ra;

void main(void)
{
	int32_t bytes_read = 0;
	int32_t sent_data;
	sent_data = ENCRYPTION_BUFF_SIZE + sizeof(nxudp_pkt);
	initialize_udp_server();
	init_nxudp_pkt();
	load_image_file();


	// set total packets count
	nxudp_update_tdfc(pkt_cnt);
	printf("*************************************\n");
	printf("TDFC--> %d\n", pkt_cnt);
	printf("*************************************\n");
	while (sent_pkt_cnt!=pkt_cnt)
	{
		bytes_read = read_image_file(data_buff, PACKET_SIZE);
		
		Do_TEA_Encryption(data_buff, bytes_read, &nxudp_pkt.cy_key);

		if (sent_pkt_cnt == 0)
		{
			//set start of frame
			nxudp_update_ft(SOFR);
		}
		else
		{
			//set fragmented frame
			nxudp_update_ft(FRAGFR);
		}

		if (bytes_read == PACKET_SIZE)
		{
			data_size = sizeof(nxudp_pkt);
		}
		else
		{
			data_size = sizeof(nxudp_pkt)-(PACKET_SIZE - bytes_read);
			nxudp_update_ft(EOFR);
		}

		nxudp_build_packet(Encryptd_data, sent_pkt_cnt + 1, bytes_read*ENCRYPTION_SIZE);
		
		sent_data = sendto(socket_fd, buff, data_size, 0, (struct sockaddr*)&ra, &ra_len);
						
		if (sent_data < 0)
		{

			printf("send failed\n");
			close(socket_fd);
			exit(2);

		}
		printf("DFC--> %d\n", sent_pkt_cnt);
		sent_pkt_cnt++;
	}
	printf("*************************************\n");
	printf("Sent EOFR\n", pkt_cnt);
	printf("*************************************\n");
	
	getch();

}


void initialize_udp_server(void)
{

	


	int32_t err;
	

	err = WSAStartup(0x22, &wsaData);
	if (err != 0) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return;
	}

	socket_fd = socket(AF_INET, SOCK_DGRAM, 0);


	if (socket_fd < 0)
	{

		printf("socket call failed");
		exit(0);

	}

	memset(&sa, 0, sizeof(struct sockaddr_in));
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr(SENDER_IP_ADDR);
	sa.sin_port = htons(SENDER_PORT_NUM);

	memset(&ra, 0, sizeof(struct sockaddr_in));
	ra.sin_family = AF_INET;
	ra.sin_addr.s_addr = inet_addr(RECEIVER_IP_ADDR);
	ra.sin_port = htons(RECEIVER_PORT_NUM);
	ra_len = sizeof(ra);
}