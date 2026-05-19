#define DT_DRV_COMPAT iomico_our_driver

#include "our_driver.h"

#include <string.h>

#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

struct our_driver_data {
	char label[16];
};

static int sample_fetch_my_impl(const struct device *dev, enum sensor_channel chan)
{
	ARG_UNUSED(dev);

	LOG_INF("Hello From Sample Fetch, channel %d", chan);
	return 0;
}

static int channel_get_my_impl(const struct device *dev, enum sensor_channel chan,
			       struct sensor_value *val)
{
	ARG_UNUSED(dev);
	ARG_UNUSED(val);

	LOG_INF("Hello From Channel Get, channel %d", chan);
	return 0;
}

static int our_driver_set_label_impl(const struct device *dev, const char *label)
{
	struct our_driver_data *data = dev->data;

	strncpy(data->label, label, sizeof(data->label) - 1);
	data->label[sizeof(data->label) - 1] = '\0';

	LOG_INF("Label set to \"%s\"", data->label);
	return 0;
}

static const struct our_driver_api api_iomico_lecture = {
	.sensor = {
		.sample_fetch = sample_fetch_my_impl,
		.channel_get = channel_get_my_impl,
	},
	.set_label = our_driver_set_label_impl,
};

static int __attribute__((used)) our_driver_init(const struct device *dev)
{
	ARG_UNUSED(dev);

	LOG_INF("Device Initialized");
	return 0;
}

#define OUR_DRIVER_DEFINE(inst)                                                                   \
	static struct our_driver_data our_driver_data_##inst;                                      \
	SENSOR_DEVICE_DT_INST_DEFINE(inst, our_driver_init, NULL, &our_driver_data_##inst, NULL,   \
				     POST_KERNEL, CONFIG_SENSOR_INIT_PRIORITY,                     \
				     (const struct sensor_driver_api *)&api_iomico_lecture);

DT_INST_FOREACH_STATUS_OKAY(OUR_DRIVER_DEFINE)
