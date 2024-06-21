#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <err.h>
#include <stdio.h>
#include <unistd.h>

static void close_all(int affix,int prefix,int postfix,int infix,int suffix,int crucifix) {
        close(affix);
        close(prefix);
        close(postfix);
        close(infix);
        close(suffix);
        close(crucifix);
}

typedef struct header {
        char bytes[4];
        uint16_t count;
        char bytes_[10];
}header;

typedef struct interval {
        int offset;
        int count;
} interval;

typedef struct complect {
        interval post;
        interval pre;
        interval in;
        interval suf;
} complect;

int main(int argc,char* argv[]) {
        if(argc != 7){
                errx(1,"args error");
        }

        int affix = open(argv[1],O_RDONLY);
        if(affix < 0) {
                err(1,"affix open error");
        }

        int prefix = open(argv[2],O_RDONLY);
        if(prefix < 0){
                close(affix);
                err(1,"prefix open fail");
        }

        int postfix = open(argv[3],O_RDONLY);
        if(postfix < 0) {
                close(affix);
                close(prefix);
                err(1,"postfix open fail");
        }

        int infix = open(argv[4],O_RDONLY);
        if(infix < 0) {
                close(affix);
                close(prefix);
                close(postfix);
                err(1,"infix open fail");
        }

        int suffix = open(argv[5],O_RDONLY);
        if(suffix < 0) {
                close(affix);
                close(prefix);
                close(postfix);
                close(infix);
                err(1,"suffix open fail");
        }

        int crucifixus = open(argv[6],O_WRONLY | O_CREAT | O_TRUNC,0644);
        if(crucifixus < 0) {
                close(affix);
                close(prefix);
                close(postfix);
                close(infix);
                close(suffix);
                err(1,"crucifixus");
        }

        header h;

        if(read(affix,&h,sizeof(header)) != sizeof(header)) {
                close_all(affix,prefix,postfix,infix,suffix,crucifixus);
                err(1,"header read error");
        }
        int affix_count = h.count;
        complect* affix_arr = (complect*)malloc(affix_count * sizeof(complect));

        if(!affix_arr) {
                close_all(affix,prefix,postfix,infix,suffix,crucifixus);
                err(1,"memory error");
        }

        for(int i = 0;i < affix_count;i++) {
                if(read(affix,&affix_arr[i],sizeof(complect)) != sizeof(complect)) {
                        free(affix_arr);
                        close_all(affix,prefix,postfix,infix,suffix,crucifixus);
                        err(1,"read error");
                }
        }

        if(read(postfix,&h,sizeof(header)) != sizeof(header)) {
                free(affix_arr);
                close_all(affix,prefix,postfix,infix,suffix,crucifixus);
                err(1,"header read error");
        }
        int postfix_count = h.count;
        uint32_t* postfix_arr = (uint32_t*)malloc(postfix_count * sizeof(uint32_t));
        if(!postfix_arr) {
                free(affix_arr);
                close_all(affix, prefix, postfix, infix, suffix, crucifixus);
                err(1,"memory fail");
        }

        for(int i = 0;i < postfix_count;i++) {
                if(read(postfix,&postfix_arr[i],sizeof(uint32_t)) != sizeof(uint32_t)) {
                        free(affix_arr);
                        free(postfix_arr);
                        close_all(affix,prefix,postfix,infix,suffix,crucifixus);
                        err(1,"read error");
                }
        }

        if(read(prefix,&h,sizeof(header)) != sizeof(header)) {
                free(affix_arr);
                free(postfix_arr);
                close_all(affix,prefix,postfix,infix,suffix,crucifixus);
                err(1,"header read error");
        }
        int prefix_count = h.count;
        uint8_t* prefix_arr = (uint8_t*)malloc(prefix_count * sizeof(uint8_t));
        if(!prefix_arr) {
                free(affix_arr);
                free(postfix_arr);
                close_all(affix,prefix,postfix,infix,suffix,crucifixus);
                err(1,"memory error");
        }

        for(int i = 0;i < prefix_count;i++) {
                if(read(prefix,&prefix_arr[i],sizeof(uint8_t)) != sizeof(uint8_t)) {
                        free(affix_arr);
                        free(postfix_arr);
                        free(prefix_arr);
                        close_all(affix,prefix,postfix,infix,suffix,crucifixus);
                        err(1,"read error");
                }
        }

        if(read(infix,&h,sizeof(header)) != sizeof(header)) {
                free(affix_arr);
                free(postfix_arr);
                free(prefix_arr);
                close_all(affix,prefix,postfix,infix,suffix,crucifixus);
                err(1,"header read error");
        }
        int infix_count = h.count;
        uint16_t* infix_arr = (uint16_t*)malloc(infix_count * sizeof(uint16_t));
        if(!infix_arr) {
                free(affix_arr);
                free(postfix_arr);
                free(prefix_arr);
                close_all(affix,prefix,postfix,infix,suffix,crucifixus);
                err(1,"memory error");
        }

        for(int i = 0;i < infix_count;i++) {
                if(read(infix,&infix_arr[i],sizeof(uint16_t)) != sizeof(uint16_t)) {
                        free(affix_arr);
                        free(postfix_arr);
                        free(prefix_arr);
                        free(infix_arr);
                        close_all(affix,prefix,postfix,infix,suffix,crucifixus);
                        err(1,"read error");
                }
        }

        if(read(suffix,&h,sizeof(header)) != sizeof(header)) {
                free(affix_arr);
                free(postfix_arr);
                free(prefix_arr);
                free(infix_arr);
                close_all(affix,prefix,postfix,infix,suffix,crucifixus);
                err(1,"header read error");
        }
        int suffix_count = h.count;
        uint64_t* suffix_arr = (uint64_t*)malloc(suffix_count * sizeof(uint64_t));
        if(!suffix_arr) {
                free(affix_arr);
                free(postfix_arr);
                free(prefix_arr);
                close_all(affix,prefix,postfix,infix,suffix,crucifixus);
                err(1,"memory error");
        }

        for(int i = 0;i < suffix_count;i++) {
                if(read(suffix,&suffix_arr[i],sizeof(uint64_t)) < 0) {
                        free(affix_arr);
                        free(postfix_arr);
                        free(prefix_arr);
                        free(infix_arr);
                        free(suffix_arr);
                        close_all(affix,prefix,postfix,infix,suffix,crucifixus);
                        err(1,"read error");
                }
        }

        for(int i = 0;i < affix_count;i++) {
                interval post = affix_arr[i].post;
                interval pre = affix_arr[i].pre;
                interval in = affix_arr[i].in;
                interval suf = affix_arr[i].suf;

                for(int j = post.offset;j <post.offset + post.count;j++){
                        if(j >= postfix_count) {
                                errx(1,"out of range");
                        }
                        if(write(crucifixus,&postfix_arr[j],sizeof(uint32_t)) != sizeof(uint32_t)) {
                                free(affix_arr);
                                free(postfix_arr);
                                free(prefix_arr);
                                free(infix_arr);
                                free(suffix_arr);
                                close_all(affix,prefix,postfix,infix,suffix,crucifixus);
                                err(1,"write error");
                        }
                }

                for(int j = pre.offset;j <pre.offset + pre.count;j++){
                        if(j >= prefix_count) {
                                errx(1,"out of range");
                        }
                        if(write(crucifixus,&prefix_arr[j],sizeof(uint8_t)) != sizeof(uint8_t)) {
                                free(affix_arr);
                                free(postfix_arr);
                                free(prefix_arr);
                                free(infix_arr);
                                free(suffix_arr);
                                close_all(affix,prefix,postfix,infix,suffix,crucifixus);
                                err(1,"write error");
                        }
                }

                for(int j = in.offset;j < in.offset + in.count;j++){
                        if(j >= infix_count) {
                                errx(1,"out of range");
                        }
                        if(write(crucifixus,&infix_arr[j],sizeof(uint16_t)) != sizeof(uint16_t)) {
                                free(affix_arr);
                                free(postfix_arr);
                                free(prefix_arr);
                                free(infix_arr);
                                free(suffix_arr);
                                close_all(affix,prefix,postfix,infix,suffix,crucifixus);
                                err(1,"write error");
                        }
                }

                for(int j = suf.offset;j < suf.offset + suf.count;j++){
                        if(j >= suffix_count) {
                                errx(1,"out of range");
                        }
                        if(write(crucifixus,&suffix_arr[j],sizeof(uint64_t)) != sizeof(uint64_t)) {
                                free(affix_arr);
                                free(postfix_arr);
                                free(prefix_arr);
                                free(infix_arr);
                                free(suffix_arr);
                                close_all(affix,prefix,postfix,infix,suffix,crucifixus);
                                err(1,"write error");
                        }
                }
        }

        close_all(affix,prefix,postfix,infix,suffix,crucifixus);
        free(affix_arr);
        free(postfix_arr);
        free(prefix_arr);
        free(infix_arr);
        free(suffix_arr);
}
