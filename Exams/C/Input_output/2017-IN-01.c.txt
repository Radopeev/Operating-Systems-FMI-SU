#include <stdint.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/stat.h>


void close_all(int f1_dat,int f1_idx,int f2_dat,int f2_idx);
void close_all(int f1_dat,int f1_idx,int f2_dat,int f2_idx) {
        close(f1_dat);
        close(f1_idx);
        close(f2_dat);
        close(f2_idx);
}

typedef struct tuple {
        uint16_t offset;
        uint8_t length;
        uint8_t reserved;
} tuple;

int main(int argc, char* argv[]){
        if(argc != 5) {
                errx(1,"adasdsad");
        }

        int f1_dat = open(argv[1],O_RDONLY);
        int f1_idx = open(argv[2],O_RDONLY);
        int f2_dat = open(argv[3],O_WRONLY | O_CREAT | O_TRUNC,0644);
        int f2_idx = open(argv[4],O_WRONLY | O_CREAT | O_TRUNC,0644);

        if(f1_dat < 0){
                err(1,"aadsa");
        }
        if(f1_idx < 0){
                close(f1_dat);
                err(1,"adssd");
        }
        if(f2_dat < 0){
                close(f1_dat);
                close(f1_idx);
                err(1,"adsda");
        }
        if(f2_idx < 0){
                close(f1_dat);
                close(f1_idx);
                close(f2_dat);
                err(1,"adsd");
        }

        char buff[512];
        tuple t;

        struct stat st;
        if(fstat(f1_dat,&st) < 0) {
                close_all(f1_dat,f1_idx,f2_dat,f2_idx);
                err(1,"sadsad");
        }

        while(read(f1_idx,&t,sizeof(t)) > 0) {
                if(t.length + t.offset > st.st_size) {
                        errx(1,"adsads");
                }

                if(read(f1_dat,&buff,t.length) > 0) {
                        if(buff[0] <= 'A' && buff[0] <= 'Z') {

                                if(write(f2_dat,&buff,t.length) < 0){
                                        close_all(f1_dat,f1_idx,f2_dat,f2_idx);
                                }
                                if(write(f2_idx,&t,sizeof(t) < 0)) {
                                        close_all(f1_dat,f1_idx,f2_dat,f2_idx);
                                }
                        }
                } else {
                        close_all(f1_dat,f1_idx,f2_dat,f2_idx);
                }
        }

        close_all(f1_dat,f1_idx,f2_dat,f2_idx);
}
