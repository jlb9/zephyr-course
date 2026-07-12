# Zephyr Course Project — History & Lecture Summary

This document summarizes work done in the **Iomico Zephyr Training** course (`zephyr-course` repo), reconstructed from git history, tags, source code, and homework READMEs. The course walks from a basic blinky app through Kconfig, devicetree, custom boards, out-of-tree drivers, shell integration, and automated testing with Twister.

**Upstream course:** [iomico-public/zephyr-course](https://github.com/iomico-public/zephyr-course)  
**Local repo:** `/home/jb/zeph-workspace/zephyr-course`  
**Workspace layout:** Zephyr sources under `deps/`, west manifest in `zephyr-manifest/west.yml`, application in `zephyr-course/app/`

---

## Course arc (high level)

| Phase | Lessons | Topics |
|-------|---------|--------|
| Setup | 1–2 | Zephyr workspace, west, blinky |
| Configuration | 3 | Kconfig / menuconfig |
| Hardware description | 4 | Devicetree overlays, GPIO via DT |
| Board support | 5 | Out-of-tree board definitions |
| Drivers | 6 | Custom sensor driver + extension API |
| Runtime interface | 7 | Zephyr Shell commands |
| Quality | 8 | ZTEST unit tests, Twister, code coverage |

Homework is submitted by tagging commits (`l6-task1`, `l7-task1`, etc.) and opening PRs to the upstream course repo.

---

## Git timeline

All dates below come from `zephyr-course` git log on `main`.

| Date | Commit / tag | Summary |
|------|--------------|---------|
| 2026-02-19 | `8456c11` | Course sandbox initialized with blinky sample + `west.yml` |
| 2026-03-02 | `c089635` | Zephyr pinned to **v4.2.0** |
| 2026-04-04 | `l2-task1` | Workspace reorganized; manifest moved to `zephyr-manifest/` |
| 2026-04-05 | `fff6115` | Lecture 3: initial Kconfig + `prj.conf` choices |
| 2026-04-09 | `l3-task1` | Kconfig expanded for menuconfig (heartbeat, advanced LED options) |
| 2026-04-21 | `3bfb52d` | Devicetree overlay created (`app.overlay`) |
| 2026-04-21 | `l4-task1` | Homework 4: GPIO blink via devicetree alias |
| 2026-05-04 | `l5-task1` | Custom board `our_board` scaffolded from nRF52840 DK |
| 2026-05-04 | `l5-task2` | Slimmer custom board `my_board` added |
| 2026-05-19 | `l6-task1` | Custom `iomico,our-driver` sensor driver |
| 2026-05-19 | `l6-task2` | Extension API on driver (`our_driver.h`) |
| 2026-05-31 | `l7-task1` | Shell subsystem + `sensor` command tree |
| 2026-05-31 | `l7-task2` | Extra shell subcommands with argument validation |
| 2026-05-31 | `l8-task1` | `ring_buf` module + ZTEST suite |
| 2026-05-31 | `l8-task2` | Twister run + coverage report (88.2% line coverage) |

**Tags on `main`:** `v1.0`, `l2-task1`, `l3-task1`, `l4-task1`, `l5-task1`, `l5-task2`, `l6-task1`, `l6-task2`, `l7-task1`, `l7-task2`, `l8-task1`, `l8-task2`

To inspect any milestone:

```bash
cd /home/jb/zeph-workspace/zephyr-course
git show l6-task1 --stat
git checkout l7-task1   # detached HEAD — read-only inspection
```

---

## Workspace structure

After Lecture 2 the project uses a multi-repo west workspace:

```
zeph-workspace/
├── deps/
│   ├── modules/          # HAL (Nordic, STM32, Espressif, CMSIS)
│   └── zephyr/           # Zephyr RTOS v4.2.0
├── zephyr-manifest/
│   └── west.yml          # west manifest (Zephyr + module imports)
├── zephyr-course/
│   ├── app/              # Main firmware application
│   ├── homework/         # Task instructions (L6–L7)
│   ├── tests/            # Twister/ZTEST tests (L8)
│   └── twister-out/      # Test & coverage artifacts (L8)
└── Documents/            # Personal notes (menuconfig, UART, this file)
```

Target hardware throughout most tasks: **nRF52840 DK** (`nrf52840dk/nrf52840` or custom `our_board` / `my_board`).

---

## Lecture 1 — Getting started (implicit)

**Git:** `8456c11` — *feat: init sandbox repo with blinky*  
**Tag:** none (baseline before `l2-task1`)

### What was done

- Cloned or initialized the course sandbox with a minimal Zephyr application.
- Added `app/src/main.cpp` — classic LED blink using GPIO.
- Added `app/prj.conf`, `app/CMakeLists.txt`, root `west.yml`, and `.gitignore`.
- README documents Zephyr 4.3.0 getting-started flow (later workspace uses 4.2.0 per manifest).

### Key takeaway

Build, flash, and verify blinky on real hardware before touching Kconfig or devicetree.

---

## Lecture 2 — Workspace setup

**Git:** `c8035e0` — *L2 task1: made workspace and moved manifest*  
**Tag:** `l2-task1` (also `v1.0`)

### Task 1 — West workspace

- Created the full west workspace under `/home/jb/zeph-workspace/`.
- Moved `west.yml` out of `zephyr-course/` into `zephyr-manifest/`.
- Documented directory layout in `README.md`.
- Minor tweak to `app/src/main.cpp`.

### Key takeaway

Separate **application repo** (`zephyr-course`) from **manifest repo** (`zephyr-manifest`) so Zephyr and HAL modules live in `deps/` and are managed by `west update`.

---

## Lecture 3 — Kconfig & menuconfig

**Git:** `fff6115` then `f9b7c99`  
**Tag:** `l3-task1`

### Task 1 — Application Kconfig

- Added `app/Kconfig` with an **LED Subsystem** menu:
  - `CONFIG_APP_HEARTBEAT_PERIOD_MS` — blink interval (100–2000 ms, default 500)
  - Nested **Advanced LED settings** (`ADVANCED_LED_SETTINGS`)
  - Brightness, fade duration, and expert-only verbose debug options
- Wired options into `prj.conf` and updated `main.cpp` to use `CONFIG_APP_HEARTBEAT_PERIOD_MS` instead of a hard-coded sleep.

### Key files

- `app/Kconfig`
- `app/prj.conf`
- Personal notes: `Documents/zephyr-menuconfig.md`

### Key takeaway

Application-specific options belong in app Kconfig; `west build -t menuconfig` edits the build-time `.config` without hand-editing `prj.conf` for every change.

---

## Lecture 4 — Devicetree

**Git:** `3bfb52d` (overlay) + `d096ef6` (homework)  
**Tag:** `l4-task1`

### Task 1 — Devicetree overlay (HW4)

- Created `app/app.overlay` defining an LED node and `app-led` alias.
- Refactored `main.cpp` from direct board GPIO macros to **devicetree GPIO API**:
  - `DT_ALIAS(app_led)` → `GPIO_DT_SPEC_GET`
  - `gpio_pin_configure_dt`, `gpio_pin_toggle_dt`
  - `LOG_INF` for LED state
- Simplified `app/Kconfig` (removed L3 menu complexity for this milestone).

### Key takeaway

Hardware pins and labels are described in devicetree; application code uses generated macros (`DT_ALIAS`, `gpio_dt_spec`) so the same source can target different boards via overlays.

---

## Lecture 5 — Custom board support

**Git:** `d03bfe9` (`l5-task1`), `9935882` (`l5-task2`)  
**Tags:** `l5-task1`, `l5-task2`

### Task 1 — `our_board`

- Created full out-of-tree board under `app/boards/our_board/`:
  - `our_board.dts`, defconfig, Kconfig, `board.yml`, pinctrl, documentation
  - Based on nRF52840 DK layout
- Updated `CMakeLists.txt` with `BOARD_ROOT`
- Updated `main.cpp` for the new board target

### Task 2 — `my_board`

- Added a second, slimmer board definition at `app/boards/my_board/`
- Minimal DTS (~80 lines) and board metadata for nRF52840 DK-class hardware

### Key takeaway

Board support packages (BSP) bundle DTS, Kconfig defaults, and CMake hooks so firmware can be built with `-b our_board` or `-b my_board` without modifying upstream Zephyr.

---

## Lecture 6 — Custom driver

**Homework docs:** `homework/l6-task1/README.md`, `homework/l6-task2/README.md`  
**Git:** `a8fec79` (`l6-task1`), `e1a5cad` (`l6-task2`)

### Task 1 — Sensor API driver (`l6-task1`)

Implemented an out-of-tree driver that controls an LED through Zephyr’s **Sensor driver API** (unusual but pedagogically useful — familiar app API over GPIO).

| Component | Path / detail |
|-----------|----------------|
| Binding | `app/dts/bindings/sensor/iomico,our-driver.yaml` |
| Driver | `app/driver/our_driver/our_driver.c` |
| Overlay | `app/src/app.overlay` — node `our_driver0`, alias `led` |
| Kconfig | `CONFIG_OUR_DRIVER=y` |
| App | `sensor_sample_fetch()` → LED on; `sensor_channel_get()` → LED off |

Driver registration uses `DT_DRV_COMPAT iomico_our_driver`, `SENSOR_DEVICE_DT_INST_DEFINE()`, and GPIO from `gpios` in the binding.

### Task 2 — Extension API (`l6-task2`)

Extended the driver beyond the standard sensor vtable:

- **`struct our_driver_data`** — runtime fields: `label`, `message`, `toggle_count`, `led_on`
- **`struct our_driver_api`** — embeds `struct sensor_driver_api` first, then custom function pointers
- **Public header** `our_driver.h` with `static inline` helpers:
  - `our_driver_set_label()`
  - `our_driver_set_message()`
  - `our_driver_set_toggle_count()`
- `main.cpp` calls `our_driver_set_message(dev, "heartbeat")` at startup

### Key takeaway

Zephyr separates **compile-time config** (`dev->config`) from **runtime data** (`dev->data`). Custom APIs extend the driver vtable while keeping the sensor interface for generic code paths.

---

## Lecture 7 — Zephyr Shell

**Homework doc:** `homework/l7-task1/README.md` (L7 task 2 doc not published upstream; implemented locally)  
**Git:** `d1b44c7` (`l7-task1`), `5213f19` (`l7-task2`)

### Task 1 — Shell commands (`l7-task1`)

- Enabled shell in `prj.conf`: `CONFIG_SHELL`, `CONFIG_SHELL_BACKEND_SERIAL`, tab completion, history
- Added `app/src/sensor_shell.c` with root command **`sensor`**
- Subcommands wired to the L6 driver via `DEVICE_DT_GET(DT_ALIAS(led))`:

| Command | Behavior |
|---------|----------|
| `sensor fetch` | `sensor_sample_fetch()` — LED on |
| `sensor read` | `sensor_channel_get()` — LED off |
| `sensor info` | Device name and `device_is_ready()` status |

- Refactored build (`CMakeLists.txt`) to compile shell module separately
- `main.cpp` still runs a heartbeat blink loop but documents available shell commands

**Console setup:** see `Documents/minicom-nrf52840-uart.md` (115200 8N1 on `/dev/ttyACM*`).

### Task 2 — Subcommands & validation (`l7-task2`)

Added shell commands that call the **L6 extension API**, with user-facing error messages:

| Command | Behavior |
|---------|----------|
| `sensor set <msg>` | `our_driver_set_message()` — rejects empty or >63 char messages |
| `sensor toggle <0-20>` | `our_driver_set_toggle_count()` — range-checked via `atoi` |

Uses `SHELL_CMD_ARG` for required arguments and `shell_error()` for validation failures.

### Key takeaway

The shell runs in its own thread; handlers should use `shell_print` / `shell_error` for user feedback and call driver APIs rather than toggling GPIO directly.

---

## Lecture 8 — Testing with ZTEST & Twister

**Git:** `6c7bb72` (`l8-task1`), `31446fb` (`l8-task2`)  
**No homework README** in repo; inferred from commits and `tests/` tree.

### Task 1 — Unit tests (`l8-task1`)

- Implemented a small **`ring_buf`** module (FIFO ring buffer for `int` values):
  - `app/modules/ring_buf/include/ring_buf.h`
  - `app/modules/ring_buf/src/ring_buf.c`
  - API: `rb_init`, `rb_push`, `rb_pop`, `rb_peek`, `rb_is_full`, `rb_is_empty`, `rb_count`
- Added Twister test application:
  - `tests/ring_buf/` — `test_ring_buf.c`, `testcase.yaml`, `prj.conf`
  - ZTEST suites: `ring_buf_init`, `ring_buf_push_pop` (FIFO order, full buffer, peek, etc.)
- Ran Twister on `native_sim` (log captured in `twister-out/twister.log`)

### Task 2 — Coverage (`l8-task2`)

- Re-ran Twister with coverage enabled
- Generated HTML coverage report under `twister-out/coverage/`
- **Results** (`twister-out/coverage_summary.json`):

| Metric | Coverage |
|--------|----------|
| Lines (`ring_buf.c`) | 88.2% (30/34) |
| Functions | 100% (7/7) |
| Branches | 64.3% (9/14) |

### Key takeaway

Out-of-tree logic can be tested on the host with `native_sim`, orchestrated by **Twister**, with **gcovr**-style coverage to find untested branches (e.g. error paths in `rb_push` / `rb_pop`).

---

## Current application state (HEAD = `l8-task2`)

At the latest tag, the firmware stack is:

1. **Board** — `our_board` or `nrf52840dk/nrf52840` with overlay for `iomico,our-driver` on LED0 (P0.13)
2. **Driver** — GPIO LED behind Sensor API + extension API for message/toggle count
3. **App** — blink loop in `main.cpp` using sensor fetch/channel_get
4. **Shell** — `sensor fetch|read|info|set|toggle` on serial console
5. **Tests** — separate `ring_buf` unit tests, not linked into the main app binary

Build example:

```bash
cd /home/jb/zeph-workspace/zephyr-course/app
west build -p always -b nrf52840dk/nrf52840
west flash
```

Run unit tests:

```bash
cd /home/jb/zeph-workspace/zephyr-course
west twister -T tests/ring_buf -p native_sim
```

---

## Supporting documents in this workspace

| File | Purpose |
|------|---------|
| `Documents/zephyr-menuconfig.md` | How to open menuconfig for `zephyr-course/app` |
| `Documents/minicom-nrf52840-uart.md` | Serial console setup for shell and logs |
| `Documents/zephyr-course-history.md` | This file |

---

## Skills gained (summary)

By the end of the course project, the codebase demonstrates:

- **West workspace** management with a separate manifest repo
- **Kconfig** menus for application tuning
- **Devicetree** overlays and GPIO via `gpio_dt_spec`
- **Out-of-tree boards** (`BOARD_ROOT`, custom `.dts`)
- **Out-of-tree drivers** with bindings, Kconfig, and Sensor API
- **Driver extension APIs** and `dev->data` runtime state
- **Zephyr Shell** command registration and argument validation
- **ZTEST + Twister** for host-side unit testing and coverage

Each lecture tag is a restorable checkpoint — useful for diffs, code review, or course submission PRs.
