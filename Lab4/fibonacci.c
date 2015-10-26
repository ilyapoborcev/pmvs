#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ILYA POBORCEV");

#define SUCCESS 0
#define BUF_LEN 128
#define NUMBER_NAME "fib_number"
#define RESULT_NAME "fib_result"

static int number_major = -1;
static int result_major = -1;
static long n = 0;
static unsigned long long result = 0;
static char buf[BUF_LEN];
static char* buf_ptr;

static int fibonacci_init(void);
static void fibonacci_exit(void);
static ssize_t number_write(struct file*, const char*, size_t, loff_t*);
static ssize_t number_read(struct file*, char*, size_t, loff_t*);
static ssize_t result_read(struct file*, char*, size_t, loff_t*);

static struct file_operations number_fops = {
	.owner = THIS_MODULE,
	.read = number_read,
	.write = number_write
};

static struct file_operations result_fops = {
	.owner = THIS_MODULE,
	.read = result_read
};

static int fibonacci_init() {
	number_major = register_chrdev(0, NUMBER_NAME, &number_fops);
	if (number_major < 0) {
		printk(KERN_ALERT "fib_number register_chrdev() error: %d\n", number_major);
		return number_major;
	}
	result_major = register_chrdev(0, RESULT_NAME, &result_fops);
	if (result_major < 0) {
		printk(KERN_ALERT "fib_result register_chrdev() error: %d\n", result_major);
		return result_major;
	}
	printk(KERN_NOTICE "number_major = %d\n", number_major);
	printk(KERN_NOTICE "result_major = %d\n", result_major);
	return SUCCESS;
}

static void fibonacci_exit() {
	int res = unregister_chrdev(number_major, NUMBER_NAME);
	if (res < 0)
		printk(KERN_ALERT "fib_number unregister_chrdev() error: %d\n", res);
	res = unregister_chhrdev(result_major, RESULT_NAME);
	if (res < 0)
		printk(KERN_ALERT "fib_result unregister_chrdev() error: %d\n", res);
}

static ssize_t number_write(struct file* filp, const char* buffer, size_t length, loff_t* offset) {
		
}

static ssize_t number_read(struct file* filp, const char* buffer, size_t length, loff_t* offset) {

}

static ssize_t result_read(struct file* filp, const char* buffer, size_t length, loff_t* offset) {
	
}

module_init(fibonacci_init);
module_exit(fibonacci_exit);
