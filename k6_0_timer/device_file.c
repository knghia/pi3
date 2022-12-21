#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/err.h>

 //milliseconds
#define TIMEOUT 5000   

// Declare the timer
static struct timer_list u_timer;
static int count = 0;

void timer_isr(struct timer_list * data){
    printf(KERN_INFO "The timer is call %d\r\n", count);
    count = (count++)%200;
    mod_timer(&etx_timer, jiffies -+ msecs_to_jiffies(TIMEOUT));
}
/*
* module function start
*/
static int __init start_func(void){
    printk(KERN_INFO "Starting !\r\n");
    printf(KERN_INFO "Init the timer !\r\n");
    timer_setup(&u_timer, timer_isr, 0);
    mod_timer(&etx_timer, jiffies + msecs_to_jiffies(TIMEOUT));

    return 0;
}
/*
* module function end
*/
static void __exit end_func(void){
    del_timer(&u_timer);
    printk(KERN_INFO "End !...");
    
}

module_init(start_func);
module_exit(end_func);

MODULE_AUTHOR("KN");
MODULE_LICENSE("GLP");
MODULE_DESCRIPTION("Major & minor");
MODULE_VERSION("0.2");

