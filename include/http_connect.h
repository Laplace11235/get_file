#ifndef HTTP_CONNECT_H
#define HTTP_CONNECT_H

int http_connect(int fd, const char* url);
int http_request(int fd, const char* url);

#endif
