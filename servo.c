// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) Demidov 2022
 *
 * Authors: Danila Demidov <dandemidow@gmail.com>
 */

#include <linux/component.h>
#include <linux/module.h>
#include <linux/of_platform.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>

static int servo_open(struct inode *inode, struct file *file)
{
    return 0;
}

static int servo_release(struct inode *inode, struct file *file)
{
    return 0;
}

static ssize_t servo_write(struct file *file, const char __user *data,
	                   size_t len, loff_t *ppos)
{
    return 0;
}

static ssize_t servo_read(struct file *filp, char __user *userbuf, size_t count, loff_t *ppos)
{
    return 0;
}

static long servo_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    return 0;
}

static struct file_operations dev_fops = {
    .owner   =   THIS_MODULE,
    .open    =   servo_open,
    .release =   servo_release,
    .write   =   servo_write,
    .read    =   servo_read,
    .unlocked_ioctl   =   servo_ioctl,
};
 
static struct miscdevice misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "servo",
    .fops = &dev_fops,
};

static int stm_servo_platform_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	int ret;

	// register miscdevice
	if(misc_register(&misc)) {
	    pr_err("could not register smilebrd misc device\n");
	    return EINVAL;
	}

	return 0;

err_put:

	return ret;
}

static int stm_servo_platform_remove(struct platform_device *pdev)
{
    misc_deregister(&misc);
	return 0;
}

static const struct of_device_id drv_dt_ids[] = {
	{ .compatible = "st,st-servo"},
	{ /* end node */ },
};

static struct platform_driver stm_servo_platform_driver = {
	.probe = stm_servo_platform_probe,
	.remove = stm_servo_platform_remove,
	.driver = {
		.name = "stm32-servo",
		.of_match_table = drv_dt_ids,
		.pm = NULL,
	},
};

module_platform_driver(stm_servo_platform_driver);

MODULE_AUTHOR("Danila Demidov <dandemidow@gmail.com>");
MODULE_DESCRIPTION("STMicroelectronics Servo driver");
MODULE_LICENSE("GPL v2");
