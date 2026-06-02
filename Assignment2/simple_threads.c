#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX_RECORDS 250000

// gcc -pthread simple_threads.c -o simple_threads
// expect 1 command line arugment: N = number of threads to create
// ./simple_threads 4

// diff_array contains array of strings with the diff text of each line 
// total_records = total number of records loaded 

// split array indices ( 0 -> total_records - 1) among N threads
// each thread will iterate thru assigned range in diff_array in read only
// string mataches "NO_DIFF"
// main thread must wait for all child threads to finish using pthread_join()

// time ./simple_threads 1 give syou time

// Lab1: input of ints and count how many odd/or not

// Shared array of diff strings accessible by all threads
char **diff_array;
int total_records = 0;

// TODO: Define a structure to pass arguments to threads (e.g., start index, end index)
typedef struct{
    int start_index;
    int end_index;
} ThreadArgs;
// TODO: Define the thread function that counts "NO_DIFF" in its assigned range
// for loop iterates thru diff array and strcmp to see if the individual element = NO_DIFF
void * count_no_diff(void * argument){
    ThreadArgs* a = (ThreadArgs*) argument;
    int start = a->start_index;
    int end = a->end_index;

    int * return_val = malloc(sizeof(int));
    *return_val = 0;
    for (int i =start; i< end; i++){
        if (strcmp(diff_array[i], "NO_DIFF") == 0) {
            (*return_val) ++; // brackets important to avoid incrementing ptr
        }
    }
    pthread_exit(return_val);
}
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <num_threads>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);
    if (num_threads <= 0) {
        fprintf(stderr, "Number of threads must be > 0\n");
        return 1;
    }

    // Allocate memory for the shared array
    diff_array = malloc(MAX_RECORDS * sizeof(char *));
    if (diff_array == NULL) {
        perror("malloc");
        return 1;
    }

    FILE *file = fopen("massive_data.txt", "r");
    if (file == NULL) {
        perror("fopen");
        return 1;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    printf("Reading data into memory...\n");
    // Read the file line by line
    while ((read = getline(&line, &len, file)) != -1) {
        // Parse the line to get the diff_text
        // Format: session_number user_number filename seq diff_text
        char *token = strtok(line, " "); // session
        token = strtok(NULL, " ");       // user
        token = strtok(NULL, " ");       // filename
        token = strtok(NULL, " ");       // seq
        
        // The rest of the line is the diff text
        char *diff_text = strtok(NULL, "\n"); 

        if (diff_text != NULL) {
            // Remove leading spaces if any
            while (*diff_text == ' ') diff_text++;
            diff_array[total_records] = strdup(diff_text);
            total_records++;
            if (total_records >= MAX_RECORDS) {
                break; // Prevent overflow
            }
        }
    }
    fclose(file);
    if (line) free(line);
    printf("Successfully read %d records.\n", total_records);

    int main( int argc, char** argv) {
    if (argc !=2){
        printf("Invalid args.\n");
        return -1;
    }
    pthread_t t; // create thread
    void* vr; // void ptr that will hold return value

    //            addr, attributes, pass in function ptr, arg passed to function
    pthread_create(&t, NULL, run, argv[1]); // create thread 
    pthread_join(t, &vr); // pause here and wait for finish, we are writing into vr
    int *r = (int*) vr; // vr is a void pointer so we have to get r
    printf("The other thread returned %d.\n", *r);
    free(vr);
    pthread_exit(0);
}

    // TODO: Create threads, assign them ranges, and count "NO_DIFF" entries
    // actual create p_thread loop num_thread times and for each thread
    // split the array into n threads with start and end index determined by total/records//n
    pthread_t threads[num_threads]; // array of size #of threads hadnles
    ThreadArgs args[num_threads]; // array of #of threads ThreadArgs structs
    int len_args = total_records/num_threads;
    
    for (int i = 0; i<num_threads; i++){
        args[i].start_index = i* len_args;
        args[i].end_index = args[i].start_index + len_args;
        pthread_create(&threads[i], NULL, count_no_diff, &args[i]);
    }



    // TODO: Wait for threads to finish, collect results, and print the total count
    // uses join function to wait for finish get return value assign return type and print total count, free void ptrs, and exit
    thread_join(t, &vr);
    int *r = int
    // Clean up
    for (int i = 0; i < total_records; i++) {
        free(diff_array[i]);
    }
    free(diff_array);

    return 0;
}



#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

// define as function of type void pointer and input argument void pointer
void * run ( void * argument ){
    char* a = (char*) argument; // define what it is, specfically char
    printf("Provided argument is %s!\n", a); 
    int * return_val = malloc(sizeof(int)); // return int assign memory
    // malloc ensures that return val survives after thread exits bc local dies
    *return_val = 99; // assign appropriate value 
    pthread_exit( return_val ); // cleanup
}
