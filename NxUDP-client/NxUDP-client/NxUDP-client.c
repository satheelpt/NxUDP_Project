#include <stdio.h>
#include <winsock2.h>
#include "NxUDP-client.h"
#include "TEA.h"
#include "NxUDP_pkt_parser.h"


#pragma comment(lib, "Ws2_32.lib")

#define RECEIVER_PORT_NUM 6001
#define RECEIVER_IP_ADDR "127.0.0.1"

void initialize_udp_client(void);

WSADATA wsaData;

int32_t recv_data; char data_buffer[sizeof(nxudp_pkt)];
int32_t recv_size = 0;
int32_t decrypted_size = 0;

uint8_t Decryptd_data[ENCRYPTION_BUFF_SIZE];
int32_t socket_fd, ra_len;
	struct sockaddr_in ra, sa;

void main(void)
{
	initialize_udp_client();	
	create_image_file();
	while (1) {
		recv_data = recvfrom(socket_fd, data_buffer, sizeof(nxudp_pkt), 0, (struct sockaddr *)&ra, &ra_len);

		nxudp_parse_packet(data_buffer,recv_data);

		Do_TEA_Decryption(nxudp_pkt.payload, nxudp_pkt.payload_size, &nxudp_pkt.cy_key);
		decrypted_size = (nxudp_pkt.payload_size / ENCRYPTION_SIZE) - 0;

		if (decrypted_size < 0)
		{
			printf("Receive failed\n");
			close(socket_fd);
			exit(2);
		}
		if (nxudp_pkt.FT == EOFR)
		{			
			printf("*************************************\n");
			printf("EOFR--> %d\n", nxudp_pkt.TDFC);
			printf("*************************************\n");
			close_file();
			break;	
		}
		
		write_image_file(Decryptd_data, nxudp_pkt.payload_size / ENCRYPTION_SIZE);
		
		printf("DFC -->%d  payload size --> %d\n", nxudp_pkt.DFC, nxudp_pkt.payload_size);

	}
}

void initialize_udp_client(void)
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

	memset(&ra, 0, sizeof(struct sockaddr_in));
	ra.sin_family = AF_INET;
	ra.sin_addr.s_addr = inet_addr(RECEIVER_IP_ADDR);
	ra.sin_port = htons(RECEIVER_PORT_NUM);

	ra_len = sizeof(ra);

	/* Bind the UDP socket to the port RECEIVER_PORT_NUM and to the current
	* machines IP address (Its defined by RECEIVER_PORT_NUM).
	* Once bind is successful for UDP sockets application can operate
	* on the socket descriptor for sending or receiving data.
	*/
	// Bind the created socket to a local IP address and port number
	if (bind(socket_fd, (struct sockaddr *)&ra, sizeof(struct sockaddr_in)) == -1)
	{
		printf("Bind to Port Number %d ,IP address %s failed!\n", RECEIVER_PORT_NUM, RECEIVER_IP_ADDR);
		close(socket_fd);
		exit(1);
	}
}