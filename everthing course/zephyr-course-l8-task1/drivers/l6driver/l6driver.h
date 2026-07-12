#pragma once

#include <zephyr/device.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint32_t l6_get_sleep_ms(const struct device *dev);

#ifdef __cplusplus
}
#endif
