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
	int (*read_data)(bool debug);
};

struct mpu6050_data_holder* get_mpu6050_data(void);
void init_mpu6050_data(struct mpu6050_data_holder *data_, size_t elements_count,
	int (*read_data)(bool debug));
void free_mpu6050_data(struct mpu6050_data_holder *data_);
void add_mpu6050_element(struct mpu6050_data_holder *data_,
	struct mpu6050_data_elements *element);
bool get_active_element(struct mpu6050_data_holder *data_,
	struct mpu6050_data_elements *element);
bool get_first_element(struct mpu6050_data_holder *data_,
	struct mpu6050_data_elements *element);
bool get_next_element(struct mpu6050_data_holder *data_,
	struct mpu6050_data_elements *element);
bool is_last_element(struct mpu6050_data_holder *data_);

#endif /* __MPU6050_DATA_H__ */
