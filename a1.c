#include <stdio.h>
#include <dirent.h>
#include <string.h> 
#include <stdbool.h>

/*
Default behaviour
Invoking ./my_ls exactly as written will list all directories and files in the current
working directory with the exception of any directory of file that begins with a
dot (the exact character .), such as .hidden

Specifying -a (all) and -A (almost all)
Invoking ./my_ls -a will list all directories and files in the current working
directory, including any that begin with a dot.
Invoking ./my_ls -A will list all directories and files in the current working
directory, including all those that begin with a dot except . itself and .. (the
current and parent directories, respectively)

It is possible to invoke both in any order. In this situation, -a takes precedence
and all directories and files are listed.

Specifying the directory to list
Whenever the last command line argument is not -a or -A, it will be a valid Linux
directory name, supplied without spaces. Therefore, the following commands
are all valid:
./my_ls -a directory/path/structure
./my_ls directory/path/structure
And so forth. If the -a and/or -A flags are present, they are always assumed to
be supplied between the invocation and the directory path. The behaviour of
these commands works as above, except the expected behaviour when a directory
is supplied is for your command to list the contents of that directory instead of
the current working directory unless the directory supplied is . itself.
2
*/

// opendir (".") open current dir
// readdir(...) call repeatedly in loop each call gives you a struct direct*
// closedir(...) when done
// directory is always the last argument so argv[argc-1]
int main (int argc, char** argv){
    DIR *dr;
    struct dirent *de;

    bool has_a = false;
    bool has_A = false;

    for(int i = 1; i<argc; i++){
        if(strcmp(argv[i], "-a")==0){
            has_a = true;
        }
        if (strcmp(argv[i],"-A") ==0){
            has_A = true;
        }
    }

    
    if (argc == 1 || strcmp(argv[argc-1], "-a") ==0 || strcmp(argv[argc-1], "-A") ==0){
        dr = opendir(".");
    }
    else{
        dr = opendir(argv[argc-1]);
    }

    if (dr == NULL){
        perror("opendir");
        return 1;
    }


    if (has_a){
            while ((de = readdir(dr))!= NULL){
                printf("%s\n", de->d_name);
            }
    }

    else if (has_A){
        // comparing whole string "."
            while ((de = readdir(dr))!= NULL){
                if (strcmp(de->d_name, ".")==0|| strcmp(de->d_name, "..") ==0){
                    continue;
                }
                printf("%s\n", de->d_name);
            }
    }

    else {
            while ((de = readdir(dr))!= NULL){
                if (de->d_name[0] != '.'){
                    printf("%s\n", de->d_name);
                }
                
            }
    }
    closedir(dr);
    
}