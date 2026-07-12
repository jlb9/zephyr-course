# Connecting to the nRF52840 DK with minicom

This guide explains how to open a serial console to the **nRF52840 DK** using **minicom** on Linux. Use it to view `printk` output and run Zephyr shell commands for the LED sensor app (`zephyr-course/app`).

**Serial settings:** 115200 baud, 8 data bits, no parity, 1 stop bit (8N1), no hardware or software flow control.

---

## Prerequisites

- nRF52840 DK connected via USB (J-Link / onboard USB-UART bridge)
- Firmware flashed (`west flash` from `zephyr-course/app`)
- `minicom` installed:

```bash
sudo apt install minicom
```

---

## Step 1: Find the serial port

With the board plugged in, list likely devices:

```bash
ls -l /dev/ttyACM* /dev/ttyUSB* 2>/dev/null
```

Check recent kernel messages if the device name is unclear:

```bash
dmesg | tail -20
```

On many Linux systems the DK appears as **`/dev/ttyACM0`** (or `ttyACM1` if multiple boards are connected).

---

## Step 2: Fix permissions (if needed)

If minicom reports “Permission denied”:

```bash
sudo usermod -aG dialout $USER
```

Log out and log back in (or reboot) for the group change to take effect.

---

## Step 3: Connect with minicom (quick start)

Replace `/dev/ttyACM0` with your actual device:

```bash
minicom -D /dev/ttyACM0 -b 115200
```

Press the board **RESET** button if you do not see boot messages or the shell prompt.

---

## Step 4: Save a minicom configuration (optional)

For a reusable setup:

```bash
sudo minicom -s
```

1. Select **Serial port setup**
2. Set:
   - **A** — Serial Device: `/dev/ttyACM0`
   - **E** — Bps/Par/Bits: `115200 8N1`
   - **F** — Hardware Flow Control: **No**
   - **G** — Software Flow Control: **No**
3. Press **Esc** to return
4. Select **Save setup as dfl** (default) or **save named** (e.g. `nrf52840`)
5. Select **Exit**

Connect later with:

```bash
minicom
```

Or, if you saved a named profile:

```bash
minicom nrf52840
```

---

## Step 5: Use the Zephyr shell

After reset you should see a prompt similar to:

```text
uart:~$
```

Shell commands for the LED sensor app:

| Command | Action |
|---------|--------|
| `sensor fetch` | Turn LED0 on (P0.13) |
| `sensor read` | Turn LED0 off, print sensor value |
| `sensor info` | Print device name and ready state |
| `help` | List available commands |

Example session:

```text
uart:~$ sensor fetch
fetch OK - LED on
uart:~$ sensor read
read OK - LED off, val=0.000000
uart:~$ sensor info
Device name : led_sensor
Device ready: YES
```

On boot, the firmware also blinks LED0 three times as a quick self-test.

---

## minicom key reference

| Action | Keys |
|--------|------|
| Command menu | `Ctrl-A`, then `Z` |
| Quit minicom | `Ctrl-A`, then `X` (confirm Yes) |
| Toggle local echo | `Ctrl-A`, then `E` (useful if typed characters do not appear) |
| Toggle line wrap | `Ctrl-A`, then `U` |

---

## Troubleshooting

| Problem | What to try |
|---------|-------------|
| No output | Correct `/dev/ttyACM*` device; press RESET; close other programs using the port |
| Permission denied | Add user to `dialout` group (Step 2) |
| Garbled text | Confirm **115200** baud and 8N1 |
| `west flash` fails | Close minicom first — only one program can use the port |
| No `uart:~$` prompt | Rebuild with shell enabled; check `CONFIG_SHELL=y` in `prj.conf` |

---

## Alternative: west attach

From the app build directory:

```bash
cd /home/jb/zeph-workspace/zephyr-course/app
west attach
```

Exit with `Ctrl+]`. Same UART as minicom; close before flashing.

---

## Workflow summary

1. Build and flash: `west build …` then `west flash`
2. Close minicom / `west attach` before flashing
3. Open minicom: `minicom -D /dev/ttyACM0 -b 115200`
4. Press RESET if needed
5. Run `sensor fetch`, `sensor read`, `sensor info`

---

*Workspace: `/home/jb/zeph-workspace` · Board: nrf52840dk/nrf52840 · App: `zephyr-course/app`*
