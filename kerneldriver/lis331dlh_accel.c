/*
 * Kernel driver for the LIS331DLH accelerometer with interfaces for
 * reading x, y, z accelerations via sysfs.
 * 
 * sys fs:
 * Readable:
    /sys/bus/i2c/devices/1-0018/x_axis
      > provides string representation of the x-axis milli-g value
      > example: 1001\n  for 1001 milli-g / 1.001 g
      > range -2000 to 2000
    /sys/bus/i2c/devices/1-0018/y_axis
      > provides string representation of the y-axis milli-g value
      > example: -1234\n  for -1234 milli-g / -1.234 g
      > range -2000 to 2000
    /sys/bus/i2c/devices/1-0018/z_axis
      > provides string representation of the z-axis milli-g value
      > example: -14\n  for -14 milli-g / -0.014 g
      > range -2000 to 2000

   Writable / Readable:
    /sys/bus/i2c/devices/1-0018/sample_rate
      > provides ability to set or get sample rate in Hz
      > example: 50\n for 50Hz
      > values: 0, 0.5, 1, 2, 5, 10, 50, 100, 400, 1000
 */

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

enum lis331dlh_pm_t
{
    LIS331DLH_PM_POWER_DOWN = 0x00,
    LIS331DLH_PM_NORMAL = 0x01,
    LIS331DLH_PM_LOW_POWER_0_5HZ = 0x02,
    LIS331DLH_PM_LOW_POWER_1HZ = 0x03,
    LIS331DLH_PM_LOW_POWER_2HZ = 0x04,
    LIS331DLH_PM_LOW_POWER_5HZ = 0x05,
    LIS331DLH_PM_LOW_POWER_10HZ = 0x06,
};

enum lis331dlh_dr_t
{
    LIS331DLH_DR_50HZ = 0x00,
    LIS331DLH_DR_100HZ = 0x01,
    LIS331DLH_DR_400HZ = 0x02,
    LIS331DLH_DR_1000HZ = 0x03,
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


static int accel_read_axis(struct i2c_client *client, u8 reg_h, u8 reg_l, u8 *buf)
{
	u8 reg_val_h = 0;
	u8 reg_val_l = 0;
	u16 reg_val = 0;
	s16 signed_reg_val = 0;
	int ret_val;

	ret_val = accel_i2c_read(client, reg_h, &reg_val_h);
	if (ret_val == 0)
	{
		ret_val = accel_i2c_read(client, reg_l, &reg_val_l);
		if (ret_val == 0)
		{
			//printk("%02X %02X\n", (int)reg_val_h, (int)reg_val_l);

			reg_val = (((u16)reg_val_h) << 4) | (((u16)reg_val_l >> 4) & 0xF);

			/* Sign extend if negative */
			if (reg_val & 0x800)
			{
				reg_val |= 0xF000;
			}
			signed_reg_val = (s16)reg_val;
		}
	}

	if (ret_val != 0)
	{
		sprintf(buf, "\n");
	}
	else
	{
		ret_val = sprintf(buf, "%d\n", (int)signed_reg_val);
	}

	return ret_val;
}

static ssize_t x_axis_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	int ret_val = 0;

	printk("%s\n", __func__);

	ret_val = accel_read_axis(accel_i2c_client, OUT_X_H, OUT_X_L, buf);

	//ret_val = accel_hex_string_read(accel_i2c_client, WHO_AM_I, buf);

	return ret_val;
}

static ssize_t y_axis_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	int ret_val;

	printk("%s\n", __func__);

	ret_val = accel_read_axis(accel_i2c_client, OUT_Y_H, OUT_Y_L, buf);

	return ret_val;
}

static ssize_t z_axis_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	int ret_val;

	printk("%s\n", __func__);

	ret_val = accel_read_axis(accel_i2c_client, OUT_Z_H, OUT_Z_L, buf);

	return ret_val;
}

