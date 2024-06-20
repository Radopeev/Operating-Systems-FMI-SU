#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <err.h>

void print_file(int fd,int number_lines,int* line_count);
void print_file(int fd, int number_lines, int* line_count) {
        char buff[1024];
        int read_size;
        while((read_size=read(fd,&buff,sizeof(buff) - 1 )) > 0) {
                buff[read_size] = '\0';

                if(number_lines) {
                        dprintf(STDOUT_FILENO,"%d %s",(*line_count)++,buff);
                } else {
                        dprintf(STDOUT_FILENO,"%s", buff);
                }
        }

        if(read_size == -1){
                err(1,"adssd");
        }
}


int main(int argc, char* argv[]) {
        int number_lines = 0;
        int line_count = 0;
        int file_start_index;

        if(argc > 2 && strcmp(argv[1],"-n") == 0) {
                number_lines = 1;
                file_start_index=3;
        }

        if(file_start_index == argc) {
                print_file(STDIN_FILENO,number_lines,&line_count);
        } else {
                for(int i = file_start_index;i < argc;i++) {
                        if(strcmp(argv[i],"-") == 0) {
                                print_file(STDIN_FILENO,number_lines, &line_count);
                        } else {
                                int fd = open(argv[i],O_RDONLY);
                                if(fd < 0){
                                        err(1,"adsasdsa");
                                }

                                print_file(fd,number_lines,&line_count);
                                close(fd);
                        }
                }
        }
}
