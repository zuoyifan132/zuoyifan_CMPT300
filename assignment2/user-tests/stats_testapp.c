#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include <errno.h>

#define _STATS_TEST_ 549 // for a 64 bit system

struct array_stats_s { 
	long min;
	long max;
	long sum; 
};

// copy from assignment 1 sample test
// Macro for custom testing; does exit(1) on failure.
#define CHECK(condition) do{ \
    if (!(condition)) { \
        printf("ERROR: %s (@%d): failed condition \"%s\"\n", __func__, __LINE__, #condition); \
        exit(1);\
    }\
} while(0)

// Test function 
static void mytest(){
	// Test case 1: size <= 0
	struct array_stats_s case_1;
	case_1.min = 0;
	case_1.max = 0;
	case_1.sum = 0;
	const long size_1 = 0;
	long data_1 = 0;
	int result_1 = syscall(_STATS_TEST_, &case_1, data_1, size_1);
	CHECK(result_1 == -1);
	int errorCode_1 = errno;
	printf("Test case 1: size <= 0: (err #%d) PASS\n\n", errorCode_1);

	// Test case 2: invalid pointer
	struct array_stats_s case_2;
	case_2.min = 0;
	case_2.max = 0;
	case_2.sum = 0;
	const long size_2 = 4;
	long data_2[4] = {1, 2, 3, 4};
	int result_2 = syscall(_STATS_TEST_, &case_2, NULL, size_2);
	CHECK(result_2 == -1);
	int result_3 = syscall(_STATS_TEST_, NULL, data_2, size_2);
	CHECK(result_3 == -1);
	int errorCode_2 = errno;
	printf("Test case 2: invalid pointer: (err #%d) PASS\n\n", errorCode_2);

	// Test case 3: size = 1, array = {1}
	printf("Test case 3: size = 1, array = {1}:\n");
	struct array_stats_s case_3;
	case_3.min = 0;
	case_3.max = 0;
	case_3.sum = 0;
	const long size_3 = 1;
	long data_3[1] = {1};
	int result_4 = syscall(_STATS_TEST_, &case_3, data_3, size_3);
	CHECK(result_4 == 0);
	CHECK(case_3.min = 1);
	CHECK(case_3.max = 1);
	CHECK(case_3.sum = 1);
	printf("min = %ld\n", case_3.min);
	printf("max = %ld\n", case_3.max);
	printf("sum = %ld\n", case_3.sum);
	printf("\n");

	// Test case 4: size = 5, array = {-5,8,1,3,2}
	printf("Test case 4: size = 5, array = {-5,8,1,3,2}:\n");
	struct array_stats_s case_4;
	case_4.min = 0;
	case_4.max = 0;
	case_4.sum = 0;
	const long size_4 = 5;
	long data_4[5] = {-5,8,1,3,2};
	int result_5 = syscall(_STATS_TEST_, &case_4, data_4, size_4);
	CHECK(result_5 == 0);
	CHECK(case_4.min = -5);
	CHECK(case_4.max = 8);
	CHECK(case_4.sum = 9);
	printf("min = %ld\n", case_4.min);
	printf("max = %ld\n", case_4.max);
	printf("sum = %ld\n", case_4.sum);
	printf("\n");
	return;
}

int main(int argc, char *argv[])
{
	mytest();

	printf("********************************\n");
    printf("             PASSED!            \n");
    printf("********************************\n");

	return 0;
}