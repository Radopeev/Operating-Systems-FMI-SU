#include <stdint.h>
#include <fcntl.h>
#include <err.h>
#include <unistd.h>

static uint8_t decode(uint16_t num) {
        uint8_t result = 0;
        for(int i = 7;i >= 0;i--) {
                uint16_t frontMask = (1 << (i * 2 + 1));
                if(num & frontMask) {
                        result = (result << 1) | 0x01;
                }
                result = (result << 1)
        }
        return result;
}

int main(int argc,char* argv[]) {
        if(argc != 3) {
                errx(1,"arg error");
        }

        int input = open(argv[1],O_RDONLY);
        if(input < 0) {
                err(1,"open error");
        }
        int output = open(argv[2],O_WRONLY | O_CREAT | O_TRUNC,0644);
        if(output < 0) {
                close(input);
                err(1,"open error");
        }

        uint16_t unit;
        int read_size;
        while((read_size = read(input,&unit,sizeof(unit))) != sizeof(unit)) {
                uint8_t byte = decode(unit);
                if(write(output,&byte,sizeof(byte)) < 0) {
                        close(input);
                        close(output);
                        err(1,"write error");
                }
        }

        if(read_size < 0) {
                close(input);
                close(output);
                err(1,"read error");
        }

        close(input);
        close(output);
}
