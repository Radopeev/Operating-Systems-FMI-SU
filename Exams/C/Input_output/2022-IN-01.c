#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <err.h>

static void close_all(int list, int data, int out) {
        close(list);
        close(data);
        close(out);
}

typedef struct header {
        uint16_t magic;
        uint16_t filetype;
        uint32_t count;
}header;

int main(int argc,char* argv[]) {
        if(argc != 4) {
                errx(1,"argc error");
        }

        int list = open(argv[1],O_RDONLY);
        if(list < 0) {
                err(1,"list error");
        }

        int data = open(argv[2],O_RDONLY);
        if(data < 0) {
                close(list);
                err(1,"data error");
        }

        int out = open(argv[3],O_WRONLY | O_CREAT | O_TRUNC,0644);
        if(out < 0) {
                close(list);
                close(data);
                err(1,"out error");
        }

        header h1;
        if(read(list,&h1,sizeof(header)) < 0) {
                close_all(list,data,out);
                err(1,"h1 error");
        }

        if(h1.magic != 0x5A4D || h1.filetype != 1) {
                close_all(list,data,out);
                errx(1,"h1 magic error");
        }

        header h2;
        if(read(data,&h2,sizeof(header)) < 0) {
                close_all(list,data,out);
                err(1,"h2 error");
        }

        if(h2.magic != 0x5A4D || h2.filetype != 2) {
                close_all(list,data,out);
                errx(1,"h2 magic error");
        }

        header h3;
        h3.count = 0;
        h3.filetype = 3;
        h3.magic = 0x5A4D;
        if(write(out,&h3,sizeof(header)) < 0) {
                close_all(list,data,out);
                err(1,"h3 write error");
        }

        uint16_t list_unit;
        uint32_t data_unit;
        uint64_t out_unit;
        int read_size;
        int write_size;
        for(uint32_t i = 0;i < h1.count;i++) {
                if(lseek(list,i * sizeof(uint16_t),SEEK_SET) < 0) {
                        close_all(list,data,out);
                        err(1,"lseek error");
                }
                if((read_size = read(list,&list_unit,sizeof(uint16_t))) != sizeof(uint16_t)) {
                        if(read_size < 0) {
                                close_all(list,data,out);
                                err(1,"list error");
                        } else {
                                close_all(list,data,out);
                                errx(1,"invalid argument");
                        }
                }
                if(lseek(data,i * sizeof(data_unit),SEEK_SET) < 0) {
                        close_all(list,data,out);
                        err(1,"lseek error");
                }
                if((read_size = read(data,&data_unit,sizeof(uint32_t))) != sizeof(uint32_t)) {
                        if(read_size < 0) {
                                close_all(list,data,out);
                                err(1,"data error");
                        } else {
                                close_all(list,data,out);
                                errx(1,"invalid argument");
                        }
                }

                if(lseek(out,list_unit * sizeof(uint64_t),SEEK_SET) < 0) {
                        close_all(list,data,out);
                        err(1,"lseek error");
                }
                out_unit = data_unit;
                if((write_size=write(out,&out_unit,sizeof(uint64_t))) != sizeof(uint64_t)) {
                        if(write_size < 0) {
                                close_all(list,data,out);
                                err(1,"list error");
                        } else {
                                close_all(list,data,out);
                                errx(1,"invalid argument");
                        }
                }
        }

        close_all(list,data,out);
}
