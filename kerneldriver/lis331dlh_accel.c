
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/i2c.h>
#include <asm/io.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

static unsigned long bps_rate = 115200;
module_param(bps_rate, ulong, 0444);


static int loopback_state = 0;
static struct i2c_client *accel_i2c_client = NULL;

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

static const struct i2c_device_id accel_id[] = {
	{ "lis331dlh_accel", 0 },
	{ }
};


static DEVICE_ATTR(x_axis, 0444, x_axis_show, NULL);

static __devinit int accel_probe(struct i2c_client *client,
	    const struct i2c_device_id *id)
{
	int ret;

	printk("%s\n", __func__);

	accel_i2c_client = client;
	
	ret = device_create_file(&client->dev, &dev_attr_x_axis);
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

static __devexit int accel_remove(struct i2c_client *client)
{

	printk("%s\n", __func__);

	accel_i2c_client = NULL;

	accel_deinit();

	device_remove_file(&client->dev, &dev_attr_x_axis);

	//misc_deregister(&accel_device);

	return 0;
}



static struct i2c_driver accel_driver = {
	.driver = {
		.name	= "lis331dlh_accel",
		.owner = THIS_MODULE
	},
	.probe		= accel_probe,
	.remove		= accel_remove,
	.id_table	= accel_id,
};

/***********************************************
 * accel_init
 ***********************************************/
static int __init accel_init(void)
{
	int ret;

	printk("%s\n", __func__);

	ret = i2c_add_driver(&accel_driver);

	return ret;
}

/***********************************************
 * accel_exit
 ***********************************************/
static void __exit accel_exit(void)
{
	printk("%s\n", __func__);
	i2c_del_driver(&accel_driver);
	return;
}


MODULE_AUTHOR("Team JJA");
MODULE_DESCRIPTION("LIS331DLH Accelerometer driver");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.01");

module_init(accel_init);
module_exit(accel_exit);
