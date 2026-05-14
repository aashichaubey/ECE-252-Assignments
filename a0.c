#include <stdio.h>
#include <string.h>


// Part 1:
// Write a C program that prints all input arguments to stdout, one per line. It
// should be able to handle any number of input arguments.

int main(int argc, char** argv){
    for (int i = 0; i<argc; i++){
        printf("%s\n", argv[i]);
    }
}

// Part 2:
// Modify your code above so that if any input argument begins with a dash (-), it
// is indented by the number of tabs equal to the number of dashes. For example,
// the argument --mike is indented by two tab characters.

int main(int argc, char** argv){
    int count = 0;
    for (int i = 0; i<argc; i++){
        count = 0;
        for (int j =0; j<strlen(argv[i]); j++){
            if (argv[i][j] != '-'){
                break;
            }
            count++;
        }
        for (int k = 0; k< count; k++){
            printf("    ");
        }
        printf("%s\n", argv[i]);
    }
}
