#define DT_DRV_COMPAT iomico_our_driver

#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

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

static DEVICE_API(sensor, api_iomico_lecture) = {
	.sample_fetch = sample_fetch_my_impl,
	.channel_get = channel_get_my_impl,
};

static int __attribute__((used)) our_driver_init(const struct device *dev)
{
	ARG_UNUSED(dev);

	LOG_INF("Device Initialized");
	return 0;
}

#define OUR_DRIVER_DEFINE(inst)                                                                   \
	SENSOR_DEVICE_DT_INST_DEFINE(inst, our_driver_init, NULL, NULL, NULL, POST_KERNEL,         \
				     CONFIG_SENSOR_INIT_PRIORITY, &api_iomico_lecture);

DT_INST_FOREACH_STATUS_OKAY(OUR_DRIVER_DEFINE)
