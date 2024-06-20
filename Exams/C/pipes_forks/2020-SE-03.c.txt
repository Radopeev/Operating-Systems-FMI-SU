#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <err.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

typedef struct header {
        char file_name[8];
        uint32_t offset;
        uint32_t length;
} header;

int main(int argc,char* argv[]) {
        if(argc != 2) {
                errx(1,"invalid args");
        }
        int input = open(argv[1],O_RDONLY);
        if(input < 0) {
                err(1,"input error");
        }

        struct stat st;
        if(fstat(input,&st) < 0) {
                close(input);
                err(1,"fstat error");
        }

        if(st.st_size % sizeof(header) != 0) {
                close(input);
                errx(1,"invalid file");
        }

        int headersCount = st.st_size / sizeof(header);
        if(headersCount > 8) {
                close(input);
                errx(1,"invalid file");
        }

        header headers[headersCount];
        if(read(input,headers,sizeof(headers)) != (ssize_t)sizeof(headers)) {
                err(1,"read error");
        }

        int fds[2];
        if(pipe(fds) < 0) {
                err(1,"pipe error");
        }
        for(int i = 0;i < headersCount;i++) {

                pid_t child = fork();
                if(child < 0) {
                        close(input);
                        err(1,"fork error");
                }

                if(child == 0) {
                        close(fds[0]);

                        uint16_t child_result = 0;
                        int file = open(headers[i].file_name,O_RDONLY);

                        if(lseek(file,headers[i].offset * sizeof(uint16_t),SEEK_SET) < 0) {
                                err(1,"lseek error");
                        }

                        uint16_t el;
                        for(uint32_t j = 0;j < headers[i].length;j++) {
                                if(read(file,&el,sizeof(el)) != sizeof(el)) {
                                        err(1,"read error");
                                }
                                child_result ^= el;
                        }
                        if(write(fds[1],&child_result,sizeof(child_result)) < 0) {
                                err(1,"write error");
                        }
                        close(fds[1]);
                        close(file);
                        exit(0);
                }
        }

        close(fds[1]);

        uint16_t parent_result = 0;
        uint16_t num;
        for(int i = 0;i < headersCount;i++) {
                wait(NULL);
                if(read(fds[0],&num,sizeof(num)) < 0) {
                        err(1,"read error");
                }
                parent_result ^= num;
        }

        dprintf(1,"result: %04X\n",parent_result);

        close(fds[0]);
        close(input);
}
