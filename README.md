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

created a workspace using the following file structure
workspace/
├── deps
│   ├── modules
│   └── zephyr
├── zephyr-course
│   ├── app
│   └── README.md
└── zephyr-manifest
    └── west.yml

---

## Homework

| Lesson | Task | Document |
|--------|------|----------|
| 6 | Task 1 — Custom driver (Sensor API) | [homework/l6-task1/README.md](homework/l6-task1/README.md) |
| 6 | Task 2 — Custom extension API on driver | [homework/l6-task2/README.md](homework/l6-task2/README.md) |
| 7 | Task 1 — Shell commands for custom driver | [homework/l7-task1/README.md](homework/l7-task1/README.md) |

Submit homework via a [fork](https://github.com/iomico-public/zephyr-course/forks) of the upstream course repository. Push a git tag matching the task (`l6-task1`, `l6-task2`, `l7-task1`, …) and open a pull request to the upstream repo.
