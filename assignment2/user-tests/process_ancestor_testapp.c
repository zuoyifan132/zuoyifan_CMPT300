#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include <linux/sched.h>
#include <errno.h>

#define PROCESS_ANCESTOR_TEST 550 // for a 64 bit system
#define ANCESTOR_NAME_LEN 16 

struct process_info {
	long pid; 						/* Process ID */
	char name[ANCESTOR_NAME_LEN]; 	/* Program name of process */
	long state;						/* Current process state */
	long uid;						/* User ID of process owner */
	long nvcsw;						/* # voluntary context switches */
	long nivcsw;					/* # involuntary context switches */
	long num_children; 				/* # children process has */
	long num_siblings;				/* # sibling process has */
};

// copy from assignment 1 sample test
// Macro for custom testing; does exit(1) on failure.
#define CHECK(condition) do{ \
    if (!(condition)) { \
        printf("ERROR: %s (@%d): failed condition \"%s\"\n", __func__, __LINE__, #condition); \
        exit(1);\
    }\
} while(0)

// Print the process information in info_array in table format
static void print_process(long num_filled, struct process_info* info_array){
	printf("Idx#    PID    Name                  State    UID    #VCSW    #IVCSW    #Child    #Sib\n");
	for (int i = num_filled-1; i >= 0; i--){
		printf("%4d %6ld    %-16s%11ld%7ld%9ld%10ld%10ld%8ld\n", i, info_array[i].pid, info_array[i].name, info_array[i].state, info_array[i].uid, info_array[i].nvcsw, info_array[i].nivcsw, info_array[i].num_children, info_array[i].num_siblings);
	}
}

// Test function, will be called in main
static void mytest(){
	// Test case 1: size <= 0
	long size_1 = 0;
	long num_filled_1;
	struct process_info info_array_1[6];
	int result_1 = syscall(PROCESS_ANCESTOR_TEST, &info_array_1, size_1, &num_filled_1);
	CHECK(result_1 == -1);
	int errorCode_1 = errno;
	printf("Test case 1: size <= 0: (err #%d) PASS\n\n", errorCode_1);

	// Test case 2: invalid pointer
	long size_2 = 6;
	long num_filled_2;
	struct process_info info_array_2[6];
	int result_2 = syscall(PROCESS_ANCESTOR_TEST, &info_array_2, size_2, NULL);
	CHECK(result_2 == -1);
	int result_3 = syscall(PROCESS_ANCESTOR_TEST, NULL, size_2, &num_filled_2);
	CHECK(result_3 == -1);
	int errorCode_2 = errno;
	printf("Test case 2: invalid pointer: (err #%d) PASS\n\n", errorCode_2);

	// Test case 3: array size is big
	printf("Test case 3: array size is big(size = 100):\n\n");
	long size_3 = 100;
	long num_filled_3;
	struct process_info info_array_3[100];
	int result_4 = syscall(PROCESS_ANCESTOR_TEST, &info_array_3, size_3, &num_filled_3);
	CHECK(result_4 == 0);
	print_process(num_filled_3, info_array_3);
	printf("\n");

	// Test case 4: array size smaller than number of processes
	printf("Test case 4: array size smaller than number of processes(size = 2):\n\n");
	long size_4 = 2;
	long num_filled_4;
	struct process_info info_array_4[2];
	int result_5 = syscall(PROCESS_ANCESTOR_TEST, &info_array_4, size_4, &num_filled_4);
	CHECK(result_5 == 0);
	print_process(num_filled_4, info_array_4);
	printf("\n");
	
	return;
}

int main(int argc, char *argv[]){
	mytest();

    printf("********************************\n");
    printf("             PASSED!            \n");
    printf("********************************\n");

    return 0;
}