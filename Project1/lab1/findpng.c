#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "lab_png.h"
int found = 0;
void findpng(const char *path);{
    DIR *dr = opendir(path);
    if (dr == NULL){
        return;
    }
    struct dirent *de;
    while ((de = readdir(dr) != NULL)){
        if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) continue;
    }
    char fullpath[1024]; // create fixed buffer to hold full path
    snprintf(fullpath, sizeof(fullpath), "%s/%s", path, de->d_name); // comvine parent path + "/" + filename into full path

    struct stat st;
    stat(fullpath, &st);

    if (S_ISDIR(st.st_mode)) {
        findpng(fullpath);
    } else if (S_ISREG(st.st_mode)){
        FILE *fp = fopen(fullpath, "rb");
        if (fp == NULL){
            continue;
        }
        unsigned char sig[8];
        fread(sig, 1, 8, fp);
        fclose(fp);
        if (is_png(sig)){
            printf("%s\n", fullpath);
            found = 1;
        }
    }
    closedir(dr);
}
int main (int argc, char **argv){
    findpng(argv[1]);
    if (!found){
        printf("findpng: No PNG file was found\n");
    }
    return 0;

}



#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include "lab_png.h"

// dont think unsigned matters that much, add at end
int is_png(char *buf){
    // is png checks if its its it is png
    int sig[] = {137, 80, 78, 71, 13, 10, 26, 10};
    if (memcmp(sig, buf, 8)!=0){
        return 0;
    }
    return 1;

}

void get_data_IHDR(int *width, int *height, FILE* fp){
    fseek(fp, 16, SEEK_SET); // 16 bytes in is the data field
    fread(width, 4, 1, fp); 
    fread(height, 4, 1, fp);
    *width = ntohl(*width); // convert to little endian
    *height = ntohl(*height);
}

int main(int argc, char **argv){

    char sig[8];
    FILE *fp = fopen(argv[1], "rb");
    fread(sig, 1, 8, fp);

    if (!(is_png(sig))){
        printf("%s: Not a PNG file", argv[1]);
        return 1;
    }
    
    int width, height;
    get_data_IHDR(&width, &height, fp);

    fseek(fp, 12, SEEK_SET);
    char buffer[17];
    fread(buffer, 1, 17, fp);
    int crc_stored;
    fread(&crc_stored, 4, 1, fp);
    crc_stored = ntohl(crc_stored);
    crc_computed = crc(buffer, 17)
    if (crc_computed != crc_stored){
        printf("%s %u x %u: IDAT chunk CRC error computed %08x, got %08x ",argv[1], width, height, buffer, crc_stored);
        fclose (fp);
        return 0;
    }

    printf("%s: :", &width, &height);
    fclose (fp);
    return 0;


}


