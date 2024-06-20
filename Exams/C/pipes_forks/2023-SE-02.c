#include <err.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc,char* argv[]) {
        if(argc < 2) {
                errx(1,"invalid args");
                exit(26);
        }

        int pipes[2];
        if(pipe(pipes) < 0) {
                err(1,"pipe error");
                exit(26);
        }

        int children[argc];

        for(int i = 1;i < argc;i++) {
                pid_t child = fork();
                if(child < 0) {
                        exit(26);
                        err(1,"fork error");
                }

                if(child == 0) {
                        close(pipes[0]);

                        if(dup2(pipes[1],1) < 0) {
                                err(1,"dup2 error");
                                exit(26);
                        }

                        dprintf(1,"%s ",argv[i]);
                        execlp(argv[i],argv[i],(char*)NULL);
                } else {
                        children[i]=child;
                }
        }

        close(pipes[1]);

        char buff[512];
        ssize_t n;
        while((n=read(pipes[0],&buff,sizeof(buff) - 1)) > 0) {
                buff[n] = '\0';
                if(strstr(buff,"found it!") != NULL) {
                        for(int i = 1;i< argc;i++) {
                                if(kill(children[i],SIGTERM) == -1) {
                                        err(1,"kill error");
                                }

                                if(waitpid(children[i],NULL,0) == -1) {
                                        err(1,"kill error");
                                }
                        }
                        close(pipes[0]);
                        exit(0);
                }
        }

        close(pipes[0]);

        while(wait(NULL) > 0);

        exit(1);
}
