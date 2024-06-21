#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/stat.h>
#include <err.h>
#include <string.h>


int main(int argc,char* argv[]) {
        if(argc != 4) {
                errx(1,"args error");
        }

        char* word = argv[1];
        int dic = open(argv[2],O_RDONLY);
        if(dic < 0) {
                err(1,"dic error");
        }

        int index = open(argv[3],O_RDONLY);
        if(index < 0) {
                close(dic);
                err(1,"index open error");
        }

        struct stat st;
        if(fstat(index,&st) < 0) {
                close(dic);
                close(index);
                err(1,"fstat error");
        }

        uint32_t start = 0, end = st.st_size / sizeof(uint32_t) - 1;

        while(start <= end) {
                uint32_t mid = start + (end - start) / 2;
                uint32_t mid_pos;

                if(lseek(index,mid * sizeof(uint32_t),SEEK_SET) < 0) {
                        close(dic);
                        close(index);
                        err(1,"lseek error");
                }



                if(read(index,&mid_pos,sizeof(uint32_t)) != sizeof(uint32_t)) {
                        close(dic);
                        close(index);
                        err(1,"read error");
                }

                if(lseek(dic,mid_pos,SEEK_SET) < 0) {
                        close(dic);
                        close(index);
                        err(1,"lseek error");
                }

                char wordFromDic[64];
                char c;
                int i = 0;
                ssize_t read_size;

                if(read(dic,&c,sizeof(c)) != sizeof(c) || c == '\0') {
                        close(dic);
                        close(index);
                        err(1,"read error");
                }

                while((read_size = read(dic,&c,sizeof(c))) > 0 && c != '\0') {
                        wordFromDic[i++]=c;
                }
                if(read_size < 0) {
                        close(dic);
                        close(index);
                        err(1,"read error");
                }
                wordFromDic[i]='\0';
                if(strcmp(wordFromDic,word) < 0) {
                        start = mid +1;
                } else if(strcmp(wordFromDic,word) > 0) {
                        end = mid - 1;
                } else {
                        while((read_size = read(dic,&c,sizeof(c)) > 0) && c != '\0') {
                                if(write(1,&c,sizeof(c)) < 0) {
                                        close(dic);
                                        close(index);
                                        err(1,"write error");
                                }
                        }
                        if(read_size < 0) {
                                close(dic);
                                close(index);
                                err(1,"read error");
                        }
                        return 0;
                }
        }
        close(dic);
        close(index);
        errx(1,"invalid word");
}
