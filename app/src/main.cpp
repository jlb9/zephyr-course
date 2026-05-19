// #include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>

// #define SLEEP_TIME_MS 500

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

namespace {
    void turn_on_led() {
        const struct device *driver = DEVICE_DT_GET(DT_NODELABEL(our_driver0));
        struct sensor_value val;
        auto ret = sensor_sample_fetch(driver);
        LOG_INF("Channel ret: %d\n", ret);
    }

    void turn_off_led() {
        const struct device *driver = DEVICE_DT_GET(DT_NODELABEL(our_driver0));
        struct sensor_value val;
        auto ret = sensor_channel_get(driver, SENSOR_CHAN_AMBIENT_TEMP, &val);
        LOG_INF("Channel ret: %d\n", ret);
    }
}

int main(void) {
    while (1) {
        turn_on_led();
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);

        turn_off_led();
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    }

    return 0;
}