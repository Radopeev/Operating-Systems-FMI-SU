#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <err.h>

typedef struct pair{
        uint32_t x;
        uint32_t y;
}pair;

int main(int argc,char** argv) {
        if(argc != 4) {
                errx(1,"args");
        }

        int f1 = open(argv[1],O_RDONLY);

        if(f1 == -1) {
                err(1,"file1");
        }

        int f2 = open(argv[2],O_RDONLY);

        if(f2 == -1) {
                close(f1);
                err(2,"file2");
        }

        int f3 = open(argv[3],O_WRONLY | O_CREAT | O_TRUNC,S_IRWXU);

        if(f2 == -1) {
                close(f1);
                close(f2);
                err(3,"file3");
        }

        struct stat st;

        if(fstat(f1,&st) == -1) {
                close(f1);
                close(f2);
                close(f3);
                err(4,"stat");
        }

        if(st.st_size % (2 * sizeof(uint32_t)) != 0) {
                close(f1);
                close(f2);
                close(f3);
                errx(5,"size");
        }

        struct stat st_2;

        if(fstat(f1,&st_2) == -1) {
                close(f1);
                close(f2);
                close(f3);
                err(6,"stat");
        }

        pair p;
        while (read(f1, &p, sizeof(pair)) > 0){
                uint32_t x = p.x;
                uint32_t y = p.y;

                if(x * sizeof(uint32_t) >= st_2.st_size || (x + y) * sizeof(uint32_t) > st_2.st_size) {
                        close(f1);
                        close(f2);
                        close(f3);
                        errx(7,"size");
                }

                if(lseek(f2,x * sizeof(uint32_t),SEEK_SET) == -1) {
                        close(f1);
                        close(f2);
                        close(f3);
                        err(8,"size");
                }

                for(uint32_t i = 0;i < y;i++) {
                        uint32_t el;

                        if(read(f2,&el,sizeof(uint32_t)) != sizeof(uint32_t)) {
                                close(f1);
                                close(f2);
                                close(f3);
                                err(9,"size");
                        }

                        if(write(f3,&el,sizeof(uint32_t)) != sizeof(uint32_t)) {
                                close(f1);
                                close(f2);
                                close(f3);
                                err(10,"size");
                        }
                }
        }

        close(f1);
        close(f2);
        close(f3);
}
