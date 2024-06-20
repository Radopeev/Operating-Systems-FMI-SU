#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <sys/wait.h>


int main(int argc,char* argv[]) {
        if(argc != 2) {
                errx(1,"args error");
        }

        const char* file_name = argv[1];
        int a[2];

        if(pipe(a) == -1) {
                err(1,"pipe failed");
        }

        const pid_t child1 = fork();
        if(child1 == -1) {
                err(3,"fork failed");
        }

        if(child1 == 0) {
                close(a[0]);
                if(dup2(a[1],1) == -1) {
                        close(a[1]);
                        err(1,"dup error");
                }

                if(execlp("cat","cat",file_name,(char*)NULL) == -1) {
                        err(1,"exec error");
                }
        }

        close(a[1]);
        int status;

        if(wait(&status) == -1) {
                close(a[0]);
                err(1,"wait");
        }

        if(!WIFEXITED(status)) {
                close(a[0]);
                err(1,"error");
        }

        if(WEXITSTATUS(status) != 0 ) {
                close(a[0]);
                err(1,"error");
        }

        if(dup2(a[0],0) == -1) {
                err(1,"dup error");
        }

        if(execlp("sort","sort",(char*)NULL) == -1) {
                err(1,"exec error");
        }


}
