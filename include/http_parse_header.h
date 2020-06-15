#ifndef PARSE_HTTP_HEADER_H
#define PARSE_HTTP_HEADER_H

struct HttpHeader
{
    int contentLength;
};

int http_check_resp(int fd);
int http_parse_header(int fd, struct HttpHeader* header);
int http_read_content(int inFd, int outFd, int len);

#endif
