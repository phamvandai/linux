#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/sched/signal.h>

static struct task_struct *thread_st;

static int thread_fn(void *unused)
{
    // Signal handler in kthread. It only allows signal
    // but has no signal handler, it should continue check for any signal
    // set and handle it.
    allow_signal(SIGKILL);

    // Check for stop request 
    while(!kthread_should_stop()) {
        printk(KERN_INFO "Thread running\n");
        ssleep(5);

        // Check for pending signal 
        if(signal_pending(thread_st)) {
            break;
        }
    }

    printk(KERN_INFO "Thread stopping\n");
    do_exit(0);

    return 0;
}

static int __init init_thread(void)
{
    printk(KERN_INFO "Creating thread\n");
  
#if 1   // kthread_create
    thread_st = kthread_create(thread_fn, NULL, "mythread");
    if(thread_st) {
        printk(KERN_INFO "Thread created done\n");
        // Thread created but not started, need to explicit start 
        wake_up_process(thread_st);
    }
    else 
        printk(KERN_INFO "Thread created failed\n");

#else   // kthread_run
    kthread_run(thread_fn, NULL, "mythread");

#endif
    return 0;
}

static void __exit cleanup_thread(void)
{
    printk(KERN_INFO "Cleaning up\n");
    if(thread_st) {
        // This is blocking call; if thread not stopped, we will block here!
        int ret = kthread_stop(thread_st);
        thread_st = NULL;
        printk(KERN_INFO "Thread stopped %d\n", ret);
    }
}

MODULE_LICENSE("GPL");
module_init(init_thread);
module_exit(cleanup_thread);