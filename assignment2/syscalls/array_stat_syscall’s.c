#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>

struct array_stats_s { 
	long min;
	long max;
	long sum; 
};

SYSCALL_DEFINE3( 
	array_stats,					/* syscall name */
	struct array_stats_s*, stats, 	/* where to write stats */
	long*, data, 					/* data to process */ 
	long, size) 					/* # values in data */
{	
	// Declare variables at the front
	int i = 0;
	long kernel_data;
	long min = 0;
	long max = 0;
	long sum = 0;
	int not_copy_min = 0;
	int not_copy_max = 0;
	int not_copy_sum = 0;
	int not_copy_from_user = 0;

	// Handle non-positive size 
	if (size <= 0){
		return -EINVAL;
	}

	// Handle unable to accesss stats or data pointer
	if (stats == NULL || data == NULL){
		return -EFAULT;
	}

	// copy user-level data[0] to kernel level data
	not_copy_from_user = copy_from_user(&kernel_data, &data[0], sizeof(long));
	if (not_copy_from_user != 0){
		printk("Copy from user initial data to kernel data failed");
		return -EFAULT;
	}

	// Initialze stats variables
	min = kernel_data;
	max = kernel_data;
	sum = kernel_data;

	// Iterate the data array
	for (i = 1; i < size; i++){

		// copy user-level data to kernel level data
		not_copy_from_user = copy_from_user(&kernel_data, &data[i], sizeof(long));
		if (not_copy_from_user != 0){
			printk("Copy from user user data to kernel data failed");
			return -EFAULT;
		}

		// compute the min value 
		if (kernel_data < min)
			min = kernel_data;

		// compute the max value
		if (kernel_data > max)
			max = kernel_data;

		// compute the sum
		sum += kernel_data;
	} 

	// Copy result into stats
	not_copy_min = copy_to_user(&(stats->min), &min, sizeof(long));
	not_copy_max = copy_to_user(&(stats->max), &max, sizeof(long));
	not_copy_sum = copy_to_user(&(stats->sum), &sum, sizeof(long));

	// Check copy successful
	if (not_copy_min != 0 || not_copy_max != 0 || not_copy_sum != 0){
		printk("write user date stats failed");
		return -EFAULT;
	}
	return 0;
}