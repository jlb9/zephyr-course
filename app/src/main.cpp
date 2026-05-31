#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <our_driver.h>

#define OUR_DRIVER_NODE DT_ALIAS(led)

int main(void)
{
	const struct device *dev = DEVICE_DT_GET(OUR_DRIVER_NODE);

	if (!device_is_ready(dev)) {
		printk("Our driver not ready\n");
		return -ENODEV;
	}

	our_driver_set_message(dev, "heartbeat");

	printk("LED blinking (period %d ms). Shell: sensor fetch/read/info/set/toggle\n",
	       CONFIG_APP_HEARTBEAT_PERIOD_MS);

	while (true) {
		struct sensor_value val;
		int ret;

		ret = sensor_sample_fetch(dev);
		if (ret < 0) {
			printk("sample_fetch failed: %d\n", ret);
			k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
			continue;
		}
		k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS / 2);

		ret = sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);
		if (ret < 0) {
			printk("channel_get failed: %d\n", ret);
			k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
			continue;
		}
		k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS / 2);
	}

	return 0;
}
