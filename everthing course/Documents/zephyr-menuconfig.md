# Opening menuconfig (Zephyr Kconfig)

Use **menuconfig** to change firmware options (blink period, shell, logging, etc.) without editing `prj.conf` by hand. Settings are saved into your build’s `.config` file.

---

## Prerequisites

- Zephyr environment active (same shell where `west` works)
- App already configured at least once (`west build …` without `-p always` is enough)

---

## Open menuconfig

Run from your **application directory** (where `prj.conf` lives):

### `zephyr-course/app`

```bash
cd /home/jb/zeph-workspace/zephyr-course/app
west build -t menuconfig
```

### `zephyr-course/app_OG`

```bash
cd /home/jb/zeph-workspace/zephyr-course/app_OG
west build -t menuconfig
```

Or from the workspace root (uses the last build directory for that app):

```bash
cd /home/jb/zeph-workspace
west build -t menuconfig -d build zephyr-course/app
```

Replace `zephyr-course/app` with `zephyr-course/app_OG` and adjust `-d build` if your build dir is `app/build`.

---

## First-time / clean build

If there is no build directory yet, configure first:

```bash
cd /home/jb/zeph-workspace/zephyr-course/app
west build -b nrf52840dk/nrf52840 .
west build -t menuconfig
```

Use `-p always` only when you want to wipe the build and start fresh:

```bash
west build -p always -b nrf52840dk/nrf52840 .
```

---

## Navigate menuconfig

| Key | Action |
|-----|--------|
| ↑ / ↓ | Move between lines |
| Enter | Open submenu or toggle option |
| Space | Toggle `[*]` enable / `[ ]` disable |
| Esc Esc | Back / exit |
| `/` | Search for a symbol (e.g. `APP_HEARTBEAT`) |
| `?` | Help for highlighted option |
| `S` | Save (default location: `build/zephyr/.config`) |
| `Q` | Quit (prompts to save if changed) |

---

## Useful menus for this project

| Menu / symbol | Purpose |
|---------------|---------|
| **LED Blink Application** → `APP heartbeat period (ms)` | Blink speed in `main.cpp` |
| **Device Drivers** → **Sensor Drivers** → **Our Driver** / **LED Sensor** | Enable custom driver |
| **Subsystems and OS Services** → **Shell** | Shell and sub-options |

After changing options, save (`S`) and quit (`Q`), then rebuild:

```bash
west build
west flash
```

---

## Alternative: edit `prj.conf` directly

Any menuconfig change can be written as a line in `prj.conf`, for example:

```ini
CONFIG_APP_HEARTBEAT_PERIOD_MS=1000
CONFIG_SHELL=y
```

Then rebuild. `prj.conf` is merged on every build; menuconfig edits the generated `.config` in `build/zephyr/`.

---

## Troubleshooting

| Problem | Fix |
|---------|-----|
| `west build -t menuconfig` fails — no build dir | Run `west build -b nrf52840dk/nrf52840 .` first |
| Changes lost after rebuild | Save in menuconfig before quit; or add symbols to `prj.conf` |
| Option not visible | Enable parent menu (e.g. `CONFIG_LED_SUBSYSTEM=y` for heartbeat period in `app_OG`) |
| Wrong app configured | `cd` into the correct app dir before `west build -t menuconfig` |

---

## Quick reference

```bash
# Open UI
west build -t menuconfig

# Rebuild and flash after changes
west build
west flash
```

*Workspace: `/home/jb/zeph-workspace` · Boards: `nrf52840dk/nrf52840`*
