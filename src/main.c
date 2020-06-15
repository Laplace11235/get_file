#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <netinet/in.h>

#include "http_parse_header.h"
#include "http_connect.h"

static void usage(const char* name){
	printf("Usage\n\t%s url [output filename]\n", name);
}

int main(int argc, char* argv[]){
	int socketFd, outFd = STDOUT_FILENO;
	char *url = "example.com";
	struct HttpHeader http_header;
	int res = 0;

	if(argc < 2){
		usage(argv[0]);
		return 0;
	}

	if(argc == 2){
		url = argv[1];
	}

	if(argc > 2){
		outFd = open(argv[2], O_WRONLY | O_CREAT);
		if(outFd < 0){
			fprintf(stderr, "Cannot open %s %s\n", argv[2], strerror(errno));
		}
	}

	socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if(socketFd < 0){
		fprintf(stderr, "Cannot create socket %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	if(http_connect(socketFd, url) < 0){
		res = EXIT_FAILURE;
		goto out;
	}

	if(http_request(socketFd, url) < 0){
		res = EXIT_FAILURE;
		goto out;
	}

	if(http_check_resp(socketFd) != 0){
		res = EXIT_FAILURE;
		goto out;
	}
	if(http_parse_header(socketFd, &http_header) < 0){
		res = EXIT_FAILURE;
		goto out;
	}
	if(http_read_content(socketFd, outFd, http_header.contentLength) < 0){
		res = EXIT_FAILURE;
		goto out;
	}

out:
	close(socketFd);
	close(outFd);
	return res;
}

