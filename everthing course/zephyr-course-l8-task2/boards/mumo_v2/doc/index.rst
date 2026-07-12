.. zephyr:board:: mumo_v2

Overview
********

MuMo (short for Museum Monitor) is a compact-sized LoRaWAN based datalogger, built around the Seeed Studio LoRa-E5 STM32WLE5JC module.

Hardware
********

The boards' LoRa-E5 Module packages a STM32WLE5JC SOC, a 32MHz TCXO,
and a 32.768kHz crystal oscillator in a 28-pin SMD package.
This STM32WLEJC SOC is powered by ARM Cortex-M4 core and integrates Semtech
SX126X LoRa IP to support LoRa modulations.

- LoRa-E5 STM32WLE5JC Module with STM32WLE5JC multiprotocol LPWAN single-core
  32-bit microcontroller (Arm® Cortex®-M4 at 48 MHz) in 28-pin SMD package
  featuring:

  - Ultra-low-power MCU
  - 256-Kbyte Flash memory and 64-Kbyte SRAM
  - Hardware encryption AES128-bit and a True random number generator
  - RF transceiver supporting LoRa® / LoRaWAN®

- reset push-button
- Board connectors:

  - USB Type-C connector - providing power and UART connectivity via built-in USB to UART converter
  - +/- LiFePO4 battery terminals (3.2V)
  - SMA antenna connector
  - ePaper Display connector (24 pin)
  - 2.54mm header for external debug probe connection
  - Grove connector for I2C and UART

- 4x M2 mounting holes

Supported Features
==================

.. zephyr:board-supported-hw::

Connections and IOs
===================

LoRa-E5 mini has 4 GPIO controllers. These controllers are responsible for pin
muxing, input/output, pull-up, etc.

Available pins:
---------------

.. image:: img/lora_e5_mini_pinout.jpg
      :align: center
      :alt: LoRa-E5 mini Pinout

Default Zephyr Peripheral Mapping:
----------------------------------

- USART_1 TX  : PB6
- USART_1 RX  : PB7
- I2C_2_SCL   : PB15
- I2C_2_SDA   : PA15
- BOOT_PB     : PB13
- LED_1       : PB5

System Clock
------------

LoRa-E5 mini board System Clock could be driven by the low-power internal (MSI),
High-speed internal (HSI) or High-speed external (HSE) oscillator, as well as
main PLL clock. By default System clock is driven by the MSI clock at 48MHz.

Flashing & Debugging
====================

The MuMo_v2does not include a on-board debug probe.
But the module can be debugged by connecting an external debug probe to the
2.54mm header, usinf SWD interface. board.cmake is set up for J-Link, ST-Link and Black Magic probes.

Per default the console on ``usart2`` is available on the USB Type C connector
via the built-in USB to UART converter.

Note : the Seeed Studio LoRa-E5 mdule used on the MuMo_v2 board comes factory programmed with an AT-Modem application and the option bytes set to BB read protection.. Remove the read protection using eg. STM32 Cube Programmer. From then on you can  the board for your zephyr application.

More Info
=========

.. _MuMo_v2 hardware:
   https://github.com/Strooom/LoRa-V3-PCB

.. _MuMo_v2 software (non zephyr):
   https://github.com/Strooom/MuMo-v2-Node-SW

