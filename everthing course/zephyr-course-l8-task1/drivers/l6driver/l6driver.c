#include <zephyr/drivers/sensor.h>        // we're sort of abusing the sensor API to control an LED, so we don't need to define a new API ourselves.
#include <zephyr/drivers/gpio.h>          // access to GPIO API for controlling the LED
#include "l6driver.h"

#define DT_DRV_COMPAT strooom_l6driver        // needed for DEVICE_DT_INST_DEFINE and GPIO_DT_SPEC_INST_GET macros to work. This should match the compatible string in the device tree overlay file for this driver.

// Storage per instance of the driver, populated by the DEVICE_DT_INST_DEFINE macro.
// Each instance could control a different LED.
struct l6_config {
    struct gpio_dt_spec led;
    uint32_t sleep_ms;
};

// implementation of first function of the API
static int sample_fetch_l6(const struct device *dev, enum sensor_channel chan) {
    const struct l6_config *cfg = dev->config;
    printk("Turn LED ON\n");
    gpio_pin_set_dt(&cfg->led, 1);
    k_msleep(cfg->sleep_ms);
    return 0;
}

// implementation of second function of the API
static int get_channel_l6(const struct device *dev, enum sensor_channel chan, struct sensor_value *val) {
    const struct l6_config *cfg = dev->config;
    printk("Turn LED OFF\n");
    gpio_pin_set_dt(&cfg->led, 0);
    k_msleep(cfg->sleep_ms);
    return 0;
}

// custom function, outside the sensor API, to read the configured sleep duration
uint32_t l6_get_sleep_ms(const struct device *dev) {
    const struct l6_config *cfg = dev->config;
    return cfg->sleep_ms;
}

// which functions of the generic sensor API are implemented by this driver
static DEVICE_API(sensor, l6api) = {
    .sample_fetch = sample_fetch_l6,
    .channel_get  = get_channel_l6,
};

// initialization function for the driver, called by DEVICE_DT_INST_DEFINE macro for each instance of the driver. This is where we configure the GPIO pin for the LED.
static int l6_init(const struct device *dev) {
    const struct l6_config *cfg = dev->config;
    printk("Initializing L6 Driver\n");
    gpio_pin_configure_dt(&cfg->led, GPIO_OUTPUT_INACTIVE);
    return 0;
}

// Macro to enable multiple instances of the driver based on the device tree.
#define DEV_INST(inst)                                        \
    static const struct l6_config l6_config_##inst = {        \
        .led      = GPIO_DT_SPEC_INST_GET(inst, gpios),       \
        .sleep_ms = DT_INST_PROP(inst, sleep_ms),             \
    };                                                        \
    DEVICE_DT_INST_DEFINE(inst, l6_init, NULL, NULL,   \
                          &l6_config_##inst, POST_KERNEL, 80, &l6api);

DT_INST_FOREACH_STATUS_OKAY(DEV_INST)
