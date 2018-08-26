#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

#include <linux/_______.h>

int my_proc_read(char *page, char **start, off_t off,int count,int *eof, void *data_unused)
{
    int len;

    len = sprintf(page, "Hi, I'm in /proc File System!!!\n");
    return ______;
}

static int __init my_proc_init(void)
{
    struct proc_dir_entry * my_proc_entry;

    printk("MY_PROC Module Loading!!\n");

    if((my_proc_entry = __________("my_proc", S_IRUGO , NULL)) == NULL)
        return -EACCES;

    my_proc_entry->_________ = my_proc_read;

    return 0;
}

static void __exit my_proc_exit(void)
{
    printk("MY_PROC Module UnLoading!!\n");
    ____________("my_proc", NULL);
}

module_init(my_proc_init);
module_exit(my_proc_exit);

MODULE_LICENSE("Dual BSD/GPL");