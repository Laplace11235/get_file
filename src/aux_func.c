#include <unistd.h>

int readline(int fd, char* buf, int size){
	int i;
	for(i = 0; i < size; ++i){
            if(read(fd, &buf[i], 1) < 0){
                return -1;
            }
			if(buf[i] == '\n'){
				return i+1;
			}
	}
	return size;
} 
