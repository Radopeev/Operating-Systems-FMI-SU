#include <stdint.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>

typedef struct  {
        uint32_t magic;
        uint32_t count;
} data_header;

typedef struct {
        uint32_t magic1;
        uint16_t magic2;
        uint16_t reserved;
        uint64_t count;
} comparator_header;

typedef struct {
        uint16_t type;
        uint16_t reserved[3];
        uint32_t offset1;
        uint32_t offset2;
} comparator_entry;

static void close_all(int data,int comparator) {
        close(data);
        close(comparator);
}

int main(int argc,char* argv[]) {
        if(argc != 3) {
                errx(1,"arg error");
        }

        int data = open(argv[1],O_RDWR,0644);
        if(data < 0) {
                err(1,"data error");
        }

        int comparator = open(argv[2],O_RDONLY);
        if(comparator < 0) {
                close(data);
                err(1,"comparator");
        }

        data_header d_header;
        ssize_t read_size;
        if((read_size=read(data,&d_header,sizeof(d_header))) != sizeof(d_header)) {
                if(read_size < 0) {
                        close_all(data,comparator);
                        err(1,"read error");
                } else {
                        close_all(data,comparator);
                        errx(1,"invalid argument");
                }
        }

        if(d_header.magic != 0x21796F4A) {
                close_all(data,comparator);
                errx(1,"invalid argument");
        }

        comparator_header c_header;
        if((read_size=read(comparator,&c_header,sizeof(c_header))) != sizeof(c_header)) {
                close_all(data,comparator);
                if(read_size < 0) {
                        err(1,"read error");
                } else {
                        errx(1,"invalid argument");
                }
        }

        if(c_header.magic1 != 0xAFBC7A37 || c_header.magic2 != 0x1C27) {
                errx(1,"invalid magic1");
        }


        for(uint64_t i = 0;i < c_header.count;i++) {
                comparator_entry entry;
                if((read_size=read(comparator,&entry,sizeof(entry))) != sizeof(entry)) {
                        close_all(data,comparator);
                        if(read_size < 0) {
                                err(1,"read error");
                        } else {
                                errx(1,"invalid argument");
                        }
                }
                if(entry.offset1 >= d_header.count || entry.offset2 >= d_header.count) {
                        close_all(data,comparator);
                        errx(1,"invalid argument");
                }

                off_t offset1 = sizeof(d_header) + entry.offset1 * sizeof(uint64_t);
                off_t offset2 = sizeof(d_header) + entry.offset2 * sizeof(uint64_t);

                uint64_t value1,value2;

                if(lseek(data,offset1,SEEK_SET) < 0) {
                        close_all(data,comparator);
                        err(1,"lseek error");
                }

                if(read(data,&value1,sizeof(value1)) != sizeof(value1)) {
                        close_all(data,comparator);
                        err(1,"read error");
                }

                if(lseek(data,offset2,SEEK_SET) < 0) {
                        close_all(data,comparator);
                        err(1,"lseek error");
                }

                if(read(data,&value2,sizeof(value2)) != sizeof(value2)) {
                        close_all(data,comparator);
                        err(1,"read error");
                }

                if((entry.type == 0 && value1 >= value2) || (entry.type == 1 && value1 <= value2)) {
                        if(lseek(data,offset1,SEEK_SET) < 0) {
                                close_all(data,comparator);
                                err(1,"lseek error");
                        }

                        if(write(data,&value2,sizeof(value2)) != sizeof(value2)) {
                                close_all(data,comparator);
                                err(1,"write error");
                        }

                        if(lseek(data,offset2,SEEK_SET) < 0) {
                                close_all(data,comparator);
                                err(1,"lseek error");
                        }

                        if(write(data,&value1,sizeof(value1)) != sizeof(value1)) {
                                close_all(data,comparator);
                                err(1,"write error");
                        }
                }
        }
        close_all(data,comparator);
}
