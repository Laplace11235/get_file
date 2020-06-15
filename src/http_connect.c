#include <stdio.h>
#include <string.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>

#include <errno.h>
#include <unistd.h>

#define MAX_REQUEST_LEN	1024

int http_connect(int fd, const char* url){
	in_addr_t inAddr;
	struct hostent *host;
	struct sockaddr_in sockaddrIn;

	host = gethostbyname(url);

	if(host == NULL){
		fprintf(stderr, "error: gethostbyname(\"%s\") %s\n",
		url, hstrerror(h_errno));
		return -1;
	}

	// inAddr
	inAddr = inet_addr(inet_ntoa(*(struct in_addr*)*(host->h_addr_list)));
	if(inAddr == (in_addr_t)-1){
		fprintf(stderr, "error: inet_addr(\"%s\")\n", *(host->h_addr_list));
		return -1;
	}

	sockaddrIn.sin_addr.s_addr = inAddr;
	sockaddrIn.sin_family = AF_INET;
	sockaddrIn.sin_port = htons(80);

	if(connect(fd, (struct sockaddr*)&sockaddrIn,
		 sizeof(sockaddrIn)) == -1){
		fprintf(stderr, "error connect to %s %s\n", url, strerror(errno));
		return -1;
    }

    return 0;
}

int http_request(int fd, const char* url){
	char request[MAX_REQUEST_LEN+1];
	snprintf(request, MAX_REQUEST_LEN,

	"GET / HTTP/1.1\r\nHost: %s\r\n\r\n", url);
	if(write(fd, request, strlen(request)) < 0){
		fprintf(stderr, "error cannot http request %s %s\n", url, strerror(errno));
        return -1;
	}
    return 0;
}
