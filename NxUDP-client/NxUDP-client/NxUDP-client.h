#ifndef NxUDP_CLIENT_H
#define NxUDP_CLIENT_H

#define FILE_PATH "..\\..\\Received_image\\Received.JPG"

#define ENCRYPTION_SIZE 4	//32bit

#define PACKET_SIZE (1000/ENCRYPTION_SIZE)

#define ENCRYPTION_BUFF_SIZE PACKET_SIZE*4


int write_image_file(char* recv_buff, int bytes);
int create_image_file();
void close_file();

#endif