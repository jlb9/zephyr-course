# Lesson 7 — Task 1: Shell commands for your custom sensor driver

**Course:** [Iomico Zephyr Training](https://github.com/iomico-public/zephyr-course)  
**Upstream repo:** [iomico-public/zephyr-course](https://github.com/iomico-public/zephyr-course)  
**Forks:** [View all course forks](https://github.com/iomico-public/zephyr-course/forks)  
**Previous task:** [l6-task2 — Custom extension API on driver](../l6-task2/README.md)

---

## Overview

In **l6-task1** and **l6-task2** you built a custom driver exposed through the **Sensor API** (and optionally a custom extension API).

In **l7-task1** you add a **Zephyr Shell** command tree so you can control and inspect that driver from the serial console — without recompiling or reflashing. This is a common pattern in production firmware: drivers stay behind a stable API, and the shell gives developers and testers a safe runtime interface.

**This task does not require a new driver or binding.** You extend the application on top of your existing l6 work.

---

## Official requirements

1. **Enable the Zephyr shell** in Kconfig (`CONFIG_SHELL=y`, plus any backend options your board needs).
2. **Register a root shell command** (recommended name: `sensor`) with at least **three subcommands**:
   - **`fetch`** — call `sensor_sample_fetch()` on your LED driver (turn LED **on** in a GPIO-based driver).
   - **`read`** — call `sensor_channel_get()` on the same device (turn LED **off**).
   - **`info`** — print whether the device is ready (device name and/or `device_is_ready()`).
3. **Wire the shell to your l6 driver** using `DEVICE_DT_GET()` / devicetree aliases — not raw GPIO from the application.
4. **Stop driving the LED from a tight blink loop in `main`.** The shell is the primary user interface; `main` may idle or perform minimal setup only.

---

## Prerequisites

- Completed **l6-task1** (`l6-task1` tag): custom sensor driver, binding, overlay, and app using `sensor_sample_fetch()` / `sensor_channel_get()`.
- Completed **l6-task2** (`l6-task2` tag) is recommended if your driver uses an extended API, but not strictly required for this task.
- Serial console works (`west attach` or your usual UART tool).
- Driver builds and runs on your board (for example `nrf52840dk/nrf52840`).

---

## Learning objectives

After completing this homework you should be able to:

1. Enable and use the [Zephyr Shell](https://docs.zephyrproject.org/latest/services/shell/index.html) subsystem.
2. Register commands with `SHELL_CMD_REGISTER()` and subcommands with `SHELL_STATIC_SUBCMD_SET_CREATE()`.
3. Implement shell handlers that call existing driver APIs (`sensor_sample_fetch`, `sensor_channel_get`).
4. Resolve devices at compile time with devicetree aliases (`DT_ALIAS()`).
5. Submit work using the course fork → branch → tag → pull request workflow.

---

## Task requirements (detailed)

### 1. Enable the shell

In `prj.conf` (minimum):

```ini
CONFIG_SHELL=y
CONFIG_LOG=y
CONFIG_SENSOR=y
CONFIG_<YOUR_DRIVER_KCONFIG>=y
```

Most boards also need a shell backend, for example:

```ini
CONFIG_UART_CONSOLE=y
CONFIG_SHELL_BACKEND_SERIAL=y
```

If you use RTT on Nordic boards, enable the RTT shell backend instead. Match whatever console backend you already use for logging.

### 2. Devicetree — target device for shell commands

Obtain your driver with a devicetree alias so the application does not hard-code node labels.

**Option A — single LED (minimum):**

```dts
/ {
    aliases {
        led = &our_driver0;   /* or your node label */
    };
};
```

**Option B — three LEDs (recommended; matches course reference):**

Instantiate three nodes with your driver `compatible`, then alias them:

```dts
/ {
    our_led0: our-led0 {
        compatible = "iomico,our-led";
        gpios = <&gpio0 13 GPIO_ACTIVE_LOW>;
        status = "okay";
    };

    our_led1: our-led1 {
        compatible = "iomico,our-led";
        gpios = <&gpio0 14 GPIO_ACTIVE_LOW>;
        status = "okay";
    };

    our_led2: our-led2 {
        compatible = "iomico,our-led";
        gpios = <&gpio0 15 GPIO_ACTIVE_LOW>;
        status = "okay";
    };

    aliases {
        ledr = &our_led0;
        ledg = &our_led1;
        ledb = &our_led2;
    };
};
```

Adjust GPIO pins for **your** board. On `nrf52840dk/nrf52840`, onboard LEDs are typically on `gpio0` pins 13–16.

For the minimum task, shell handlers may target **one** device (for example `ledr` or `led`). Option B prepares you for later tasks that control multiple instances.

### 3. Shell command handlers

Include `<zephyr/shell/shell.h>` and implement handlers with this signature:

```c
static int cmd_sensor_fetch(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    int ret = sensor_sample_fetch(led_dev);
    if (ret < 0) {
        shell_error(sh, "fetch failed: %d", ret);
        return ret;
    }
    shell_print(sh, "LED ON");
    return 0;
}
```

Implement similar handlers for **`read`** (`sensor_channel_get`) and **`info`** (`device_is_ready()` and/or `dev->name`).

Use `shell_print()` / `shell_error()` for user-visible output — not only `LOG_INF()`, so feedback appears in the shell session.

### 4. Register the command tree

```c
SHELL_STATIC_SUBCMD_SET_CREATE(sub_sensor,
    SHELL_CMD(fetch, NULL, "Turn LED on (sample_fetch)",  cmd_sensor_fetch),
    SHELL_CMD(read,  NULL, "Turn LED off (channel_get)", cmd_sensor_read),
    SHELL_CMD(info,  NULL, "Show device status",         cmd_sensor_info),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &sub_sensor, "Custom sensor driver commands", NULL);
```

Subcommand names may vary slightly (for example `channel_get` instead of `read`) as long as behavior matches the requirements above. Keep help strings accurate.

### 5. Application `main`

Replace the l6 heartbeat blink loop with shell-driven control:

```cpp
#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/shell/shell.h>

static const struct device *led_dev = DEVICE_DT_GET(DT_ALIAS(ledr));  /* or DT_ALIAS(led) */

int main(void)
{
    if (!device_is_ready(led_dev)) {
        return 1;
    }

    /* Shell runs in its own thread; main can idle */
    while (1) {
        k_sleep(K_FOREVER);
    }
    return 0;
}

/* ... shell handlers and SHELL_CMD_REGISTER above or below main ... */
```

Handlers and `SHELL_CMD_REGISTER` may live in `main.cpp` or a separate source file added to `CMakeLists.txt`.

### 6. Keep l6 driver behavior intact

Your shell commands must call the **Sensor API**, which dispatches into your l6 driver:

| Shell command | Sensor API | Typical driver action |
|---------------|------------|------------------------|
| `sensor fetch` | `sensor_sample_fetch()` | LED on |
| `sensor read` | `sensor_channel_get()` | LED off |
| `sensor info` | (app checks `device_is_ready()`) | Print status |

Do **not** bypass the driver with direct `gpio_pin_set()` calls from shell handlers unless your instructor explicitly allows it.

---

## Minimal file checklist

| File | Change |
|------|--------|
| `src/main.cpp` (or `main.c`) | Shell handlers, `SHELL_CMD_REGISTER`, idle `main` |
| `src/app.overlay` | Alias for target device (`led`, `ledr`, …) |
| `prj.conf` | `CONFIG_SHELL=y`, console/backend options |
| `CMakeLists.txt` | Only if you add a separate `shell_commands.c` |

**Not required for l7-task1:** new driver code, new binding YAML, custom board, or changes to l6-task2 extension API (unless you want `info` to print the dynamic label — optional).

---

## Build and test

```bash
cd app
west build -p always -b nrf52840dk/nrf52840
west flash
west attach
```

At the shell prompt (often `uart:~$` or `shell:~$`):

```text
uart:~$ sensor
  fetch  : Turn LED on (sample_fetch)
  read   : Turn LED off (channel_get)
  info   : Show device status

uart:~$ sensor fetch
LED ON

uart:~$ sensor read
LED OFF

uart:~$ sensor info
our-led0: ready
```

Press **Tab** to autocomplete command names. Use `sensor --help` if your Zephyr version supports it.

### Expected behavior

- Shell accepts `sensor fetch`, `sensor read`, and `sensor info`.
- `fetch` / `read` change LED state through the sensor driver (visible on hardware or in driver logs).
- `info` reports device readiness.
- No automatic blink loop running in parallel unless your instructor asks for both.

---

## Submission workflow

Follow the same process used by other students in the [course forks list](https://github.com/iomico-public/zephyr-course/forks):

1. **Fork** [iomico-public/zephyr-course](https://github.com/iomico-public/zephyr-course) (if you have not already).

2. **Branch:**
   ```bash
   git checkout -b feat/l7-task1
   ```

3. **Implement** shell commands on top of your l6 driver and verify on hardware.

4. **Commit**, for example:
   ```text
   feat: add shell commands for custom sensor driver (l7-task1)
   ```

5. **Tag and push:**
   ```bash
   git tag l7-task1
   git push origin feat/l7-task1
   git push origin l7-task1
   ```

6. **Open a Pull Request** to `iomico-public/zephyr-course` from `feat/l7-task1`.

### Pull request template

```markdown
## Summary
Lesson 7 Task 1 — Zephyr Shell commands for the custom sensor driver.

## Changes
- Enabled CONFIG_SHELL and serial (or RTT) shell backend
- Added devicetree alias for shell target device
- Registered `sensor` command with `fetch`, `read`, and `info` subcommands
- Replaced blink loop with shell-driven control

## Board
- Built for: `nrf52840dk/nrf52840` (replace with yours)

## Verification
- [ ] `west build` succeeds
- [ ] Shell prompt appears on serial console
- [ ] `sensor fetch` turns LED on via driver
- [ ] `sensor read` turns LED off via driver
- [ ] `sensor info` prints device status
- [ ] Tag `l7-task1` pushed

## Tag
l7-task1
```

---

## Acceptance checklist

| # | Criterion | Done |
|---|-----------|------|
| 1 | `CONFIG_SHELL=y` (and working backend) | ☐ |
| 2 | Root command registered (e.g. `sensor`) | ☐ |
| 3 | Subcommand calls `sensor_sample_fetch()` | ☐ |
| 4 | Subcommand calls `sensor_channel_get()` | ☐ |
| 5 | Subcommand prints device info / readiness | ☐ |
| 6 | Uses `DEVICE_DT_GET` + devicetree alias | ☐ |
| 7 | No tight blink loop in `main` | ☐ |
| 8 | `west build` clean for your board | ☐ |
| 9 | Tag `l7-task1` pushed; PR opened | ☐ |

---

## Common mistakes (seen in forks)

| Mistake | Symptom | Fix |
|---------|---------|-----|
| Shell enabled but no backend | No prompt on UART | Add `CONFIG_SHELL_BACKEND_SERIAL=y` (or RTT) |
| Handler uses `LOG_INF` only | Nothing in shell output | Use `shell_print()` / `shell_error()` |
| Direct GPIO in shell handler | Bypasses driver | Call `sensor_sample_fetch` / `sensor_channel_get` |
| `SHELL_CMD_REGISTER` missing | Command not listed on Tab | Register root + subcommand set |
| Wrong alias | Build error or wrong device | Match overlay alias to `DT_ALIAS(...)` |
| Blink loop still running | Shell works but LED toggles alone | Remove or disable l6 heartbeat loop |
| Stale build after `prj.conf` change | Shell not linked in | `west build -p always` |

---

## Example implementations in course forks

These forks have pushed an `l7-task1` tag (useful for comparison — write your own solution):

| Fork | Notes |
|------|-------|
| [carlassaraf/zephyr-course @ l7-task1](https://github.com/carlassaraf/zephyr-course/tree/l7-task1/l7-task1) | Reference: three aliases (`ledr`, `ledg`, `ledb`), `sensor fetch/read/info` |
| [Juanferreyra412/zephyr-course @ l7-task1](https://github.com/Juanferreyra412/zephyr-course/tree/l7-task1) | Single alias `led`, subcommands `fetch`, `channel_get`, `info` |
| [mfelder-nxt/zephyr-course @ l7-task1](https://github.com/mfelder-nxt/zephyr-course/tree/l7-task1) | Tagged submission |

Browse [all forks](https://github.com/iomico-public/zephyr-course/forks) for more examples.

---

## Optional extensions (not required)

- Add subcommands for `ledg` and `ledb` when using three instances.
- Print the l6-task2 dynamic label in `sensor info`.
- Add `sensor label <text>` that calls your custom extension API (`our_driver_set_label`).
- Move shell code to `src/shell_commands.c` for cleaner `main`.

---

## References

- [Zephyr Shell subsystem](https://docs.zephyrproject.org/latest/services/shell/index.html)
- [Zephyr Sensor API](https://docs.zephyrproject.org/latest/hardware/peripherals/sensor.html)
- [Devicetree aliases](https://docs.zephyrproject.org/latest/build/dts/intro.html#aliases-and-chosen-nodes)
- [l6-task1 homework](../l6-task1/README.md)
- [l6-task2 homework](../l6-task2/README.md)
- Course reference: [carlassaraf/zephyr-course @ l7-task1](https://github.com/carlassaraf/zephyr-course/tree/l7-task1/l7-task1)

---

## What comes next

**Lesson 7, Task 2 (`l7-task2`):** typically extends shell integration (for example additional commands, multiple devices, or testing). Check the course repo or instructor materials when `homework/l7-task2/README.md` is published.
