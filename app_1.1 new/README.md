# L7 Task 1 — LED Sensor (nRF52840 DK)

## Description

Custom `iomico,led-sensor` driver exposed through Zephyr's Sensor API,
controlled via an interactive shell. Ported from the STM32 Nucleo base
to the **nRF52840 DK** (PCA10056).

## Hardware

- Board: **nRF52840 DK** (`nrf52840dk/nrf52840`)
- LED used: **LED0** — P0.13, active-low (onboard green LED)

## Key changes vs. Nucleo base

| File | What changed |
|------|-------------|
| `boards/nrf52840dk_nrf52840.overlay` | New file — instantiates `led_sensor` on `gpio0 13 GPIO_ACTIVE_LOW` |
| `app.overlay` | Alias `app-led` now points at `&led0` (nRF52840 DK LED0) |
| `west.yml` | HAL allowlist trimmed to `cmsis_6` + `hal_nordic` only |
| STM32 board dirs | Removed (not needed) |

## How to build

From your west workspace root (`/home/jb/zeph-workspace`):

```bash
west build -p always -b nrf52840dk/nrf52840 zephyr-course/app
```

## How to flash

```bash
west flash
```

## Shell commands (via USB UART at 115200)

```
sensor fetch   # turns LED0 ON
sensor read    # turns LED0 OFF, prints value
sensor info    # prints device name and ready state
```
