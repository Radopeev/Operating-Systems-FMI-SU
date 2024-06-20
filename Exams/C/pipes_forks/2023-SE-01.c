#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc,char* argv[]) {
        if(argc != 2) {
                errx(1,"invalid args");
        }

        int fds[2];
        if(pipe(fds) < 0) {
                err(1,"pipe error");
        }

        pid_t child = fork();

        if(child < 0) {
                err(1,"fork error");
        }

        if (child == 0) {
                close(fds[0]);

                if(dup2(fds[1],1) < 0) {
                        err(1,"dup 2 error");
                }
                if(execlp("find","find",argv[1],"-type","f",(char*)NULL) < 0) {
                        err(1,"exec error");
                }
        }

        close(fds[1]);

        char file_name[4096];
        char c;
        int i = 0;
        while(read(fds[0],&c,sizeof(c)) > 0) {
                if(c == '\n'){
                        file_name[i]='\0';
                        i=0;
                        if(!strstr(file_name,".hash")) {
                                int pipes[2];
                                pipe(pipes);
                                pid_t id = fork();
                                if(id < 0) {
                                        err(1,"fork error");
                                }
                                if(id == 0) {
                                        close(pipes[0]);
                                        dup2(pipes[1],1);
                                        if(execlp("md5sum","md5sum",file_name,(char*)NULL) < 0) {
                                                err(1,"error");
                                        }
                                }
                                close(pipes[1]);
                                strcat(file_name,".hash");
                                int newFile = open(file_name,O_WRONLY | O_TRUNC |O_CREAT,0644);
                                char hash[33];
                                if(read(pipes[0],hash,32) < 0) {
                                        err(1,"read error");
                                }
                                hash[32]='\0';
                                dprintf(newFile,"%s",hash);
                                close(pipes[0]);
                        }
                } else {
                        file_name[i++]=c;
                }
        }
        close(fds[0]);

        while(wait(NULL) > 0);
}
