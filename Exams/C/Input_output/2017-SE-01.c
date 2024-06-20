#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>


typedef struct tuple {
        uint16_t offset;
        uint8_t original;
        uint8_t new;
} tuple;

void close_all(int f1,int f2,int patch);
void close_all(int f1,int f2,int patch){
        close(f1);
        close(f2);
        close(patch);
}

int main(int argc, char* argv[]){
        if(argc != 4){
                errx(1,"asdsad");
        }

        int f1 = open(argv[1], O_RDONLY);

        if(f1 < 0) {
                err(1,"asds");
        }

        int f2 = open(argv[2], O_RDONLY);

        if(f2 < 0) {
                close(f1);
                err(1,"asdd");
        }

        int patch = open(argv[3],O_WRONLY | O_CREAT | O_TRUNC,0644);

        if(patch < 0) {
                close(f2);
                err(1,"asdsd");
        }

        uint8_t byte_1,byte_2;

        struct stat st_1,st_2;

        if(fstat(f1,&st_1) < 0) {
                close_all(f1,f2,patch);
                err(1,"asdsad");
        }

        if(fstat(f2,&st_2) < 0) {
                close_all(f1,f2,patch);
                err(1,"asds");
        }

        if(st_1.st_size != st_2.st_size) {
                errx(1,"asdsd");
        }

        int count = 0;

        while(read(f1,&byte_1,sizeof(uint8_t) > 0)) {
                if(read(f2,&byte_2,sizeof(uint8_t)) <=  0) {
                        close_all(f1,f2,patch);
                        err(1,"adsdasd");
                }

                if(byte_1 != byte_2) {
                        tuple t;
                        t.offset = count;
                        t.original = byte_1;
                        t.new = byte_2;
                        if(write(patch,&t,sizeof(t)) < 0) {
                                close_all(f1,f2,patch);
                                err(1,"adsada");
                        }
                }
                count++;
        }

        close_all(f1,f2,patch);
}

