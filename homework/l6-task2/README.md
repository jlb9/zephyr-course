# Lesson 6 — Task 2: Custom extension API on your driver

**Course:** [Iomico Zephyr Training](https://github.com/iomico-public/zephyr-course)  
**Upstream repo:** [iomico-public/zephyr-course](https://github.com/iomico-public/zephyr-course)  
**Forks:** [View all course forks](https://github.com/iomico-public/zephyr-course/forks)  
**Previous task:** [l6-task1 — Custom driver (Sensor API)](../l6-task1/README.md)

---

## Overview

In **l6-task1** you registered a custom driver using Zephyr’s standard **Sensor API** (`sample_fetch`, `channel_get`).

In **l6-task2** you add a **custom extension API** — extra function(s) on your driver that are **not** part of `struct sensor_driver_api`. The application calls these directly to read or change **runtime (dynamic) data** stored in the driver’s `data` struct.

**This task does not require a custom board.** Keep building on the same board and devicetree setup from l6-task1.

---

## Official requirements

1. **Add a custom extension API function** to your driver.
2. **Call it from `main.c`** (or `main.cpp` in this course repo).
3. The function must **change a parameter of your choosing** in the driver’s **dynamic data** struct (`dev->data`).

---

## Prerequisites

- Completed **l6-task1** (`l6-task1` tag): binding, overlay node, driver, and app using `sensor_sample_fetch()` / `sensor_channel_get()`.
- Driver still builds and runs on your board (for example `nrf52840dk/nrf52840`).

---

## Learning objectives

1. Separate **compile-time config** (`struct ..._config` / `dev->config`) from **runtime data** (`struct ..._data` / `dev->data`).
2. Extend the driver API beyond the Sensor class with a custom struct and function pointers.
3. Expose a small **public header** with `static inline` helpers for the application.
4. Call the extension API from application code and verify the change (for example via log output).

---

## Task requirements (detailed)

### 1. Add a dynamic data struct

If you do not have one yet, add runtime storage per device instance. Example:

```c
struct our_driver_data {
    bool led_on;
    char label[16];           /* example: parameter you will change */
    uint32_t toggle_count;    /* another example */
};
```

Wire it into `SENSOR_DEVICE_DT_INST_DEFINE()` as the **data** pointer (fourth argument), with a matching `static struct our_driver_data our_driver_data_##inst` per instance.

### 2. Define a custom API struct

Embed the Sensor API as the **first member**, then add your extension(s):

```c
typedef int (*our_driver_set_label_t)(const struct device *dev, const char *label);

struct our_driver_api {
    struct sensor_driver_api sensor;
    our_driver_set_label_t set_label;
};
```

Implement **one** function (for example `set_label`) to **write into `dev->data`** — e.g. copy a string into `data->label`. A separate `get_*` helper is optional, not required.

### 3. Small public header (recommended)

Keep it minimal — one extension pointer and one inline wrapper is enough:

```c
struct our_driver_api {
    struct sensor_driver_api sensor;
    int (*set_label)(const struct device *dev, const char *label);
};

static inline int our_driver_set_label(const struct device *dev, const char *label)
{
    return ((const struct our_driver_api *)dev->api)->set_label(dev, label);
}
```

Register the device with the extended API table cast to `const struct sensor_driver_api *`.

### 4. Call from `main.cpp`

In `src/main.cpp` (course uses C++; slides may say `main.c`):

```cpp
#include "our_driver.h"   /* or correct include path */

const struct device *driver = DEVICE_DT_GET(DT_NODELABEL(our_driver0));

our_driver_set_label(driver, "heartbeat-led");
```

Keep your existing l6-task1 sensor loop unless your instructor says otherwise.

### 5. Verify

Prove the dynamic field changed — logging inside `set_label` (for example `LOG_INF("label is now %s", data->label)`) is sufficient. You do **not** need a second getter API unless you want it.

---

## Minimal file checklist

| File | Change |
|------|--------|
| `driver/our_driver/our_driver.c` | `our_driver_data`, extended API, implementation |
| `driver/our_driver/our_driver.h` | `static inline` wrapper(s) for app |
| `driver/our_driver/CMakeLists.txt` | Add header to include path if needed |
| `src/main.cpp` | Include header + call extension API |
| `prj.conf` | No new options required if l6-task1 already works |

**Not required for l6-task2:** new `boards/` folder, `BOARD_ROOT`, or board `.dts` changes.

---

## Build and test

```bash
cd app
west build -p always -b nrf52840dk/nrf52840
west flash
west attach
```

You should see:

- l6-task1 driver init and sensor API logs (unchanged behavior).
- A new log line showing your extension API ran and the dynamic field was updated.

---

## Submission workflow

1. Branch: `git checkout -b feat/l6-task2`
2. Implement extension API + `main` call on top of l6-task1.
3. Tag and push:
   ```bash
   git tag l6-task2
   git push origin feat/l6-task2
   git push origin l6-task2
   ```
4. Open a PR to [iomico-public/zephyr-course](https://github.com/iomico-public/zephyr-course).

### Pull request template

```markdown
## Summary
Lesson 6 Task 2 — custom extension API on our_driver.

## Changes
- Added `our_driver_data` runtime struct
- Extended driver API with `<your_function>` that updates `<your_field>`
- Added `our_driver.h` with inline helper
- Called extension API from `src/main.cpp`

## Verification
- [ ] `west build` succeeds
- [ ] Extension API called from main
- [ ] Dynamic data field changes (visible in log)
- [ ] l6-task1 sensor loop still works
- [ ] Tag `l6-task2` pushed
```

---

## Acceptance checklist

| # | Criterion | Done |
|---|-----------|------|
| 1 | Driver has a per-instance **data** struct (`dev->data`) | ☐ |
| 2 | At least one **custom** API function (not Sensor standard) | ☐ |
| 3 | That function **modifies** a field in the data struct | ☐ |
| 4 | Application calls the extension API from `main` | ☐ |
| 5 | Change is observable (log or getter) | ☐ |
| 6 | l6-task1 build still succeeds | ☐ |
| 7 | Tag `l6-task2` pushed; PR opened | ☐ |

---

## Common mistakes

| Mistake | Fix |
|---------|-----|
| Only changing `dev->config` | Config is read-only at runtime; use **`dev->data`** |
| No data pointer in `SENSOR_DEVICE_DT_INST_DEFINE` | Pass `&our_driver_data_##inst` as data argument |
| Extended API not first member | Put `struct sensor_driver_api sensor` **first** in your API struct |
| App casts to `sensor_driver_api` only | Cast to **your** `struct our_driver_api` in inline helpers |
| Extension never called from main | Required — wire the call in `main.cpp` |

---

## Example ideas for the dynamic field

Pick one (or your own):

| Field | Extension API | Effect |
|-------|---------------|--------|
| `char label[16]` | `set_label(dev, "foo")` | Store a name string |
| `uint32_t toggle_count` | `reset_count(dev)` | Zero the counter |
| `bool led_on` | `force_off(dev)` | Set state without sensor call |
| `uint8_t brightness` | `set_brightness(dev, n)` | Store level for next GPIO write |

---

## References

- [Zephyr device model](https://docs.zephyrproject.org/latest/kernel/drivers/index.html) — `config` vs `data`
- [Sensor driver API](https://docs.zephyrproject.org/latest/hardware/peripherals/sensor.html)
- [l6-task1 homework](../l6-task1/README.md)
- Optional advanced fork (also adds custom board): [carlassaraf/zephyr-course @ l6-task2](https://github.com/carlassaraf/zephyr-course/tree/l6-task2/l6-task2)

---

## What comes next

**Lesson 7, Task 1 (`l7-task1`):** see [homework/l7-task1/README.md](../l7-task1/README.md) — add **Zephyr Shell** commands (`sensor fetch`, `sensor read`, `sensor info`) that control your l6 driver from the serial console.

A later lesson may also cover **custom board ports** (`boards/`, `BOARD_ROOT`). That is **outside** the core l6-task2 requirements above unless your instructor assigns it separately.
