
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/platform_device.h>
#include <asm/io.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

static unsigned long bps_rate = 115200;
module_param(bps_rate, ulong, 0444);


static int loopback_state = 0;


static ssize_t x_axis_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	char test[] = "\n\0";

	printk("%s\n", __func__);

	return sprintf(buf, "%s\n", test);
}

static ssize_t y_axis_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	char test[] = "\n\0";

	printk("%s\n", __func__);

	return sprintf(buf, "%s\n", test);
}

static ssize_t z_axis_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	char test[] = "\n\0";

	printk("%s\n", __func__);

	return sprintf(buf, "%s\n", test);
}

static ssize_t loopback_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	printk("loopback store = %s", buf);

	if (strcmp(buf, "on\n") == 0)
	{
		loopback_state = 1;
	}
	else if (strcmp(buf, "off\n") == 0)
	{
		printk("loopback store set off\n");
		loopback_state = 0;
	}

	return count;
}


static DEVICE_ATTR(x_axis, 0444, x_axis_show, NULL);

static __devinit int accel_probe(struct platform_device *pdev)
{
	int ret;

	printk("%s\n", __func__);
	
	ret = device_create_file(&pdev->dev, &dev_attr_x_axis);
	if (ret < 0)
	{
		printk("%s: error in device_create_file\n", __func__);
	}
	else
	{


		//ret = misc_register(&accel_device);
		//if (ret < 0)
		//	pr_err("accel_probe: error %d registering device\n", ret);
	}

	//ret = 0;

	return ret;
}

static void accel_deinit(void)
{
	printk("%s\n", __func__);

		
}

static __devexit int accel_remove(struct platform_device *pdev)
{

	printk("%s\n", __func__);

	accel_deinit();

	device_remove_file(&pdev->dev, &dev_attr_x_axis);

	//misc_deregister(&accel_device);

	return 0;
}


static struct platform_driver accel_driver = {
	.probe = accel_probe,
	.remove = __devexit_p(accel_remove),
	.driver = {
		.name = "lis331dlh_accel",
		.owner = THIS_MODULE
	},
};



/***********************************************
 * accel_init
 ***********************************************/
static int __init accel_init(void)
{
	printk("%s\n", __func__);
	return platform_driver_register(&accel_driver);
}

/***********************************************
 * accel_exit
 ***********************************************/
static void __exit accel_exit(void)
{
	printk("%s\n", __func__);
	platform_driver_unregister(&accel_driver);
	return;
}


MODULE_AUTHOR("Team JJA");
MODULE_DESCRIPTION("LIS331DLH Accelerometer driver");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.01");

module_init(accel_init);
module_exit(accel_exit);
