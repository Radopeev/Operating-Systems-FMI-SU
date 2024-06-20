#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <string.h>

const char* L[] = {"tic ","tac ","toe \n"};

void handleMessage(int,int,int);

void handleMessage(int wc, int from, int to) {
        int count;
        ssize_t read_size;
        int written_final_signal = 0;

        while((read_size = read(from,&count,sizeof(count))) > 0) {
                if(count >= wc) {
                        if(!written_final_signal) {
                                if(write(to,&count,sizeof(count)) < 0) {
                                        err(1,"write error");
                                }
                        }
                        return;
                }
                if(write(1,L[count % 3],strlen(L[count % 3])) < 0) {
                        err(1,"write error");
                }

                int next = count + 1;
                if(write(to,&next,sizeof(next)) < 0) {
                        err(1,"write error");
                }

                if(next >= wc) {
                        written_final_signal = 1;
                }
        }
        if(read_size == 0) {
                errx(2,"error");
        }

        if(read_size < 0) {
                err(1,"read");
        }
}

int main(int argc,char* argv[]) {
        if(argc != 3) {
                errx(1,"invalid arguments");
        }

        char* endptr;
        int nc = strtol(argv[1],&endptr,10);
        if(*endptr != '\0' || nc < 1 || nc > 7) {
                errx(1,"invalid nc");
        }

        int wc = strtol(argv[2],&endptr,10);
        if(*endptr != '\0' || wc < 1 || wc > 35) {
                errx(1,"invalid wc");
        }

        int pipes = nc + 1;
        int fds[8][2];

        for(int i = 0 ;i <pipes;i++) {
                if(pipe(fds[i]) < 0) {
                        err(1,"pipe error");
                }
        }

        for(int i = 1;i <= nc;i++) {
                pid_t pid = fork();
                if(pid < 0) {
                        err(1,"fork error");
                }
                if(pid == 0) {
                        int from = fds[i-1][0];
                        int to = fds[i][1];

                        for(int j = 0;j < pipes;j++) {
                                if(fds[j][0] != from) {
                                        close(fds[j][0]);
                                }
                                if(fds[j][1] != to){
                                        close(fds[j][1]);
                                }
                        }

                        handleMessage(wc,from,to);

                        close(from);
                        close(to);
                        exit(0);
                }
        }

        int from = fds[nc][0];
        int to = fds[0][1];

        for(int i = 0;i < pipes;i++) {
                if(fds[i][0] != from) {
                        close(fds[i][0]);
                }
                if(fds[i][1] != to) {
                        close(fds[i][1]);
                }
        }

        write(1,L[0],strlen(L[0]));

        int start = 1;
        write(to,&start,sizeof(start));


        handleMessage(wc,from,to);

        close(from);
        close(to);

        for(int i = 1;i <= nc;i++) {
                pid_t pid;
                int status;
                if((pid = wait(&status)) == -1) {
                        err(1,"wait");
                }

                if(WIFEXITED(status)) {
                        if(WEXITSTATUS(status) != 0) {
                                warnx("asd");
                        }
                } else {
                        warnx("asdsd");
                }

        }
}
