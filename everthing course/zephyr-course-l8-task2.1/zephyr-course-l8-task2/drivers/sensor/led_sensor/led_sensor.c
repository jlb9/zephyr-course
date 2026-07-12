#define DT_DRV_COMPAT custom_led_sensor
#include <errno.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include "led_sensor.h"

struct led_sensor_config {
    struct gpio_dt_spec led;
};

struct led_sensor_data {
	uint32_t blink_count;
};

static int led_sensor_sample_fetch(const struct device *dev,
                                   enum sensor_channel chan) {

    const struct led_sensor_config *cfg = dev->config;

    struct led_sensor_data *data = dev->data;
    data->blink_count++;
    return gpio_pin_set_dt(&cfg->led, 1);
}

static int led_sensor_channel_get(const struct device *dev,
                                  enum sensor_channel chan,
                                  struct sensor_value *val) {
    const struct led_sensor_config *cfg = dev->config;

    struct led_sensor_data *data = dev->data;
    gpio_pin_set_dt(&cfg->led, 0);
    val->val1 = data->blink_count;
    val->val2 = 0;
    return 0;
}

static int led_sensor_set_blink_count_impl(const struct device *dev,
                                           uint32_t count) {
    struct led_sensor_data *data = dev->data;

    data->blink_count = count;
    return 0;
}

static int led_sensor_init(const struct device *dev) {
    const struct led_sensor_config *cfg = dev->config;
    if (!gpio_is_ready_dt(&cfg->led)) {
        return -ENODEV;
    }
    return gpio_pin_configure_dt(&cfg->led, GPIO_OUTPUT_INACTIVE);
}

static const struct led_sensor_driver_api led_sensor_api = {
    .sensor =
        {
            .sample_fetch = led_sensor_sample_fetch,
            .channel_get = led_sensor_channel_get,
        },
    .set_blink_count = led_sensor_set_blink_count_impl,
};

#define LED_SENSOR_DEFINE(n)                                                   \
    static struct led_sensor_data led_sensor_data_##n;                         \
    static const struct led_sensor_config led_sensor_cfg_##n = {               \
        .led = GPIO_DT_SPEC_INST_GET(n, gpios),                                \
    };                                                                         \
	DEVICE_DT_INST_DEFINE(n, led_sensor_init, NULL,         \
			      &led_sensor_data_##n,                     \
			      &led_sensor_cfg_##n,			            \
			      POST_KERNEL, CONFIG_SENSOR_INIT_PRIORITY,	\
			      &led_sensor_api);

DT_INST_FOREACH_STATUS_OKAY(LED_SENSOR_DEFINE)
