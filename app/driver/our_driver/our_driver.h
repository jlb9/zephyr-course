#ifndef OUR_DRIVER_H_
#define OUR_DRIVER_H_

#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

#ifdef __cplusplus
extern "C" {
#endif

struct our_driver_api {
	struct sensor_driver_api sensor;
	int (*set_label)(const struct device *dev, const char *label);
};

static inline int our_driver_set_label(const struct device *dev, const char *label)
{
	return ((const struct our_driver_api *)dev->api)->set_label(dev, label);
}

#ifdef __cplusplus
}
#endif

#endif /* OUR_DRIVER_H_ */
