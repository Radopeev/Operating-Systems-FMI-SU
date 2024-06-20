#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <err.h>
#include <stdio.h>
#include <sys/stat.h>


int main(int argc,char* argv[]) {
        if(argc != 3) {
                errx(1,"args error");
        }

        int input = open(argv[1],O_RDONLY);
        if(input < 0) {
                err(1,"error");
        }

        int output = open(argv[2],O_WRONLY | O_CREAT | O_TRUNC,0644);
        if(output < 0) {
                close(input);
                err(1,"read error");
        }

        struct stat st;
        if(fstat(input,&st) < 0) {
                close(input);
                close(output);
                err(1,"fstat error");
        }

        if(st.st_size % sizeof(uint16_t) != 0) {
                close(input);
                close(output);
                errx(1,"invalid file");
        }

        uint32_t arrN = st.st_size / 2;
        if(arrN > 524288) {
                close(input);
                close(output);
                err(1,"invalid number of elements");
        }

        if(write(output,"#include <stdint.h>\n\n",21) < 0) {
                close(input);
                close(output);
                err(1,"write error");
        }

        if(write(output,"uint32_t arrN=",14) < 0) {
                close(input);
                close(output);
                err(1,"write error");
        }

        dprintf(output, "%d;\n", arrN);

        if(write(output,"const uint16_t arr[] = {",24) < 0) {
                close(input);
                close(output);
                err(1,"write error");
        }


        uint16_t unit;
        for(uint16_t i = 0;i < arrN;i++) {
                if(read(input,&unit,sizeof(unit) < 0)) {
                        close(input);
                        close(output);
                        err(1,"read error");
                }
                dprintf(output,"%d,",unit);
        }

        if(read(input,&unit,sizeof(unit) < 0)) {
                close(input);
                close(output);
                err(1,"read_error");
        }

        dprintf(output,"%d};\n",unit);

        close(input);
        close(output);
}
