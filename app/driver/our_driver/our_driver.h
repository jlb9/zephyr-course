#ifndef OUR_DRIVER_H_
#define OUR_DRIVER_H_

#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

#ifdef __cplusplus
extern "C" {
#endif

struct our_driver_data {
	char label[16];
	char message[64];
	int toggle_count;
	bool led_on;
};

struct our_driver_api {
	struct sensor_driver_api sensor;
	int (*set_label)(const struct device *dev, const char *label);
	int (*set_message)(const struct device *dev, const char *msg);
	int (*set_toggle_count)(const struct device *dev, int count);
};

static inline int our_driver_set_label(const struct device *dev, const char *label)
{
	return ((const struct our_driver_api *)dev->api)->set_label(dev, label);
}

static inline int our_driver_set_message(const struct device *dev, const char *msg)
{
	return ((const struct our_driver_api *)dev->api)->set_message(dev, msg);
}

static inline int our_driver_set_toggle_count(const struct device *dev, int count)
{
	return ((const struct our_driver_api *)dev->api)->set_toggle_count(dev, count);
}

#ifdef __cplusplus
}
#endif

#endif /* OUR_DRIVER_H_ */
