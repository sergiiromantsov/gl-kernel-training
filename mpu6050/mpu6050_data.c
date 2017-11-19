#include "mpu6050_data.h"

void init_mpu6050_data(struct mpu6050_data_holder *data, size_t elements_count,
	int (*read_data)(bool debug))
{
	size_t const element_size = sizeof(struct mpu6050_data_list);
	if (!data)
		return;
	data->elements_count = 0;
	data->element_iter_current = NULL;
	data->read_data = read_data;

	INIT_LIST_HEAD(&data->list.list);

	do {
		struct mpu6050_data_list *element = kzalloc(element_size, GFP_KERNEL);

		pr_info("%s %s: element %p\n",
			THIS_MODULE->name, __FUNCTION__, element);
		INIT_LIST_HEAD(&element->list);
		list_add(&element->list, &data->list.list);
		++data->elements_count;
	} while (data->elements_count < elements_count);

	data->element_iter_current = NULL;
}

void free_mpu6050_data(struct mpu6050_data_holder *data)
{
	struct mpu6050_data_list *node, *tmp;
	if (!data)
		return;

	list_for_each_entry_safe(node, tmp, &data->list.list, list)	{
		pr_info("%s %s: freeing node %p", THIS_MODULE->name, __FUNCTION__, node);
		list_del(&node->list);
		kfree(node);
	}
}

void add_mpu6050_element(struct mpu6050_data_holder *data,
	struct mpu6050_data_elements *element)
{
	int is_last = false;
	if (!data || !element)
		return;
	if (!data->element_iter_current) {
		data->element_iter_current =
			list_first_entry(&data->list.list, struct mpu6050_data_list, list);
	}
	else {
		// check if its the last element
		is_last = list_is_last(&data->element_iter_current->list, &data->list.list);
		if (is_last) {
			struct mpu6050_data_list *first =
				list_first_entry(&data->list.list, struct mpu6050_data_list, list);
			if (first) {
				// move first to the last and use it
				list_move_tail(&first->list, &data->list.list);
				data->element_iter_current = list_next_entry(
					data->element_iter_current, list);
			}
		}
		else {
			data->element_iter_current = list_next_entry(
				data->element_iter_current, list);
		}
	}

	if (data->element_iter_current) {
		memcpy( data->element_iter_current->data.data, element->data,
			sizeof(data->element_iter_current->data.data));
		memcpy( data->element_iter_current->data.extra_data, element->extra_data,
			sizeof(data->element_iter_current->data.extra_data));
		//pr_info("%s %s: was_last: %d next %p", THIS_MODULE->name, __FUNCTION__,
		//	is_last, data->element_iter_current);
	}
	else {
		//pr_err("%s %s: data holder is not initialized properly",
		//	THIS_MODULE->name, __FUNCTION__);
		return;
	}
}

struct mpu6050_data_elements* get_active_element(struct mpu6050_data_holder *data)
{
	struct mpu6050_data_elements* element = NULL;
	if (!data || !data->element_iter_current)
		return element;
	element = &data->element_iter_current->data;
	return element;
}
