#ifndef __MPU6050_DATA_H__
#define __MPU6050_DATA_H__

#include <linux/list.h>
#include <linux/slab.h>
#include <linux/module.h>


enum mpu6050_data_index {
	INDEX_ACCEL_X = 0,
	INDEX_ACCEL_Y,
	INDEX_ACCEL_Z,
	INDEX_GYRO_X,
	INDEX_GYRO_Y,
	INDEX_GYRO_Z,
	INDEX_TEMPERATURE,
	INDEX_COUNT,
	/* Another level of data*/
	INDEX_TIMESTAMP = 0,
	INDEX_EXTRA_COUNT
};

struct mpu6050_data_elements {
	int data[INDEX_COUNT];
	u64 extra_data[INDEX_EXTRA_COUNT];
};

struct mpu6050_data_list {
	struct list_head list;
	struct mpu6050_data_elements data;
};

struct mpu6050_data_holder {
	struct i2c_client *drv_client;
	size_t elements_count;
	struct mpu6050_data_list *element_iter_current;
	struct mpu6050_data_list list;
	int (*read_data)(bool debug);
};

void init_mpu6050_data(struct mpu6050_data_holder *data, size_t elements_count,
	int (*read_data)(bool debug));
void free_mpu6050_data(struct mpu6050_data_holder *data);
void add_mpu6050_element(struct mpu6050_data_holder *data,
	struct mpu6050_data_elements *element);
struct mpu6050_data_elements* get_active_element(struct mpu6050_data_holder *data);

#endif// __MPU6050_DATA_H__