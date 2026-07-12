#ifndef LED_SENSOR_H_
#define LED_SENSOR_H_

#include <zephyr/drivers/sensor.h>

struct led_sensor_driver_api {
	struct sensor_driver_api sensor;
	int (*set_blink_count)(const struct device *dev, uint32_t count);
};

static inline int led_sensor_set_blink_count(const struct device *dev,
uint32_t count)
{
	const struct led_sensor_driver_api *api =
		(const struct led_sensor_driver_api *)dev->api;

	return api->set_blink_count(dev, count);
}

#endif /* LED_SENSOR_H_ */