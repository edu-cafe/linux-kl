#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/delay.h>
#include <linux/kthread.h>

#include <linux/timer.h>


#ifdef CONFIG_SMP
#define SPIN
#endif

#ifndef SPIN
int gcnt=0;
#else
//spinlock_t myevent_lock;
DEFINE_SPINLOCK(myevent_lock);
int gcnt=0;
#endif

struct task_struct *task1=NULL;
struct task_struct *task2=NULL;
struct task_struct *task3=NULL;

struct timer_list timer;

void my_delay(volatile int cnt)
{
	volatile int i, j;
	for(i=0; i<cnt; i++) {
		for(j=0; j<1000000; j++) {
			j = j;
		}
	}
}

void my_timer(unsigned long data)
{
	int t=0;
#if 0
	int i;
	for(i=1; i<=data; i++) {
		printk("Kernel Timer Time-Out Function Doing_%d...\n", i);
	}
#endif

#ifndef SPIN
	printk("[MY_TIMER_before(%d)!!]\n", gcnt);
	t = gcnt;
	t += 1;
	gcnt = t;
	printk("[MY_TIMER_after(%d)!!]\n", gcnt);
#else
	printk("[MY_TIMER_SPIN_before(%d)!!]\n", gcnt);
	spin_lock(&myevent_lock);
	t = gcnt;
	t += 1;
	gcnt = t;
	spin_unlock(&myevent_lock);
	printk("[MY_TIMER_SPIN_after(%d)!!]\n", gcnt);
#endif

    timer.expires = jiffies + 1*HZ;
    add_timer(&timer);

	printk("Kernel Timer Time-Out Function Done!!!\n");
}

int thread1(void *unused)
{
	int t=0;

	printk("thread1 start..\n");

	while(!kthread_should_stop()) {
#ifndef SPIN
		printk("[thread1_before(%d)!!]\n", gcnt);
		t = gcnt;
		t += 1;
		my_delay(100);
		gcnt = t;
		printk("[thread1_after(%d)!!]\n", gcnt);
#else
		printk("[thread1_SPIN_before(%d)!!]\n", gcnt);
		spin_lock(&myevent_lock);
		//spin_lock_irqsave(&myevent_lock, flags);

		t = gcnt;
		t += 1;
		my_delay(100);
		gcnt = t;

		spin_unlock(&myevent_lock);
		printk("[thread1_SPIN_after(%d)!!]\n", gcnt);
		//spin_unlock_irqrestore(&myevent_lock, flags);
#endif

		msleep(1000);
    }

	task1 = NULL;
	printk("thread1 exit..\n");
    return 0;
}

int thread2(void *unused)
{
	int t=0;
	printk("\t\t-->thread2 start..\n");

	while(!kthread_should_stop()) {
#ifndef SPIN
		printk("\t\t-->thread2_before(%d)!!\n", gcnt);
		t = gcnt;
		t += 100;
		my_delay(100);
		gcnt = t;
		printk("\t\t-->thread2_after(%d)!!\n", gcnt);
#else
		printk("\t\t-->thread2_SPIN_before(%d)!!\n", gcnt);
		spin_lock(&myevent_lock);

		t = gcnt;
		t += 100;
		my_delay(100);
		gcnt = t;

		spin_unlock(&myevent_lock);

		printk("\t\t-->thread2_SPIN_after(%d)!!\n", gcnt);
#endif

		msleep(500);
    }

	task2 = NULL;
	printk("\t\t-->thread2 exit..\n");
    return 0;
}

int thread3(void *unused)
{
	int cnt=0;

	while(!kthread_should_stop()) {
		printk("\t\t-->thread3(%d)!!\n", ++cnt);
		msleep(300);
    }

	task3 = NULL;
	printk("\t\t\t==>thread3 exit..\n");
    return 0;
}

int SpinLockTest_init(void)
{
	printk("SpinLockTest Module id Loaded ....\n");

	task1 = kthread_create(thread1, NULL, "%s", "thread1");
	if (task1) wake_up_process(task1);
	printk("Kernel Thread Create Done(task1:%p)!!\n", task1);

	task2 = kthread_create(thread2, NULL, "%s", "thread2");
	if (task2) wake_up_process(task2);
	printk("Kernel Thread Create Done(task2:%p)!!\n", task2);

	task3 = kthread_create(thread3, NULL, "%s", "thread3");
	if (task3) wake_up_process(task3);
	printk("Kernel Thread Create Done(task3:%p)!!\n", task3);

    init_timer(&timer);
    timer.expires = jiffies + 1*HZ;
    timer.function = my_timer;
    timer.data = 5;
    add_timer(&timer);

	return 0;
}

void SpinLockTest_exit(void)
{
	if(task1) {
		printk("task1 thread stopped(%p)!!\n", task1);
		kthread_stop(task1);
	}
	if(task2) {
		printk("task2 thread stopped(%p)!!\n", task2);
		kthread_stop(task2);
	}
	if(task3) {
		printk("task3 thread stopped(%p)!!\n", task3);
		kthread_stop(task3);
	}

    del_timer(&timer);

	printk("SpinLockTest Module is Unloaded ....\n");
}

module_init(SpinLockTest_init);
module_exit(SpinLockTest_exit);

MODULE_LICENSE("Dual BSD/GPL");
