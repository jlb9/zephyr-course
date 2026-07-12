#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

// shell commands 'msense'
// msense : shows help
// msense temp (optional C or F)
// msense hum

static int cmd_msense_temp(const struct shell *shell, size_t argc, char **argv) {
    const struct device *bme680 = DEVICE_DT_GET_ONE(bosch_bme680);
    struct sensor_value temperature;
    sensor_sample_fetch(bme680);
    sensor_channel_get(bme680, SENSOR_CHAN_AMBIENT_TEMP, &temperature);

    if (argc == 1) {
        shell_print(shell, "Temperature: %d.%02d C\n", temperature.val1, temperature.val2 / 10000);        // default is Celsius
        return 0;
    }
    if (argc == 2) {
        if (strcmp(argv[1], "F") == 0) {
            temperature.val1 = temperature.val1 * 9 / 5 + 32;
            temperature.val2 = temperature.val2 * 9 / 5;
            shell_print(shell, "Temperature: %d.%02d F\n", temperature.val1, temperature.val2 / 10000);
            return 0;
        } else if (strcmp(argv[1], "C") == 0) {
            shell_print(shell, "Temperature: %d.%02d C\n", temperature.val1, temperature.val2 / 10000);
            return 0;
        } else {
            shell_print(shell, "Invalid argument. Use 'C' for Celsius or 'F' for Fahrenheit.");
            return -1;
        }
    }
    if (argc > 2) {
        shell_print(shell, "Too many arguments. Use 'msense temp [C|F]'.");
        return -1;
    }
}

    static int cmd_msense_hum(const struct shell *shell, size_t argc, char **argv) {
        const struct device *bme680 = DEVICE_DT_GET_ONE(bosch_bme680);
        struct sensor_value humidity;
        sensor_sample_fetch(bme680);
        sensor_channel_get(bme680, SENSOR_CHAN_HUMIDITY, &humidity);
        shell_print(shell, "Humidity: %d.%02d %%\n", humidity.val1, humidity.val2 / 10000);
        return 0;
    }

    SHELL_STATIC_SUBCMD_SET_CREATE(sub_msense,
                                   SHELL_CMD(temp, NULL, "Get temperature", cmd_msense_temp),
                                   SHELL_CMD(hum, NULL, "Get humidity", cmd_msense_hum),
                                   SHELL_SUBCMD_SET_END);

    static int cmd_msense(const struct shell *shell, size_t argc, char **argv) {
        shell_print(shell, "Use 'msense temp [C|F]' or 'msense hum' to read sensor data.");
        return 0;
    }

    SHELL_CMD_REGISTER(msense, &sub_msense, "MSense commands", cmd_msense);

    int main(void) {
        const struct device *bme680 = DEVICE_DT_GET_ONE(bosch_bme680);

        if (!device_is_ready(bme680)) {
            printk("BME680 not ready\n");
            return -1;
        }

        while (true) {
            k_msleep(1000);
        }
        return 0;
    }
