#include <zephyr/shell/shell.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>

static const struct device *get_our_driver(void)
{
	return DEVICE_DT_GET(DT_ALIAS(led));
}

static int cmd_sensor_fetch(const struct shell *sh, size_t argc, char **argv)
{
	const struct device *dev = get_our_driver();

	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	if (!device_is_ready(dev)) {
		shell_error(sh, "Sensor not available");
		return -ENODEV;
	}

	int ret = sensor_sample_fetch(dev);
	if (ret) {
		shell_error(sh, "fetch failed: %d", ret);
		return ret;
	}

	shell_print(sh, "fetch OK - LED on");
	return 0;
}

static int cmd_sensor_read(const struct shell *sh, size_t argc, char **argv)
{
	const struct device *dev = get_our_driver();

	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	if (!device_is_ready(dev)) {
		shell_error(sh, "Sensor not available");
		return -ENODEV;
	}

	struct sensor_value val;
	int ret = sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);
	if (ret) {
		shell_error(sh, "channel_get failed: %d", ret);
		return ret;
	}

	shell_print(sh, "read OK - LED off, val=%d.%06d", val.val1, val.val2);
	return 0;
}

static int cmd_sensor_info(const struct shell *sh, size_t argc, char **argv)
{
	const struct device *dev = get_our_driver();

	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	shell_print(sh, "Device name : %s", dev->name);
	shell_print(sh, "Device ready: %s", device_is_ready(dev) ? "YES" : "NO");
	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_sensor,
	SHELL_CMD(fetch, NULL, "Fetch sensor sample (LED ON)", cmd_sensor_fetch),
	SHELL_CMD(read,  NULL, "Read sensor channel (LED OFF)", cmd_sensor_read),
	SHELL_CMD(info,  NULL, "Print device info", cmd_sensor_info),
	SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &sub_sensor, "Our driver commands", NULL);
