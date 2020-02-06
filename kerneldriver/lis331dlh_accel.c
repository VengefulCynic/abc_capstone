
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


enum lis331dlh_reg
{
    WHO_AM_I = 0x0F,
    CTRL_1 = 0x20,
    CTRL_2 = 0X21,
    CTRL_3 = 0X22,
    CTRL_4 = 0X23,
    CTRL_5 = 0X24,
    HP_FILTER_RESET = 0X25,
    REFERENCE = 0X26,
    STATUS = 0X27,
    OUT_X_L = 0X28,
    OUT_X_H = 0X29,
    OUT_Y_L = 0X2A,
    OUT_Y_H = 0X2B,
    OUT_Z_L = 0X2C,
    OUT_Z_H = 0X2D,
    INT1_CFG = 0X30,
    INT1_SOURCE = 0X31,
    INT1_THS = 0X32,
    INT1_DURATION = 0X33,
    INT2_CFG = 0X34,
    INT2_SOURCE = 0X35,
    INT2_THS = 0X36,
    INT2_DURATION = 0X37
};

static unsigned long bps_rate = 115200;
module_param(bps_rate, ulong, 0444);


//static int loopback_state = 0;
static struct i2c_client *accel_i2c_client = NULL;


static int accel_i2c_read(struct i2c_client *client, u8 reg, u8 *buf)
{
	s32 ret;

	ret = i2c_smbus_read_byte_data(client, reg);
	if (ret < 0)
		return -EIO;

	*buf = ret;
	return 0;
}



static ssize_t x_axis_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	char test[] = "x\n\0";
	u8 id_reg = 0;
	int ret_val = 0;

	printk("%s\n", __func__);

	ret_val = accel_i2c_read(accel_i2c_client, WHO_AM_I, &id_reg);

	if (ret_val == 0)
	{
		ret_val = sprintf(buf, "%02X\n", (int)id_reg);
	}
	else
	{
		sprintf(buf, "\n");
	}

	return ret_val;
}

static ssize_t y_axis_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	char test[] = "y\n\0";

	printk("%s\n", __func__);

	return sprintf(buf, "%s\n", test);
}

static ssize_t z_axis_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	char test[] = "z\n\0";

	printk("%s\n", __func__);

	return sprintf(buf, "%s\n", test);
}

static ssize_t loopback_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	printk("loopback store = %s", buf);

	if (strcmp(buf, "on\n") == 0)
	{
		//loopback_state = 1;
	}
	else if (strcmp(buf, "off\n") == 0)
	{
		printk("loopback store set off\n");
		//loopback_state = 0;
	}

	return count;
}

static const struct i2c_device_id accel_id[] = {
	{ "lis331dlh_accel", 0 },
	{ }
};


static DEVICE_ATTR(x_axis, 0444, x_axis_show, NULL);
static DEVICE_ATTR(y_axis, 0444, y_axis_show, NULL);
static DEVICE_ATTR(z_axis, 0444, z_axis_show, NULL);

static __devinit int accel_probe(struct i2c_client *client,
	    const struct i2c_device_id *id)
{
	int ret = 0;

	printk("%s\n", __func__);

	accel_i2c_client = client;
	
	ret = device_create_file(&client->dev, &dev_attr_x_axis);
	if (ret < 0)
	{
		printk("%s: error in device_create_file x axis\n", __func__);
		return ret;
	}

	
	ret = device_create_file(&client->dev, &dev_attr_y_axis);
	if (ret < 0)
	{
		printk("%s: error in device_create_file y axis\n", __func__);
		return ret;
	}

	ret = device_create_file(&client->dev, &dev_attr_z_axis);
	if (ret < 0)
	{
		printk("%s: error in device_create_file z axis\n", __func__);
		return ret;
	}

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

	device_remove_file(&client->dev, &dev_attr_y_axis);

	device_remove_file(&client->dev, &dev_attr_z_axis);

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
