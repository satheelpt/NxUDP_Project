#include <stdio.h>
#include "NxUDP-client.h"



static int get_file_size();

static FILE *img_ptr = NULL;

int file_size = 0;
int pkt_cnt = 0;
int resd_bytes = 0;


int create_image_file()
{

	char ch;

	img_ptr = fopen(FILE_PATH, "wb");

	if (NULL == img_ptr) {
		printf("file can't be opened \n");
	}
	
	return 0;
}

void close_file()
{
	fclose(img_ptr);
}

int write_image_file(char* recv_buff, int bytes)
{

	int bytes_read = 0;
	bytes_read = fwrite(recv_buff, 1, bytes, img_ptr);
	return bytes_read;
}
