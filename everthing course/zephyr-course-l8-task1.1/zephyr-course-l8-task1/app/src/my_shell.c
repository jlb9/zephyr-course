#include <string.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/shell/shell.h>

static const struct device *const sensor_dev =
    DEVICE_DT_GET(DT_NODELABEL(led_sensor));

static int cmd_sensor_fetch(const struct shell *sh, size_t argc, char **argv) {
    if (!device_is_ready(sensor_dev)) {
        shell_error(sh, "Sensor device not ready");
        return -ENODEV;
    }

    int ret = sensor_sample_fetch(sensor_dev);

    if (ret) {
        shell_error(sh, "sensor_sample_fetch failed: %d", ret);
        return ret;
    }

    shell_print(sh, "LED ON");
    return 0;
}

static int cmd_sensor_read(const struct shell *sh, size_t argc, char **argv) {
    if (!device_is_ready(sensor_dev)) {
        shell_error(sh, "Sensor device not ready");
        return -ENODEV;
    }

    struct sensor_value val;

    int ret = sensor_channel_get(sensor_dev, SENSOR_CHAN_ALL, &val);

    if (ret) {
        shell_error(sh, "sensor_channel_get failed: %d", ret);
        return ret;
    }

    shell_print(sh, "LED OFF - Count: %d", val.val1);

    return 0;
}


static int cmd_sensor_info(const struct shell *sh, size_t argc, char **argv) {
    if (argc < 2) {
        shell_error(sh, "Usage:\n"
                        "sensor info name  - Print driver name\n"
                        "sensor info state - Print driver state");

        return -EINVAL;
    }

    if (strcmp(argv[1], "name") == 0) {

        shell_print(sh, "Driver : %s", sensor_dev->name);

    } else if (strcmp(argv[1], "state") == 0) {

        shell_print(sh, "State  : %s",
                    device_is_ready(sensor_dev) ? "Ready" : "Not Ready");

    } else {

        shell_error(sh,
                    "Invalid argument: %s\n"
                    "Valid args: name | state",
                    argv[1]);

        return -EINVAL;
    }

    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(
    sub_sensor,
    SHELL_CMD(set, NULL, "LED ON - From Sensor Driver", cmd_sensor_fetch),

    SHELL_CMD(get, NULL, "LED OFF - From Sensor Driver", cmd_sensor_read),

    SHELL_CMD_ARG(info, NULL, "Info - Driver Name & Status", cmd_sensor_info, 2, 0),

    SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(sensor, &sub_sensor, "My Shell Command", NULL);