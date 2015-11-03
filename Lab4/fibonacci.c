#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ILYA POBORCEV");

#define SUCCESS 0
#define BUF_LEN 128
#define DEVICE_NAME "fibonacci"

static int major = -1;
static long n = 0;
static unsigned long long result = 0;
static char buf[BUF_LEN];
static int input_error_code = 0;
static char *input_error_message;

static int fibonacci_init(void);
static void fibonacci_exit(void);
static ssize_t fibonacci_write(struct file *, const char *, size_t, loff_t *);
static ssize_t fibonacci_read(struct file *, char *, size_t, loff_t *);

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = fibonacci_read,
	.write = fibonacci_write
};

static int fibonacci_init() 
{
	major = register_chrdev(249, DEVICE_NAME, &fops);
	if (major < 0) {
		printk(KERN_ALERT "fibonacci register_chrdev() error: %d\n", major);
		return major;
	}
	printk(KERN_NOTICE "major number = %d\n", major);
	return SUCCESS;
}

static void fibonacci_exit() 
{
	if (major > 0)
		unregister_chrdev(249, DEVICE_NAME);
}

static ssize_t fibonacci_write(struct file *filp, const char *buffer, size_t length, loff_t *offset) 
{
	long i;
	unsigned long long a;
	unsigned long long tmp;

	if (length > BUF_LEN) {
		input_error_code = -EINVAL;
		input_error_message = "Too long input string";
		return input_error_code;
	}
	if (copy_from_user(buf, buffer, length)) {
		input_error_code = -EINVAL;
		input_error_message = "copy_from_user error";
		return input_error_code;
	}
	if (kstrtol(buf, 10, &n) == -EINVAL) {
		input_error_code = -EINVAL;
		input_error_message = "Input string is not the number";
		return input_error_code;
	}

	if (n < 0) {
		input_error_code = -EINVAL;
		input_error_message = "Input number must be non-negative";
		return input_error_code;
	}

	a = 0;
	result = 1;

	if (n == 0) {
		result = 0;
	}

	for (i = 0; i < n - 1; ++i) {
		tmp = result;
		result += a;
		if (result < tmp) {
			input_error_code = -EINVAL;
			input_error_message = "Too big input number";
			return input_error_code;
		}
		a = tmp;
	}

	return length;	
}

static ssize_t fibonacci_read(struct file *filp, char *buffer, size_t length, loff_t *offset)
{
	ssize_t size;
	long error_length;

	if (*offset != 0) {
		return 0;
	}

	if (input_error_code < 0) {
		error_length = strlen(input_error_message);
		copy_to_user(buffer, input_error_message, error_length);
		*offset = error_length;
		return error_length;
	}

	snprintf(buf, BUF_LEN, "%llu", result);
	size = strlen(buf);
	if (size >= length) {
		return -EINVAL;
	}
	if (copy_to_user(buffer, buf, size)) {
		return -EINVAL;
	}
	*offset = size;
	return size;
}

module_init(fibonacci_init);
module_exit(fibonacci_exit);
