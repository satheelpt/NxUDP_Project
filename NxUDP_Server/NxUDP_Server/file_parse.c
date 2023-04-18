#include <stdio.h>
#include "NxUDP-server.h"



static int get_file_size();

static FILE *img_ptr = NULL;

int file_size = 0;
int pkt_cnt = 0;
int resd_bytes = 0;


int load_image_file()
{
	
	char ch;
		
	img_ptr = fopen(FILE_PATH, "rb");
	
	if (NULL == img_ptr) {
		printf("file can't be opened \n");
	}
	file_size = get_file_size();
	pkt_cnt =  file_size / PACKET_SIZE;
	resd_bytes = file_size - (pkt_cnt*PACKET_SIZE);
	if (resd_bytes > 0)
	{
		pkt_cnt += 1;
	}
	
	return 0;
}

void close_file()
{
	fclose(img_ptr);
}

int read_image_file(char* read_buff, int bytes)
{
	
	int bytes_read = 0;	
	bytes_read = fread(read_buff, 1, PACKET_SIZE, img_ptr);
	return bytes_read;
}

static int get_file_size()
{
	int size = 0;
	fseek(img_ptr, 0L, SEEK_END);
	size= ftell(img_ptr);
	fseek(img_ptr, 0L, SEEK_SET);
	return size;
}