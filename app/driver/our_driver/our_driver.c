#define DT_DRV_COMPAT iomico_our_driver

#include "our_driver.h"

#include <string.h>

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(our_driver, CONFIG_SENSOR_LOG_LEVEL);

struct our_driver_config {
	struct gpio_dt_spec led;
};

struct our_driver_data {
	char label[16];
	bool led_on;
};

static int sample_fetch_my_impl(const struct device *dev, enum sensor_channel chan)
{
	struct our_driver_data *data = dev->data;
	const struct our_driver_config *cfg = dev->config;

	ARG_UNUSED(chan);

	data->led_on = true;
	gpio_pin_set_dt(&cfg->led, 1);
	LOG_INF("LED ON - %s", data->label);
	return 0;
}

static int channel_get_my_impl(const struct device *dev, enum sensor_channel chan,
			       struct sensor_value *val)
{
	struct our_driver_data *data = dev->data;
	const struct our_driver_config *cfg = dev->config;

	ARG_UNUSED(chan);

	data->led_on = false;
	gpio_pin_set_dt(&cfg->led, 0);
	val->val1 = 0;
	val->val2 = 0;
	LOG_INF("LED off (channel_get)");
	return 0;
}

static int our_driver_set_label_impl(const struct device *dev, const char *label)
{
	struct our_driver_data *data = dev->data;

	strncpy(data->label, label, sizeof(data->label) - 1);
	data->label[sizeof(data->label) - 1] = '\0';

	LOG_INF("Label updated: %s", data->label);
	return 0;
}

static const struct our_driver_api api_iomico_lecture = {
	.sensor = {
		.sample_fetch = sample_fetch_my_impl,
		.channel_get = channel_get_my_impl,
	},
	.set_label = our_driver_set_label_impl,
};

static int our_driver_init(const struct device *dev)
{
	const struct our_driver_config *cfg = dev->config;

	if (!gpio_is_ready_dt(&cfg->led)) {
		LOG_ERR("GPIO not ready");
		return -ENODEV;
	}

	gpio_pin_configure_dt(&cfg->led, GPIO_OUTPUT_INACTIVE);
	LOG_INF("Our driver initialized");
	return 0;
}

#define OUR_DRIVER_DEFINE(inst)                                                                   \
	static struct our_driver_data our_driver_data_##inst;                                      \
	static const struct our_driver_config our_driver_config_##inst = {                         \
		.led = GPIO_DT_SPEC_INST_GET(inst, gpios),                                          \
	};                                                                                         \
	SENSOR_DEVICE_DT_INST_DEFINE(inst, our_driver_init, NULL, &our_driver_data_##inst,         \
				     &our_driver_config_##inst, POST_KERNEL,                       \
				     CONFIG_SENSOR_INIT_PRIORITY,                                  \
				     (const struct sensor_driver_api *)&api_iomico_lecture);

DT_INST_FOREACH_STATUS_OKAY(OUR_DRIVER_DEFINE)
