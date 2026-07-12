#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/device.h>
#include "led_sensor.h"

#define LED_SENSOR_NODE DT_NODELABEL(led_sensor)

int main(void)
{
    printk("Hello from My Board - Custom LED Sensor Driver - Shell integration!\n");

    const struct device *dev = DEVICE_DT_GET(LED_SENSOR_NODE);

    if (!device_is_ready(dev)) {
        printk("LED sensor device not ready\n");
        return 0;
    }

    led_sensor_set_blink_count(dev, 0);
    printk("LED sensor driver ready...\n");

    struct sensor_value val;

    while (1) {
        // struct sensor_value val;
        // printk("Turning LED ON using sensor_sample_fetch()\n");
        // sensor_sample_fetch(dev);

        k_sleep(K_MSEC(1000)); // This will avoid blocks the lower priority threads like shell.

        // sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);
        // printk("Turning LED OFF using sensor_channel_get() : Count : %d\n", val.val1);
        // k_sleep(K_MSEC(1000));
    }
}