// SPDX-License-Identifier: GPL-2.0
/**
 * Copyright (C) Demidov 2022
 *
 * Authors: Danila Demidov <dandemidow@gmail.com>
 */

#include <linux/component.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/pwm.h>
#include <linux/uaccess.h>

static unsigned int ang_data;
struct servo_dev {
  struct pwm_chip chip;
};

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
    int val,ret;
    ret = copy_from_user(&val, data, len);
    printk(KERN_INFO "val:%d\n",val);
    if(val>100){
        printk(KERN_INFO "error\n");
        return -1;
    }
    val = 100000 - (val * 1000);
//    writel_relaxed(val,base + PWM_REG_LPR);
    return 0;
}

static ssize_t servo_read(struct file *filp, char __user *userbuf, size_t count, loff_t *ppos)
{
    if (copy_to_user(userbuf, &ang_data, sizeof(ang_data)) != 0) {
      return -EIO;
    }

    return sizeof(unsigned int);
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


static int servo_pwm_request(struct pwm_chip *chip, struct pwm_device *pwm) {
    struct platform_device *pdev = to_platform_device(chip->dev);

    return 0;
}

static void servo_pwm_free(struct pwm_chip *chip, struct pwm_device *pwm) {
    struct platform_device *pdev = to_platform_device(chip->dev);
}

static int servo_pwm_config(struct pwm_chip *chip, struct pwm_device *pwm,
                 int duty_ns, int period_ns) {
    return 0;
}

static int servo_pwm_polarity(struct pwm_chip *chip, struct pwm_device *pwm,
                   enum pwm_polarity polarity) {
    return 0;
}

static int servo_pwm_enable(struct pwm_chip *chip, struct pwm_device *pwm) {
    return 0;
}

static void servo_pwm_disable(struct pwm_chip *chip, struct pwm_device *pwm) {

}

static const struct pwm_ops servo_pwm_ops = {
    .request = servo_pwm_request,
    .free = servo_pwm_free,
    .config = servo_pwm_config,
    .set_polarity = servo_pwm_polarity,
    .enable = servo_pwm_enable,
    .disable = servo_pwm_disable,
    .owner = THIS_MODULE,
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

    struct servo_dev *servo_dev;
    servo_dev = devm_kzalloc(dev, sizeof(*servo_dev), GFP_KERNEL);
    if (!servo_dev)
        return -ENOMEM;

    servo_dev->chip.dev = &pdev->dev;
    servo_dev->chip.ops = &servo_pwm_ops;
    servo_dev->chip.base = -1;
    servo_dev->chip.npwm = 1;

    ret = pwmchip_add(&servo_dev->chip);
    if (ret < 0)
        return ret;

    platform_set_drvdata(pdev, servo_dev);

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
    { .compatible = "st,stm32f429-servo"},
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
