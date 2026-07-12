# Zephyr Training Environment

Welcome to the Zephyr RTOS training! This repository includes a ready-to-use
development environment based on Zephyr 4.3.0, which you can set up in one of
three ways:

---

## Manual Zephyr Setup

Follow the following guide:
- [Getting Started Guide](https://docs.zephyrproject.org/latest/develop/getting_started/index.html#).

Make sure to select appropriate OS and to perform all steps till
[Build the Blinky Sample](https://docs.zephyrproject.org/latest/develop/getting_started/index.html#build-the-blinky-sample).

# ESP32-S3 Zephyr Build & Run (esp32s3_devkitc)

## 🔧 Environment Setup

``` bash
export ZEPHYR_TOOLCHAIN_VARIANT=espressif
export ESPRESSIF_TOOLCHAIN_PATH=/home/sarath/xtensa-esp32s3-elf
export PATH="$PATH:/home/sarath/xtensa-esp32s3-elf/bin"

source deps/zephyr/zephyr-env.sh

echo $ZEPHYR_BASE
# Output:
# /home/sarath/zephyr-dev/deps/zephyr
```

------------------------------------------------------------------------

## 📦 Build Application

``` bash
cd zephyr-course/

west build -b esp32s3_devkitc/esp32s3/procpu app -p
```


## 📦 Build Application

``` bash

west build -b esp32s3_devkitc/esp32s3/procpu app -p
```


### 🧾 Build Summary

-   **Zephyr Version:** 4.2.0\
-   **Board:** esp32s3_devkitc (esp32s3/procpu)\
-   **Toolchain:** espressif\
-   **CMake:** 3.22.1\
-   **Python:** 3.12.2

### 📊 Memory Usage

    FLASH:         136200 B / 8388352 B   (1.62%)
    IRAM:           45224 B / 343552 B   (13.16%)
    DRAM:           50792 B / 327168 B   (15.52%)
    IROM:           14767 B / 32 MB      (0.04%)
    DROM:           70664 B / 32 MB      (0.21%)

### ✅ Build Result

    Successfully created esp32s3 image.

------------------------------------------------------------------------

## 🚀 Boot Log (Serial Output)

    ESP-ROM:esp32s3-20210327
    Build:Mar 27 2021

    rst:0x1 (POWERON),boot:0xb (SPI_FAST_FLASH_BOOT)
    mode:DIO, clock div:1

    SHA-256 comparison failed:
    Calculated: 976d4a6aa2afa2f653fa729847bc687117693d6011ef33474d23df6d47c69f2c
    Expected:   0000000000000000000000000000000000000000000000000000000000000000

    Attempting to boot anyway...

    *** Booting Zephyr OS build v4.2.0 ***

------------------------------------------------------------------------

## 💡 Application Output (LED Blink)

    [00:00:00.173] <inf> main: ESP32 (GPIO-4) - LED state: OFF
    [00:00:01.173] <inf> main: ESP32 (GPIO-4) - LED state: ON
    [00:00:02.173] <inf> main: ESP32 (GPIO-4) - LED state: OFF
    [00:00:03.173] <inf> main: ESP32 (GPIO-4) - LED state: ON
    [00:00:04.174] <inf> main: ESP32 (GPIO-4) - LED state: OFF
    [00:00:05.174] <inf> main: ESP32 (GPIO-4) - LED state: ON
    [00:00:06.174] <inf> main: ESP32 (GPIO-4) - LED state: OFF
    [00:00:07.174] <inf> main: ESP32 (GPIO-4) - LED state: ON
    [00:00:08.174] <inf> main: ESP32 (GPIO-4) - LED state: OFF

------------------------------------------------------------------------

## ⚠️ Notes

-   LED toggling successfully on **GPIO-4**.
-   Serial port used: `/dev/ttyUSB0`
