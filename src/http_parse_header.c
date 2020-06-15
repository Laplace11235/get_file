#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <errno.h>

#include "http_parse_header.h"
#include "aux_func.h"

#define HTTP_RESP_SIZE	128
#define HTTP_HEADER_BUF_SIZE 256
#define HTTP_CONTENT_BUF_SIZE 1024

static int outFd = STDOUT_FILENO;

int http_check_resp(int fd){
		char buf[HTTP_RESP_SIZE];
		int nRead = readline(fd, buf, HTTP_RESP_SIZE);
		if(nRead < 0){
			return -1;
		}
		buf[nRead] = '\0';
		if(strstr(buf, "HTTP/") != NULL && 
			strstr(buf, "OK") != NULL){
				return 0;
		}
		fprintf(stderr, "error check responce %s", buf);
		return -1;	
}

int http_parse_header(int fd, struct HttpHeader* header){
	char buf[HTTP_HEADER_BUF_SIZE];
	while(1){
		int nRead = readline(fd, buf, HTTP_HEADER_BUF_SIZE);
		if(nRead < 0){
			fprintf(stderr, "error reading http responce %s\n", strerror(errno));
			return -1;
		}
		buf[nRead] = '\0';
		if(!strncmp("\r\n", buf, nRead)){
			// end of header
			break;
		}else if(!strncmp(buf, "Content-Length:", 15)){
			char* p = strstr(buf, " ");
			if(p != NULL){
				header->contentLength = atoi(p);
			}
		}
	}
    return 0;
}

int http_read_content(int inFd, int outFd, int len){
	char buf[HTTP_CONTENT_BUF_SIZE];
	int readCount = 0;
	while(readCount < len){
		int nRead = read(inFd, buf, HTTP_CONTENT_BUF_SIZE);
		if(nRead < 0){
			fprintf(stderr, "cannot read content %s", strerror(errno));
			return -1;
		}
		if(write(outFd, buf, nRead) < 0){
			fprintf(stderr, "cannot write to output %s", strerror(errno));
			return -1;
		}
		readCount += nRead;
	}
    return 0;
}
