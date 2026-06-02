#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

/* 
1. Check for arguments: If no file arguments are provided, your program
must read from standard input (file descriptor 0) and write to standard
output (file descriptor 1) until an EOF (Ctrl+D) is received


2. Handle multiple files: If one or more file paths are provided, iterate
through them, open each one, read its content in chunks, and write that
content to standard output.

3. Handle errors: If a file does not exist or cannot be opened, your program
should print an error message and proceed to the next file (if any).

4. Buffered I/O: You should use a fixed-size buffer for reading and writing,
rather than reading one character at a time

*/

void readwrite (int fd) {
    int buf_size = 256;
    char* buffer = malloc(buf_size);
    while(1){
        memset( buffer, 0, buf_size);
        int bytes_read = read (fd, buffer, buf_size - 1);
        int bytes_write = write(1, buffer, bytes_read);
        if (bytes_read <= 0){
            break;
        }
    }
    free(buffer);
}

int main (int argc, char** argv) {
    int fd;
    if (argc == 1){
        readwrite(0);
        return 0;
    }
    
    for (int i = 1; i<argc; i++){
        fd = open(argv[i], O_RDONLY);
        if (fd == -1) {
            printf("Unable to open file ! %s is invalid name \n", argv[i]);
            continue;
        }
        readwrite(fd);
        close (fd);
        printf("\n");

    }
    return 0;

}

