#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <err.h>


int main(void) {
        int a[2];

        if(pipe(a) == -1) {
                err(1,"pipe failed");
        }

        pid_t pid;
        if ((pid = fork()) == -1) {
                err(2,"first fork error");
        }

        if(pid == 0){
                close(a[0]);
                if(dup2(a[1],1) == -1) {
                        err(1,"first dup2 error");
                }

                if(execlp("cut","cut","-d:","-f7","/etc/passwd",(char*)NULL) == -1) {
                        err(1,"first exec error");
                }
        } else {
                close(a[1]);
        }

        int b[2];
        if(pipe(b) == -1) {
                err(4,"second pipe failed");
        }

        if((pid = fork()) == -1) {
                err(1,"second fork error");
        }

        if(pid == 0) {
                close(b[0]);
                if(dup2(a[0],0) == -1) {
                        err(1,"second dup2 failed");
                }
                if(dup2(b[1],1) == -1) {
                        err(1,"second dup2 failed");
                }

                execlp("sort","sort",(char*)NULL);
        } else {
                close(b[1]);
        }

        int c[2];

        if(pipe(c) == -1) {
                err(1,"third pipe error");
        }

        if((pid = fork()) == -1) {
                err(1,"third fork error");
        }

        if(pid == 0) {
                close(a[0]);

                if(dup2(b[0],0) == -1) {
                        err(1,"third dup2 failed");
                }

                if(dup2(c[1],1) == -1) {
                        err(1,"third dup2 failed");
                }

                if(execlp("uniq","uniq","-c",(char*)NULL) == -1) {
                        err(1,"error uniq");
                }
        } else {
                close(c[1]);
        }

        close(a[0]);
        close(b[0]);

        while(wait(NULL) > 0);

        if(dup2(c[0],0) == -1) {
                err(1,"parent dup 2 error");
        }

        if(execlp("sort","sort","-n",(char*)NULL) == -1) {
                err(1,"error exec in parrent");
        }
        return 0;
}
