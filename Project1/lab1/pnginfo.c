#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "lab_png.h"
#include "crc.h"
int is_png(unsigned char* buf){
    unsigned char sig[] = {137, 80, 78, 71, 13, 10, 26, 10};
    return memcmp (buf, sig, 8) == 0; // compare first 8 bytes, return 1 if match

}
void get_data_IHDR(unsigned int *width, unsigned int *height, FILE*fp){
    fseek(fp, 16, SEEK_SET) // skip to 16 it contains width and height info
    fread(width, 4, 1, fp); // read 4 bytes into width
    fread(height, 4, 1, fp); // 4 bytes from height
    *width = ntohl(*width); // convert to little-endian
    *height = ntohl(*height);

}
int main (int argc, char **argv){
    unsigned char sig[8];
    FILE *fp = fopen(argv[1], "rb");
    fread(sig, 1, 8, fp);
    if (!ispng(sig)) {
        printf("%s: Not a PNG file\n", argv[1]);
        fclose(fp);
        return 1;
    }
    unsigned int width, height;
    get_data_IHDR(&width, &height, fp);
    // cRC check
    fseek (fp, 12, SEEK_SET); // type field
    unsigned char type_and_data[17]; // fixed buffer
    fread(type_and_data, 1, 17, fp); // read 4 type + 13 data bytes for cRC
    unsigned int crc_stored;
    fread(&crc_stored, 4, 1, fp);
    crc_stored = ntohl(crc_stored);
    unsigned int crc_computed = crc(type_and_data, 17);

    if (crc_computed != crc_stored) {
        printf("%s: %u x %u IHDR chunk CRC error: computed %08x, expected %08x\n",
               argv[1], width, height, crc_computed, crc_stored);
        fclose(fp);
        return 1;
    }
    printf("%s: %u x %u\n", argv[1], width, height);
    fclose(fp);
    return 0;
}


#include <stdlib.h>
#include <stdio.h>
#include "lab_png.h"
#include "crc.h"
#include <dirent.h>
#include <sys/stat.h>


int found = 0

void findpng (char *path){
    DIR *dr = opendir(path);
    struct dirent *de;
    if (dr == NULL){
        printf("No dir")
    }

    while ((de = readdir(dr))!=NULL){
        if(strcmp(de->d_name, ".") == 0|| strcmp(de->d_name, ".") == 0){
            continue;
        }


    }

    // build path
    char fullpath[1024];
    snprintf(fullpath, sizeof(fullpath), "%s/%s", path, de->d_name);

    struct stat st;

    stat(fullpath, &st);

    // is it a directiory yes then call findpng and recurse
    if (S_ISDIR(st.st_mode)){
        find_png(fullpath);
    }
    // is it a regular file
    else if (S_ISREG(st.st_mode)){
        FILE* fp = fopen(fullpath, "rb");
        if (fp == NULL){
            continue;
        }
        // read first 8 bytes to check if its a png
        unsigned char buffer[8];
        fread(buffer, 1, 8, fp);
        int sig[8] = {137, 80, 78, 71, 13, 10, 26, 10};
        if (memcmp (buffer, sig, 8) == 0){
            found = 1
            return 0;
        }
    }

    closedir(dr);


}


int main (int argc, char **argv){
    findpng(argv[1]);
    if (!found){
        printf("findpng: No PNG file found\n");
        return 0;
    }


}
