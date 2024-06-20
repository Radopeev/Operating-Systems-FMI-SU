#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>

const char ding[] = "DING\n";
const char dong[] = "DONG\n";


int main(int argc,char* argv[]) {
        if(argc != 3) {
                errx(1,"invalid args");
        }

        char* endptr;

        int n = strtol(argv[1], &endptr, 10);
        if(*endptr != '\0' || n <= 0 || n >= 10) {
                errx(1,"invalid N");
        }

        int d = strtol(argv[2], &endptr, 10);
        if(*endptr != '\0' || n <= 0 || n>= 10) {
                errx(1,"invalid d");
        }

        int parent_to_child[2];

        if(pipe(parent_to_child) < 0) {
                err(1,"pipe error");
        }

        int child_to_parent[2];

        if(pipe(child_to_parent) < 0) {
                err(1,"pipe error");
        }

        pid_t child = fork();

        if(child < 0) {
                err(1,"fork erro");
        }

        if(child == 0) {
                close(parent_to_child[1]);
                close(child_to_parent[0]);

                int dummy;
                for(int i = 0;i < n;i++) {
                        if(read(parent_to_child[0],&dummy,sizeof(dummy)) < 0) {
                                err(1,"read error");
                        }
                        if(write(1,dong,strlen(dong)) < 0) {
                                err(1,"write error");
                        }
                        if(write(child_to_parent[1],&dummy,sizeof(dummy)) < 0) {
                                err(1,"write error");
                        }
                }

                close(parent_to_child[0]);
                close(child_to_parent[1]);
                exit(0);
        }

        close(parent_to_child[0]);
        close(child_to_parent[1]);

        for(int i = 0;i < n;i++) {
                if(write(1,ding,sizeof(ding)) < 0) {
                        err(1,"write error");
                }
                int dummy;
                if(write(parent_to_child[1],&dummy,sizeof(dummy)) < 0) {
                        err(1,"write error");
                }
                if(read(child_to_parent[0],&dummy,sizeof(dummy)) < 0) {
                        err(1,"read error");
                }
                sleep(d);
        }

        close(parent_to_child[1]);
        close(child_to_parent[0]);

        int status;
        if(wait(&status) < 0) {
                err(11,"wait error");
        }

        if(WIFEXITED(status)) {
                if(WEXITSTATUS(status) != 0) {
                        err(1,"child terminated with status not 0");
                }
        } else {
                err(1,"child not terminated normally");
        }


}

