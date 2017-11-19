#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>

#include "mpu6050_data.h"


struct cdev_instance {
	unsigned int major;
	dev_t dev_no;
	dev_t read_all;
	struct cdev *cdev;
};

struct cdevs_holder {
	struct cdev_instance cdev_line;
	struct cdev_instance cdev_full;
	struct mpu6050_data_holder *data;
};

static inline struct cdevs_holder *get_cdevs(void)
{
	static struct cdevs_holder g_cdevs_holder;
	return &g_cdevs_holder;
}

struct cdev_instance *get_cdev(dev_t dev_no);
int init_cdevs(struct cdevs_holder *cdevs, struct mpu6050_data_holder *data);
void free_cdevs(struct cdevs_holder *cdevs);
