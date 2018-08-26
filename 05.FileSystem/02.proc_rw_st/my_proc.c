#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>


/*
#include <linux/unistd.h>
#include <linux/errno.h>
*/

#include <linux/proc_fs.h>
#include <asm/uaccess.h>

struct proc_dir_entry *time_dir, *my_dir;
unsigned long my_start = 0;

int proc_read_my(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	int size1, size2;
	size1 = sprintf(page, "my : %lu.%03lusec\n", (jiffies-my_start) / 100, (jiffies-my_start) % 100);
	size2 = sprintf(________, "my_HZ : %lu.%03lusec\n", (jiffies-my_start) / HZ, (jiffies-my_start) % HZ);
	return _______;
}

int proc_write_my(struct file *foke, const char *buffer, unsigned long count, void *data)
{
	char input;
	copy_from_user(&input, buffer, 1);
	if(input == '1') {
		my_start = jiffies;
		printk("my : initiated\n");		
	}
	else {
		printk("my : invalid input\n");
	}
	return count;
}


int procTest_init(void)
{
	printk(KERN_INFO "procTest Module is Loaded!! ....\n");

	time_dir = _________("time", NULL);
	my_dir = ___________("my", 0644, _______);
	my_dir->________ = proc_read_my;
	my_dir->________ = proc_write_my;
	printk("/proc/time/my procfs created!!\n");

	return 0;
}

void procTest_exit(void)
{
	__________("my", ______);
	__________("time", ______);
	printk("/proc/time/my procfs removed!!\n");

	printk("procTest Module is Unloaded ....\n");
}

module_init(procTest_init);
module_exit(procTest_exit);

MODULE_LICENSE("Dual BSD/GPL");


