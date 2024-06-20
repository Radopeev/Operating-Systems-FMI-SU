#include <err.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>

static void close_all(int patch,int f1,int f2) {
        close(patch);
        close(f1);
        close(f2);
}

typedef struct header {
        uint32_t magic;
        uint8_t header_version;
        uint8_t data_version;
        uint16_t count;
        uint32_t reserved1;
        uint32_t reserved2;
} header;

typedef struct data_version_0 {
        uint16_t offset;
        uint8_t original;
        uint8_t new;
} data_version_0;

typedef struct data_version_1 {
        uint32_t offset;
        uint16_t original;
        uint16_t new;
} data_version_1;

int main(int argc,char* argv[]) {
        if(argc != 4) {
                errx(1,"args not exact");
        }

        int patch = open(argv[1],O_RDONLY);
        if(patch < 0) {
                err(1,"patch not opened");
        }

        int f1 = open(argv[2],O_RDONLY);
        if(f1 < 0) {
                close(patch);
                err(1,"f1 not opened");
        }

        int f2 = open(argv[2],O_WRONLY | O_CREAT | O_TRUNC,0644);
        if(f2 < 0) {
                close(patch);
                close(f1);
                err(1,"f2 not opened");
        }


        header h;
        if(h.magic != 0xDEADBEEF) {
                errx(1,"magic error");
        }

        if(read(patch,&h,sizeof(header)) != sizeof(header)) {
                close_all(patch,f1,f2);
                err(1,"read error");
        }

        if(h.header_version != 0x01) {
                close_all(patch,f1,f2);
                errx(1,"invalid version");
        }

        uint8_t byte;
        ssize_t read_size;
        while((read_size=read(f1,&byte,sizeof(uint8_t))) > 0) {
                if(write(f2,&byte,sizeof(uint8_t)) < 0) {
                        close_all(patch,f1,f2);
                        err(1,"write error");
                }
        }

        if(read_size < 0) {
                close_all(patch,f1,f2);
                err(1,"read error");
        }

        struct stat st;
        if(fstat(f1,&st) < 0) {
                close_all(patch,f1,f2);
                err(1,"fstat error");
        }

        if(h.data_version == 0x00) {
                data_version_0 dv0;
                for(int i = 0;i < h.count;i++) {
                        if(read(patch,&dv0,sizeof(data_version_0) != sizeof(data_version_0))) {
                                close_all(patch,f1,f2);
                                err(1,"read error");
                        }

                        if(dv0.offset >= st.st_size) {
                                close_all(patch,f1,f2);
                                errx(1,"wrong data");
                        }

                        if(lseek(f1,dv0.offset,SEEK_SET) < 0) {
                                close_all(patch,f1,f2);
                                err(1,"lseek error");
                        }

                        if(read(f1,&byte,sizeof(uint8_t)) != sizeof(uint8_t)) {
                                close_all(patch,f1,f2);
                                err(1,"read error");
                        }

                        if(byte != dv0.original) {
                                close_all(patch,f1,f2);
                                errx(1,"wrong data");
                        }

                        if(lseek(f2,dv0.offset,SEEK_SET) < 0) {
                                close_all(patch,f1,f2);
                                err(1,"lseek error");
                        }

                        if(write(f2,&dv0.new,sizeof(uint8_t)) != sizeof(uint8_t)) {
                                close_all(patch,f1,f2);
                                err(1,"write error");
                        }
                }
        } else if(h.data_version == 0x01) {
                data_version_1 dv1;
                for(int i = 0;i < h.count;i++){
                        if(read(patch,&dv1,sizeof(data_version_1)) != sizeof(data_version_1)) {
                                close_all(patch,f1,f2);
                                err(1,"read error");
                        }
                        if(dv1.offset * sizeof(int16_t) > st.st_size) {
                                close_all(patch,f1,f2);
                                errx(1,"wrong data");
                        }

                        if(lseek(f1,dv1.offset,SEEK_SET) < 0) {
                                close_all(patch,f1,f2);
                                err(1,"lseek error");
                        }

                        uint16_t word;
                        if(read(f1,&word,sizeof(uint16_t)) != sizeof(uint16_t)) {
                                close_all(patch,f1,f2);
                                err(1,"read error");
                        }

                        if(word != dv1.original) {
                                close_all(patch,f1,f2);
                                errx(1,"wrong data");
                        }

                        if(lseek(f2,dv1.offset,SEEK_SET) < 0) {
                                close_all(patch,f1,f2);
                                err(1,"lseek error");
                        }

                        if(write(f2,&dv1.new,sizeof(uint16_t)) != sizeof(uint16_t)) {
                                close_all(patch,f1,f2);
                                err(1,"write error");
                        }
                }
                if(read_size < 0) {
                        close_all(patch,f1,f2);
                        err(1,"read error");
                }
        } else {
                close_all(patch,f1,f2);
                errx(1,"wrong data");
        }

        close_all(patch,f1,f2);
        return 0;
}
