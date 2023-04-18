#ifndef NxUDP_H
#define NxUDP_H

#define FILE_PATH "..\\..\\Input_image\\Input.JPG"

#define ENCRYPTION_SIZE 4	//32bit

#define PACKET_SIZE (1000/ENCRYPTION_SIZE)

#define ENCRYPTION_BUFF_SIZE PACKET_SIZE*4



int read_image_file(char* read_buff,int bytes);
int load_image_file();
void close_file();

#endif