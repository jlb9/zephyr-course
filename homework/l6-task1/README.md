# Lesson 6 — Task 1: Custom driver via the Sensor API

**Course:** [Iomico Zephyr Training](https://github.com/iomico-public/zephyr-course)  
**Upstream repo:** [iomico-public/zephyr-course](https://github.com/iomico-public/zephyr-course)  
**Forks:** [View all course forks](https://github.com/iomico-public/zephyr-course/forks)

---

## Overview

In this task you implement a **custom out-of-tree driver** that controls an LED using Zephyr’s **Sensor driver API** (`sensor_sample_fetch()` / `sensor_channel_get()`), even though the hardware is a simple GPIO.

This pattern is common in Zephyr courses and production code: a familiar application API hides board-specific details behind devicetree and a driver module.

**Reference scope (Task 1 only):** driver + binding + overlay + application.  
**Not in this task:** custom board definitions (that is **l6-task2**).

---

## Prerequisites

- Completed **Lesson 5, Task 2** (`l5-task2` tag on your fork), or equivalent working `app/` build.
- A fork of [iomico-public/zephyr-course](https://github.com/iomico-public/zephyr-course) under your GitHub account.
- Zephyr workspace builds successfully (`west build` from `app/` or your task directory).
- Basic familiarity with:
  - [Devicetree overlays](https://docs.zephyrproject.org/latest/build/dts/intro.html)
  - [Out-of-tree bindings](https://docs.zephyrproject.org/latest/build/dts/api/bindings.html)
  - [Sensor driver API](https://docs.zephyrproject.org/latest/hardware/peripherals/sensor.html)

---

## Learning objectives

After completing this homework you should be able to:

1. Define a devicetree binding with a custom `compatible` string.
2. Instantiate driver devices from devicetree using `DT_DRV_COMPAT` and `DT_INST_FOREACH_STATUS_OKAY()`.
3. Register a driver with `SENSOR_DEVICE_DT_INST_DEFINE()`.
4. Map `sample_fetch` / `channel_get` callbacks to hardware actions (LED on / off).
5. Use `DEVICE_DT_GET()` / `DT_ALIAS()` from application code.
6. Submit work using the course fork → branch → tag → pull request workflow.

---

## Task requirements

### 1. Devicetree binding

Create a binding file, for example:

`app/dts/bindings/sensor/iomico,our-led.yaml`  
(or `modules/dts/bindings/driver/custom_led.yaml` if you use a Zephyr module)

Minimum content:

- `compatible: "iomico,our-led"` (you may use your own vendor prefix, but keep it consistent everywhere)
- `include: sensor-device.yaml` **or** `base.yaml` plus a required `gpios` property if you drive a real GPIO pin

### 2. Devicetree overlay

Add at least **one** device node with `status = "okay"`, for example:

```dts
/ {
    our_led0: our-led {
        compatible = "iomico,our-led";
        gpios = <&gpio0 13 GPIO_ACTIVE_LOW>;   /* adjust for your board */
        status = "okay";
    };

    aliases {
        our-led = &our_led0;
    };
};
```

Use GPIO lines that are valid on **your** board. On `nrf52840dk/nrf52840`, onboard LEDs are typically on `gpio0` pins 13–16.

### 3. Driver implementation

Implement a driver (C file) that:

| Sensor API call | Required behavior |
|-----------------|-------------------|
| `sample_fetch()` | Turn the LED **on** (or active state) |
| `channel_get()` | Turn the LED **off** (or inactive state) |

Also implement:

- `DT_DRV_COMPAT` matching your binding (`iomico_our_led` for `iomico,our-led`)
- Device init: configure GPIO as output (if using GPIO)
- `SENSOR_DEVICE_DT_INST_DEFINE()` + `DT_INST_FOREACH_STATUS_OKAY()`
- Log module registration (`LOG_MODULE_REGISTER`)

Enable the driver in Kconfig (`CONFIG_*` symbol tied to `DT_HAS_..._ENABLED`).

### 4. Build system integration

Wire the driver into the build:

- **Option A — in-tree under `app/`** (recommended if you continued Lessons 2–5 in `app/`):
  - `list(APPEND DTS_ROOT ${CMAKE_CURRENT_SOURCE_DIR})`
  - `set(DTC_OVERLAY_FILE ...)` if overlay is not named `app.overlay` at app root
  - `add_subdirectory(driver/our_led)` (or similar)
  - `rsource` in `app/Kconfig`

- **Option B — Zephyr module under `l6-task1/modules/`** (see [reference solution](https://github.com/carlassaraf/zephyr-course/tree/l6-task1/l6-task1)):
  - `zephyr/module.yml` with `dts: dirs: [dts/bindings]`
  - `list(APPEND ZEPHYR_EXTRA_MODULES ...)` and `list(APPEND DTS_ROOT ...)` in `CMakeLists.txt`

### 5. Application

Update `src/main.cpp` (or equivalent) to:

1. Obtain the device with `DEVICE_DT_GET(DT_ALIAS(our_led))` or `DT_NODELABEL(our_led0)`.
2. Check `device_is_ready()` before use.
3. In a loop:
   - call `sensor_sample_fetch()` → LED on
   - sleep `CONFIG_APP_HEARTBEAT_PERIOD_MS` (or `CONFIG_BLINKY_SLEEP_MS`)
   - call `sensor_channel_get()` → LED off
   - sleep again
4. Log return codes or state for verification.

### 6. Configuration

In `prj.conf` (minimum):

```ini
CONFIG_GPIO=y
CONFIG_LOG=y
CONFIG_SENSOR=y
CONFIG_<YOUR_DRIVER_KCONFIG>=y
```

---

## Suggested project layout (Option A)

```text
app/
├── CMakeLists.txt          # DTS_ROOT, overlay, driver subdirectory
├── Kconfig                 # rsource driver Kconfig
├── prj.conf
├── dts/bindings/sensor/iomico,our-led.yaml
├── driver/our_led/
│   ├── CMakeLists.txt
│   ├── Kconfig
│   └── our_led.c
└── src/
    ├── app.overlay
    └── main.cpp
```

---

## Build and test

From your application directory:

```bash
# Nordic DK example
west build -p always -b nrf52840dk/nrf52840

# Or your own board / custom board from earlier lessons
west build -p always -b <your_board>
```

Flash and attach the serial console:

```bash
west flash
west attach   # or your usual UART tool
```

### Expected console output

You should see logs similar to:

```text
[inf] our_led: Device Initialized
[inf] our_led: Hello From Sample Fetch, channel ...
[inf] main: Channel ret: 0
[inf] our_led: Hello From Channel Get, channel ...
[inf] main: Channel ret: 0
```

The onboard LED (or wired GPIO) should toggle on each heartbeat period.

---

## Submission workflow (required)

Follow the same process used by other students in the [course forks list](https://github.com/iomico-public/zephyr-course/forks):

1. **Fork** [iomico-public/zephyr-course](https://github.com/iomico-public/zephyr-course) to your GitHub account (if you have not already).

2. **Clone your fork** and create a branch:
   ```bash
   git checkout -b feat/l6-task1
   ```

3. **Implement** all requirements above and verify build + runtime behavior.

4. **Commit** with a clear message, for example:
   ```text
   feat: custom LED driver using sensor API (l6-task1)
   ```

5. **Tag** the submission commit:
   ```bash
   git tag l6-task1
   git push origin feat/l6-task1
   git push origin l6-task1
   ```

6. **Open a Pull Request** from your fork to `iomico-public/zephyr-course` on branch `feat/l6-task1`.

### Pull request template

Use this structure in the PR description (adapt paths to your layout):

```markdown
## Summary
Lesson 6 Task 1 — custom driver exposed through the Zephyr Sensor API.

## Changes
- Added devicetree binding `iomico,our-led`
- Added devicetree overlay with `our_led0` node and alias
- Implemented driver with `sample_fetch` (LED on) and `channel_get` (LED off)
- Updated application to use `DEVICE_DT_GET` + sensor API calls
- Enabled `CONFIG_SENSOR` and driver Kconfig options in `prj.conf`

## Board
- Built for: `nrf52840dk/nrf52840` (replace with yours)

## Verification
- [ ] `west build` succeeds without errors
- [ ] `west flash` runs on hardware
- [ ] Serial log shows driver init + fetch/channel messages
- [ ] LED toggles with configured sleep period

## Tag
l6-task1
```

---

## Acceptance checklist (self-grade before opening PR)

| # | Criterion | Done |
|---|-----------|------|
| 1 | Custom `compatible` string in binding YAML | ☐ |
| 2 | Overlay node has `status = "okay"` | ☐ |
| 3 | Driver builds only when devicetree node is present | ☐ |
| 4 | `sample_fetch` turns LED on | ☐ |
| 5 | `channel_get` turns LED off | ☐ |
| 6 | Application uses Sensor API (not raw GPIO toggle) | ☐ |
| 7 | `west build` clean for your board | ☐ |
| 8 | Tag `l6-task1` pushed to your fork | ☐ |
| 9 | PR opened against `iomico-public/zephyr-course` | ☐ |

---

## Common mistakes (seen in forks / CI)

| Mistake | Symptom | Fix |
|---------|---------|-----|
| Wrong board name | `Invalid BOARD` | Use `nrf52840dk/nrf52840`, not `nrf52840/nrf52480dk` |
| `compatible` in `aliases` block | Devicetree parse errors | Put `compatible` / `status` on the device node, not under `aliases` |
| Missing `DTS_ROOT` | Binding not found | `list(APPEND DTS_ROOT ${CMAKE_CURRENT_SOURCE_DIR})` before `find_package(Zephyr)` |
| `DEVICE_DT_GET` without DT node | `our_driver0_ORD was not declared` | Add overlay node + binding; rebuild with `-p always` |
| Kconfig path typo | `Kconfig not found` | Match folder name: `driver/` vs `drivers/` |
| Undefined init priority | Linker: `Undefined initialization levels` | Use `CONFIG_SENSOR_INIT_PRIORITY` in `SENSOR_DEVICE_DT_INST_DEFINE` |

---

## Optional extensions (not required)

- Drive **three** LEDs with aliases `ledr`, `ledg`, `ledb` (see [reference `l6-task1` overlay](https://github.com/carlassaraf/zephyr-course/blob/l6-task1/l6-task1/app.overlay)).
- Add `menuconfig` option for blink period (`CONFIG_BLINKY_SLEEP_MS`).
- Return real `sensor_value` data from `channel_get()`.
- Package the driver as a reusable Zephyr module (`zephyr/module.yml`).

---

## References

- [Zephyr Sensor API](https://docs.zephyrproject.org/latest/hardware/peripherals/sensor.html)
- [Device driver model](https://docs.zephyrproject.org/latest/kernel/drivers/index.html)
- [Devicetree bindings](https://docs.zephyrproject.org/latest/build/dts/api/bindings.html)
- [Zephyr module build (out-of-tree)](https://docs.zephyrproject.org/latest/guides/modules.html)
- Course reference implementation: [carlassaraf/zephyr-course @ l6-task1](https://github.com/carlassaraf/zephyr-course/tree/l6-task1/l6-task1)
- Sample: `zephyr/samples/sensor/sensor_shell` (`vnd,fake-sensor` binding)

---

## What comes next

**Lesson 6, Task 2 (`l6-task2`):** see [homework/l6-task2/README.md](../l6-task2/README.md) — add a **custom extension API** on your driver, call it from `main`, and change a field in the driver’s dynamic data struct. Custom board work is **not** required for l6-task2.