static ssize_t sample_rate_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	int ret_val;
	u8 register_value = 0;
	enum lis331dlh_pm_t power_mode = LIS331DLH_PM_POWER_DOWN;
	enum lis331dlh_dr_t data_rate = LIS331DLH_DR_50HZ;

	printk("%s\n", __func__);

	ret_val = accel_i2c_read(accel_i2c_client, CTRL_1, &register_value);
	if (ret_val != 0)
	{
		pr_err("sample_rate_show: error %d registering device\n", ret_val);
		return -EIO;
	}

	power_mode = (register_value & 0xE0) >> 5;

	if (power_mode == LIS331DLH_PM_POWER_DOWN)
	{
		ret_val = sprintf(buf, "0\n");
	}
	else if (power_mode == LIS331DLH_PM_NORMAL)
	{
		data_rate = (register_value & 0x18) >> 3;
		if (data_rate == LIS331DLH_DR_50HZ)
		{
			ret_val = sprintf(buf, "50\n");
		}
		else if (data_rate == LIS331DLH_DR_100HZ)
		{
			ret_val = sprintf(buf, "100\n");
		}
		else if (data_rate == LIS331DLH_DR_400HZ)
		{
			ret_val = sprintf(buf, "400\n");
		}
		else
		{
			ret_val = sprintf(buf, "1000\n");
		}
	}
	else if (power_mode == LIS331DLH_PM_LOW_POWER_0_5HZ)
	{
		ret_val = sprintf(buf, "0.5\n");
	}
	else if (power_mode == LIS331DLH_PM_LOW_POWER_1HZ)
	{
		ret_val = sprintf(buf, "1\n");
	}
	else if (power_mode == LIS331DLH_PM_LOW_POWER_2HZ)
	{
		ret_val = sprintf(buf, "2\n");
	}
	else if (power_mode == LIS331DLH_PM_LOW_POWER_5HZ)
	{
		ret_val = sprintf(buf, "5\n");
	}
	else
	{
		// 10Hz
		ret_val = sprintf(buf, "10\n");
	}

	return ret_val;
}


static ssize_t sample_rate_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	enum lis331dlh_pm_t power_mode = LIS331DLH_PM_POWER_DOWN;
	enum lis331dlh_dr_t data_rate = LIS331DLH_DR_50HZ;
	u8 register_value = 0;
	int ret_val = 0;

	printk("sample_rate_store = %s", buf);

	if (strcmp("0\n", buf) == 0)
	{
		power_mode = LIS331DLH_PM_POWER_DOWN;
	}
	else if (strcmp("50\n", buf) == 0)
	{
		//norm
		power_mode = LIS331DLH_PM_NORMAL;
	}
	else if (strcmp("100\n", buf) == 0)
	{
		//norm
		power_mode = LIS331DLH_PM_NORMAL;
		data_rate = LIS331DLH_DR_100HZ;
	}
	else if (strcmp("400\n", buf) == 0)
	{
		//norm
		power_mode = LIS331DLH_PM_NORMAL;
		data_rate = LIS331DLH_DR_400HZ;
	}
	else if (strcmp("1000\n", buf) == 0)
	{
		//norm		
		power_mode = LIS331DLH_PM_NORMAL;
		data_rate = LIS331DLH_DR_1000HZ;
	}
	else if (strcmp("0.5\n", buf) == 0)
	{
		power_mode = LIS331DLH_PM_LOW_POWER_0_5HZ;
	}
	else if (strcmp("1\n", buf) == 0)
	{
		power_mode = LIS331DLH_PM_LOW_POWER_1HZ;
	}
	else if (strcmp("2\n", buf) == 0)
	{
		power_mode = LIS331DLH_PM_LOW_POWER_2HZ;
	}
	else if (strcmp("5\n", buf) == 0)
	{
		power_mode = LIS331DLH_PM_LOW_POWER_5HZ;
	}
	else if (strcmp("10\n", buf) == 0)
	{
		power_mode = LIS331DLH_PM_LOW_POWER_10HZ;
	}
	else
	{
		return -EINVAL;
	}


	ret_val = accel_i2c_read(accel_i2c_client, CTRL_1, &register_value);
	if (ret_val != 0)
	{
		pr_err("sample_rate_store: error %d reading device\n", ret_val);
		return -EIO;
	}

	register_value = (register_value & 0x07) | (power_mode << 5) | (data_rate << 3);
	ret_val = i2c_smbus_write_byte_data(accel_i2c_client, CTRL_1, register_value);
	if (ret_val != 0)
	{
		pr_err("sample_rate_store: error %d writing device\n", ret_val);
		return -EIO;
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
static DEVICE_ATTR(sample_rate, 0666, sample_rate_show, sample_rate_store);

static __devinit int accel_probe(struct i2c_client *client,
	    const struct i2c_device_id *id)
{
	int ret = 0;

	printk("%s\n", __func__);

	//power on and enable X, Y, Z
	ret = i2c_smbus_write_byte_data(client, CTRL_1, 0x27); // 00100111
	if (ret != 0)
	{
		pr_err("accel_probe: error %d registering device\n", ret);
		return -EIO;
	}

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

	ret = device_create_file(&client->dev, &dev_attr_sample_rate);
	if (ret < 0)
	{
		printk("%s: error in device_create_file sample_rate\n", __func__);
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

	device_remove_file(&client->dev, &dev_attr_sample_rate);

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
