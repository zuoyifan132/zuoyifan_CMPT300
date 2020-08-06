#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
#include <linux/sched.h>

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

SYSCALL_DEFINE3( 
	process_ancestors, 					/* syscall name for macro */ 
	struct process_info*, info_array, 	/* array of process info strct */
	long, size, 						/* size of the array */
	long*, num_filled) 					/* # elements written to array */
{
	// Declare variables at the front
	struct task_struct* current_task = current;
	struct list_head* children_list;
	struct list_head* sibling_list;
	int i = 0;
	long filled = 0;
	long num_children = 0;
	long num_siblings = 0;
	long buffer_pid;
	long buffer_state;
	int not_copy_pid = 0;
	int not_copy_name = 0;
	int not_copy_state = 0;
	int not_copy_uid = 0;
	int not_copy_nvcsw = 0;
	int not_copy_nivcsw = 0;
	int not_copy_child = 0;
	int not_copy_sibling = 0;
	int not_copy_filled = 0;

	// Handle non-positive size 
	if (size <= 0){
		return -EINVAL;
	}

	// Handle unable to accesss info_array or num_filled
	if (info_array == NULL || num_filled == NULL){
		return -EFAULT;
	}

	// Record of current_task and its ancestor
	for (i = 0; i < size; i++){
		// count number of children
		list_for_each(children_list, &(current_task->children)) {
			num_children++;
		}
		// count number of siblings
		list_for_each(sibling_list, &(current_task->sibling)) {
			num_siblings++;
		}

		// Casting 
		buffer_pid = (long)current_task->pid;
		buffer_state = (long)current_task->state;

		// copy information to process_info
		not_copy_pid = copy_to_user(&(info_array[i].pid), &buffer_pid, sizeof(long));
		not_copy_name = copy_to_user(&(info_array[i].name), &current_task->comm, ANCESTOR_NAME_LEN*sizeof(char));
		not_copy_state = copy_to_user(&(info_array[i].state), &buffer_state, sizeof(long));
		not_copy_uid = copy_to_user(&(info_array[i].uid), &current_task->cred->uid.val, sizeof(long));
		not_copy_nvcsw = copy_to_user(&(info_array[i].nvcsw), &current_task->nvcsw, sizeof(long));
		not_copy_nivcsw = copy_to_user(&(info_array[i].nivcsw), &current_task->nivcsw, sizeof(long));
		not_copy_child = copy_to_user(&(info_array[i].num_children), &num_children, sizeof(long));
		not_copy_sibling = copy_to_user(&(info_array[i].num_siblings), &num_siblings, sizeof(long));

		// check copy successful
		if (not_copy_pid != 0 || not_copy_name != 0 || not_copy_state != 0 || not_copy_uid != 0 || not_copy_nvcsw != 0 || not_copy_nivcsw != 0 || not_copy_child != 0 || not_copy_sibling != 0){
			printk("write to user data info_array failed");
			return -EFAULT;
		}

		// reset num_children and num_siblings
		num_children = 0;
		num_siblings = 0;

		filled++;

		// Repeat until the parent of the process you are working on is itself
		if (current_task->parent == current_task)
			break;

		current_task = current_task->parent;
	}

	// Copy filled to num_filled
	not_copy_filled = copy_to_user(num_filled, &filled, sizeof(long));
	if (not_copy_filled != 0){
		printk("write to num_filled failed");
		return -EFAULT;
	}

	return 0;
}