<p align="center">
  <h1 align="center">🛰️ STM32 Baremetal Data Acquisition & Telemetry System</h1>
  <p align="center">
    <b>Real-Time Embedded DSP · Custom RTOS Kernel · SPI Inter-MCU Link · MQTT IoT Cloud · GSM SMS Alerts</b>
  </p>
  <p align="center">
    <img src="https://img.shields.io/badge/STM32-F446RE-03234B?style=for-the-badge&logo=stmicroelectronics&logoColor=white" alt="STM32"/>
    <img src="https://img.shields.io/badge/ESP32-DevKit-E7352C?style=for-the-badge&logo=espressif&logoColor=white" alt="ESP32"/>
    <img src="https://img.shields.io/badge/BMP280-Sensor-green?style=for-the-badge" alt="BMP280"/>
    <img src="https://img.shields.io/badge/MQTT-HiveMQ-FFD700?style=for-the-badge" alt="MQTT"/>
    <img src="https://img.shields.io/badge/License-MIT-blue?style=for-the-badge" alt="License"/>
  </p>
</p>

---

## 📋 Table of Contents

- [Overview](#overview)
- [Technical Firmware Documentation](#technical-firmware-documentation)
- [System Architecture](#system-architecture)
- [Repository Structure](#repository-structure)
- [STM32 Firmware](#stm32-firmware--stm32-rtos-dsp-telemetry-system)
  - [Custom Baremetal RTOS Kernel](#custom-baremetal-rtos-kernel)
  - [Advanced DSP Filter Pipeline](#advanced-dsp-filter-pipeline)
  - [Baremetal Peripheral Drivers](#baremetal-peripheral-drivers)
  - [Sensor Interface — BMP280](#sensor-interface--bmp280)
  - [SPI Telemetry Master](#spi-telemetry-master)
  - [GSM/SMS Module](#gsmsms-module)
  - [Independent Watchdog (IWDG)](#independent-watchdog-iwdg)
  - [RTOS Application Tasks](#rtos-application-tasks)
- [ESP32 Firmware](#esp32-firmware--esp32-spi-mqtt-telemetry)
  - [Baremetal SPI Slave Driver](#baremetal-spi-slave-driver)
  - [Baremetal MQTT Protocol Stack](#baremetal-mqtt-protocol-stack)
  - [SMS Controller — Bidirectional Command Channel](#sms-controller--bidirectional-command-channel)
  - [Telemetry Task & MQTT Publishing](#telemetry-task--mqtt-publishing)
- [Node-RED Dashboard](#node-red-dashboard)
- [Hardware Setup & Pin Mapping](#hardware-setup--pin-mapping)
- [SPI Protocol — Packet Format](#spi-protocol--packet-format)
- [Data Flow Pipeline](#data-flow-pipeline)
- [Build & Flash Instructions](#build--flash-instructions)
- [License](#license)

---

## Overview

> **🎥 System Demonstration Video:** [Watch on Google Drive](https://drive.google.com/file/d/1UevbJu37hguGYBoKWQwyH6ImwLKadNzi/view?usp=sharing)

A **production-grade, dual-MCU embedded telemetry system** built entirely from scratch — **zero HAL libraries, zero third-party RTOS, zero MQTT client libraries**. The system acquires environmental sensor data (temperature, pressure, altitude) on an STM32F446RE, processes it through a multi-stage DSP filter pipeline, and transmits the filtered data over SPI to an ESP32, which bridges it to the cloud via MQTT and provides SMS alerting capabilities.

### Key Highlights

| Feature | Implementation |
|---|---|
| **RTOS Kernel** | Custom preemptive kernel with PendSV context switching, TCB management, semaphores, mutexes, queues, event groups, software timers, and heap allocator — all written from scratch in C and ARM assembly |
| **DSP Pipeline** | 3-stage cascaded filter: Median → IIR Low-Pass → 2D Kalman (with velocity estimation) |
| **Peripheral Drivers** | 100% register-level baremetal drivers for GPIO, I2C, SPI, USART, RCC, IWDG on STM32; register-level SPI3 slave on ESP32 |
| **MQTT Stack** | Hand-crafted MQTT 3.1.1 packet builder (CONNECT, PUBLISH, SUBSCRIBE, DISCONNECT) — no library dependency |
| **Telemetry Link** | Full-duplex SPI1 (Master) ↔ SPI3/VSPI (Slave) with software CS, bidirectional packet exchange, and magic-word command channel |
| **SMS Alerts** | SIM900 GSM module with AT command interface; triggerable from physical button or remote MQTT dashboard |
| **Cloud Dashboard** | Node-RED flow with live gauges for temperature, pressure, altitude, vertical speed, and SMS trigger button |
| **Watchdog** | Independent Watchdog (IWDG) with 20-second timeout, refreshed by highest-priority RTOS task |

---

## Technical Firmware Documentation

> **In-depth firmware design documents covering register-level implementation details, signal flow, and hardware interface specifications.**

| Document | Covers | Link |
|---|---|---|
| **ESP32 Firmware Technical Document** | SPI Slave bare-metal driver, MQTT packet builder, SMS controller, Wi-Fi stack integration, FreeRTOS task architecture | [📥 View / Download (Google Drive)](https://drive.google.com/file/d/18bVclN1iqHQIFytH--G03Z43vWV8Vs81/view?usp=sharing) |
| **STM32 Firmware Technical Document** | Custom RTOS kernel design, DSP filter pipeline, baremetal peripheral drivers (I2C, SPI, USART, GPIO, RCC), BMP280 sensor interface, GSM AT commands, IWDG watchdog | [📥 View / Download (Google Drive)](https://drive.google.com/file/d/1OwQ9lUHhYAzFEu3UqaFkpom1Lghf1FIk/view?usp=sharing) |

---

## System Architecture

```
┌─────────────────────────────────────────────────────────────────────┐
│                        CLOUD / DASHBOARD                           │
│  ┌───────────────┐    ┌──────────────────────────────────────────┐  │
│  │  HiveMQ MQTT  │◄──►│  Node-RED Dashboard                     │  │
│  │  Broker        │    │  • Temperature Gauge   • Altitude Gauge │  │
│  │  (broker.      │    │  • Pressure Gauge      • V-Speed Gauge  │  │
│  │   hivemq.com)  │    │  • SMS Trigger Button                   │  │
│  └───────┬───────┘    └──────────────────────────────────────────┘  │
│          │ TCP:1883                                                  │
│          │ MQTT 3.1.1 (Baremetal packet builder)                    │
└──────────┼──────────────────────────────────────────────────────────┘
           │
┌──────────▼──────────────────────────────────────────────────────────┐
│                     ESP32 DevKit (Wi-Fi Gateway)                    │
│                                                                      │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────────────────┐   │
│  │ SPI3/VSPI    │  │ MQTT Packet  │  │ SMS Controller           │   │
│  │ Slave Driver │  │ Builder      │  │ (Subscribes command/sms) │   │
│  │ (Baremetal   │  │ (Baremetal   │  │ Sets magic word in SPI   │   │
│  │  Registers)  │  │  Protocol)   │  │ response packet          │   │
│  └──────┬───────┘  └──────┬───────┘  └──────────────────────────┘   │
│         │                  │                                         │
│         │ Full-Duplex SPI  │ JSON → telemetry/bmp280                │
└─────────┼──────────────────┼────────────────────────────────────────┘
          │ 5-Wire           │
          │ MOSI/MISO/       │
          │ SCLK/CS/GND      │
┌─────────▼──────────────────▼────────────────────────────────────────┐
│                   STM32F446RE Nucleo (Sensor Node)                   │
│                                                                      │
│  ┌─────────────────────────────────────────────────────────────────┐ │
│  │                  Custom Baremetal RTOS Kernel                   │ │
│  │  PendSV Context Switch │ Round-Robin Scheduler │ Task Delays   │ │
│  │  Semaphores │ Mutexes │ Queues │ Event Groups │ Software Timers│ │
│  │  Heap Allocator (8KB bump)                                     │ │
│  └─────────────────────────────────────────────────────────────────┘ │
│                                                                      │
│  ┌────────────┐  ┌──────────────────────┐  ┌────────────────────┐   │
│  │ BMP280     │  │ DSP Filter Pipeline  │  │ SPI1 Telemetry     │   │
│  │ I2C Driver │─►│ Median → IIR → Kalman│─►│ Master (Baremetal) │   │
│  │ (0x76)     │  │ + Velocity Estimation│  │ + Software CS      │   │
│  └────────────┘  └──────────────────────┘  └────────────────────┘   │
│                                                                      │
│  ┌──────────────────┐  ┌────────────────┐  ┌────────────────────┐   │
│  │ GSM/SIM900       │  │ USART2 Debug   │  │ IWDG Watchdog      │   │
│  │ USART1 @ 9600bps │  │ printf @ 115200│  │ 20s Timeout        │   │
│  │ AT+CMGF, AT+CMGS │  │ RAW + FILT logs│  │ Refresh Task (P5)  │   │
│  └──────────────────┘  └────────────────┘  └────────────────────┘   │
└─────────────────────────────────────────────────────────────────────┘
```

---

## Repository Structure

```
stm32-baremetal-data-acquisition-telemetry-system/
│
├── stm32-rtos-dsp-telemetry-system/          # STM32F446RE firmware
│   ├── Inc/                                   # Header files
│   │   ├── AppConfig.h                        # Task intervals, priorities, phone number
│   │   ├── BoardConfig.h                      # Pin assignments, peripheral mappings
│   │   ├── app/
│   │   │   └── app_tasks.h                    # RTOS task declarations
│   │   ├── drivers/
│   │   │   ├── stm32f446xx.h                  # Full MCU register definitions (45KB)
│   │   │   ├── stm32f446xx_gpio_driver.h      # GPIO driver API
│   │   │   ├── stm32f446xx_i2c_driver.h       # I2C driver API
│   │   │   ├── stm32f446xx_spi_driver.h       # SPI driver API
│   │   │   ├── stm32f446xx_usart_driver.h     # USART driver API
│   │   │   ├── stm32f446xx_rcc_driver.h       # RCC clock driver API
│   │   │   └── delay.h                        # SysTick timer & delay API
│   │   ├── dsp/
│   │   │   ├── dsp_engine.h                   # Top-level DSP engine
│   │   │   ├── filter_cascade.h               # 3-stage cascade filter
│   │   │   ├── filter_median.h                # Median filter (window=5)
│   │   │   ├── filter_iir.h                   # IIR low-pass filter
│   │   │   └── filter_kalman.h                # 2D Kalman filter (position + velocity)
│   │   ├── esp32-telemetry/
│   │   │   └── spi_telemetry.h                # SPI telemetry packet definition & API
│   │   ├── gsm/
│   │   │   └── gsm.h                          # GSM/SIM900 SMS driver API
│   │   ├── rtos/
│   │   │   ├── kernel.h                       # RTOS kernel API
│   │   │   ├── task.h                         # Task Control Block (TCB) & task API
│   │   │   ├── scheduler.h                    # Cooperative scheduler API
│   │   │   ├── semaphore.h                    # Counting semaphore API
│   │   │   ├── mutex.h                        # Mutex with ownership tracking
│   │   │   ├── queue.h                        # Generic thread-safe queue
│   │   │   ├── event.h                        # Event group flags
│   │   │   ├── timer.h                        # Software timer with auto-reload
│   │   │   ├── heap.h                         # Bump allocator (8KB)
│   │   │   ├── idle.h                         # Idle task
│   │   │   └── syscall.h                      # SVC system call
│   │   ├── sensors/
│   │   │   └── bmp280.h                       # BMP280 sensor driver API
│   │   └── watchdog/
│   │       └── stm32f446xx_iwdg_driver.h      # Independent Watchdog driver API
│   │
│   ├── Src/                                   # Source files
│   │   ├── main.c                             # System init, peripheral setup, kernel launch
│   │   ├── app/
│   │   │   └── app_tasks.c                    # 7 RTOS tasks definition & registration
│   │   ├── drivers/
│   │   │   ├── stm32f446xx_gpio_driver.c      # GPIO register-level driver (12.7KB)
│   │   │   ├── stm32f446xx_i2c_driver.c       # I2C register-level driver (24.3KB)
│   │   │   ├── stm32f446xx_spi_driver.c       # SPI register-level driver (12.8KB)
│   │   │   ├── stm32f446xx_usart_driver.c     # USART register-level driver (25.4KB)
│   │   │   ├── stm32f446xx_rcc_driver.c       # RCC clock configuration
│   │   │   └── delay.c                        # SysTick init, delay_ms, tick counter
│   │   ├── dsp/
│   │   │   ├── dsp_engine.c                   # DSP orchestrator (per-channel tuning)
│   │   │   ├── filter_cascade.c               # Median → IIR → Kalman pipeline
│   │   │   ├── filter_median.c                # Sliding window median (insertion sort)
│   │   │   ├── filter_iir.c                   # First-order IIR: y = αx + (1-α)y
│   │   │   └── filter_kalman.c                # 2D Kalman: predict + update (69 lines)
│   │   ├── esp32-telemetry/
│   │   │   └── spi_telemetry.c                # SPI1 master TX/RX with software CS
│   │   ├── gsm/
│   │   │   └── gsm.c                          # AT commands, SMS formatting, button handler
│   │   ├── rtos/
│   │   │   ├── kernel.c                       # Kernel init, start, SysTick handler
│   │   │   ├── task.c                         # TCB creation, stack init, round-robin select
│   │   │   ├── context_switch.s               # PendSV handler (ARM Cortex-M4 assembly)
│   │   │   ├── scheduler.c                    # Cooperative time-based scheduler
│   │   │   ├── semaphore.c                    # Counting semaphore (CPSID/CPSIE guards)
│   │   │   ├── mutex.c                        # Mutex with owner tracking
│   │   │   ├── queue.c                        # Circular buffer queue (memcpy-based)
│   │   │   ├── event.c                        # Event group wait/set with clear-on-exit
│   │   │   ├── timer.c                        # Software timers (one-shot + auto-reload)
│   │   │   ├── heap.c                         # 8KB bump allocator (8-byte aligned)
│   │   │   ├── idle.c                         # Idle task placeholder
│   │   │   └── syscall.c                      # SVC trap handler
│   │   ├── sensors/
│   │   │   └── bmp280.c                       # BMP280 init, calibration, compensation
│   │   └── watchdog/
│   │       └── stm32f446xx_iwdg_driver.c      # IWDG init & refresh
│   │
│   ├── Startup/
│   │   └── startup_stm32f446retx.s            # Vector table, Reset_Handler
│   ├── STM32F446RETX_FLASH.ld                 # Linker script (flash)
│   └── STM32F446RETX_RAM.ld                   # Linker script (RAM)
│
├── esp32-spi-mqtt-telemetry/                  # ESP32 firmware
│   ├── main/
│   │   ├── main.c                             # Wi-Fi init, MQTT connect, task creation
│   │   └── CMakeLists.txt                     # Build configuration
│   ├── components/
│   │   ├── spi_slave_driver/
│   │   │   ├── spi_slave.c                    # SPI3/VSPI bare-metal register driver
│   │   │   └── include/spi_slave.h            # TelemetryPacket_t definition
│   │   ├── mqtt_server/
│   │   │   ├── mqtt_server.c                  # MQTT 3.1.1 packet builder (CONNECT/PUBLISH/SUBSCRIBE/DISCONNECT)
│   │   │   └── include/mqtt_server.h          # MQTT API declarations
│   │   └── sms_controller/
│   │       ├── sms_controller.c               # MQTT RX listener for "command/sms" topic
│   │       └── include/sms_controller.h       # SMS trigger flag
│   ├── telemetry_stm32/
│   │   └── telemetry_stm32.c                  # SPI slave task: receive → parse → MQTT publish
│   ├── node_red_dashboard_flow.json           # Node-RED importable dashboard flow
│   ├── CMakeLists.txt                         # Top-level ESP-IDF project file
│   └── sdkconfig                              # ESP-IDF SDK configuration
│
├── LICENSE                                    # MIT License
└── README.md                                  # This file
```

---

## STM32 Firmware — `stm32-rtos-dsp-telemetry-system/`

> **Target:** STM32F446RE (ARM Cortex-M4, 180 MHz, FPU)  
> **Toolchain:** ARM GCC, STM32CubeIDE  
> **No HAL. No CMSIS. 100% register-level baremetal C.**

### Custom Baremetal RTOS Kernel

A fully custom preemptive real-time kernel built from scratch, featuring PendSV-based context switching in ARM assembly.

| Component | File | Description |
|---|---|---|
| **Kernel** | `Src/rtos/kernel.c` | Initializes task system, starts first task, sets PendSV to lowest priority (`0xFF`), SysTick-driven tick handler triggers context switches |
| **Task Manager** | `Src/rtos/task.c` | TCB array (16 tasks max, 1KB stack each), Cortex-M4 stack frame initialization (`xPSR`, `PC`, `LR`, `R12`, `R0-R3`), round-robin scheduling with blocked-state support |
| **Context Switch** | `Src/rtos/context_switch.s` | PendSV_Handler in ARM assembly — saves `R4-R11` + `LR` on PSP, calls `Task_SelectNext`, restores next task's context, returns via `BX LR` |
| **Semaphore** | `Src/rtos/semaphore.c` | Counting semaphore with interrupt-disabled critical sections (`CPSID I` / `CPSIE I`), spin-wait with `Task_Yield()` |
| **Mutex** | `Src/rtos/mutex.c` | Mutex with owner tracking (pointer to `current_tcb`), prevents double-unlock by non-owner |
| **Queue** | `Src/rtos/queue.c` | Generic circular buffer queue with `memcpy`-based item transfer, blocking send/receive |
| **Event Groups** | `Src/rtos/event.c` | Bitmask event flags with wait-all/wait-any modes and optional clear-on-exit |
| **Software Timers** | `Src/rtos/timer.c` | Up to 10 active timers, one-shot or auto-reload, callback-driven with `void*` argument |
| **Heap Allocator** | `Src/rtos/heap.c` | 8KB bump allocator with 8-byte alignment, thread-safe allocation |
| **Scheduler** | `Src/rtos/scheduler.c` | Cooperative time-interval scheduler (alternative to preemptive kernel) |

**Task Scheduling Model:**

```
SysTick (1ms) ──► Kernel_SysTickHandler() ──► Task_UpdateDelays() ──► PendSV (lowest priority)
                                                                          │
                                                                    ┌─────▼──────┐
                                                                    │ PendSV_    │
                                                                    │ Handler    │
                                                                    │ (ASM)      │
                                                                    │            │
                                                                    │ Save R4-R11│
                                                                    │ Select Next│
                                                                    │ Load R4-R11│
                                                                    │ BX LR      │
                                                                    └────────────┘
```

---

### Advanced DSP Filter Pipeline

A 3-stage cascaded digital signal processing pipeline applied independently to temperature, pressure, and altitude channels, with per-channel parameter tuning.

```
Raw Sensor Data ──► [Median Filter] ──► [IIR Low-Pass] ──► [2D Kalman Filter] ──► Filtered Output
                     (Window = 5)       (Configurable α)     (State + Velocity)     + Vertical Speed
```

| Stage | Algorithm | Purpose | Key Parameters |
|---|---|---|---|
| **1. Median Filter** | Sliding window (N=5) with insertion sort | Rejects spike anomalies & outliers | `MEDIAN_WINDOW_SIZE = 5` |
| **2. IIR Low-Pass** | `y[n] = α·x[n] + (1−α)·y[n−1]` | Smooths high-frequency noise | `α` per channel: Temp=0.2, Press=0.5, Alt=0.8 |
| **3. 2D Kalman Filter** | Predict-Update cycle with 2×2 state `[position, velocity]` | Optimal state estimation + velocity tracking | `R` (measurement noise), `Q_val`, `Q_vel` per channel |

**Per-Channel Tuning (from `dsp_engine.c`):**

| Channel | α (IIR) | R (Kalman) | Q_val | Q_vel | Rationale |
|---|---|---|---|---|---|
| Temperature | 0.2 | 0.5 | 0.001 | 0.0001 | Slow-changing, heavy smoothing |
| Pressure | 0.5 | 2.0 | 0.1 | 0.01 | Moderate dynamics |
| Altitude | 0.8 | 1.0 | 0.01 | 0.001 | Fast response needed for vertical speed |

---

### Baremetal Peripheral Drivers

All drivers operate at the register level using direct memory-mapped register access — no STM32 HAL, no CMSIS.

| Driver | File | Size | Key Features |
|---|---|---|---|
| **MCU Register Map** | `Inc/drivers/stm32f446xx.h` | 45 KB | Complete peripheral register definitions, base addresses, bit masks, clock enable macros for all STM32F446RE peripherals |
| **GPIO** | `Src/drivers/stm32f446xx_gpio_driver.c` | 12.7 KB | Mode config (Input/Output/AltFn/Analog), push-pull/open-drain, pull-up/down, speed, read/write pin/port, toggle |
| **I2C** | `Src/drivers/stm32f446xx_i2c_driver.c` | 24.3 KB | Master TX/RX, ACK control, repeated start, clock stretching, FM/SM speed, address handling |
| **SPI** | `Src/drivers/stm32f446xx_spi_driver.c` | 12.8 KB | Full-duplex, master/slave, DFF 8/16-bit, CPOL/CPHA modes, SSM/SSI, bus config |
| **USART** | `Src/drivers/stm32f446xx_usart_driver.c` | 25.4 KB | TX/RX, baud rate calculation from bus clock, word length, stop bits, parity, HW flow control |
| **RCC** | `Src/drivers/stm32f446xx_rcc_driver.c` | 1.8 KB | APB1/APB2 bus clock calculation for baud rate derivation |
| **Delay/SysTick** | `Src/drivers/delay.c` | 0.6 KB | SysTick @ 1ms (16 MHz HSI), `delay_ms()`, `SysTick_GetMillis()`, hooks into RTOS kernel |

---

### Sensor Interface — BMP280

| Parameter | Value |
|---|---|
| **Interface** | I2C1 @ Standard Mode (100 kHz) |
| **Address** | `0x76` |
| **Device ID Check** | Reads register `0xD0`, expects `0x58` |
| **Calibration** | Reads 24 bytes from `0x88` (T1-T3, P1-P9), stores in `BMP280_CalibData` struct |
| **Mode** | Normal mode, x1 oversampling (temp & pressure) |
| **Compensation** | Integer-based Bosch compensation algorithms (temperature → `t_fine` → pressure) |
| **Altitude** | Hypsometric formula: `44330 × (1 - (P/P₀)^0.1903)` where P₀ = 101325 Pa |

---

### SPI Telemetry Master

| Parameter | Value |
|---|---|
| **Peripheral** | SPI1 |
| **Mode** | Master, Full-Duplex, Mode 1 (CPOL=0, CPHA=1) |
| **Clock** | PCLK/64 (slow for breadboard signal integrity) |
| **Data Frame** | 8-bit, MSB first |
| **CS Management** | Software (PB6 GPIO), manual assert/deassert with 500-cycle delays for ESP32 sync |
| **Packet Size** | 20 bytes (`TelemetryPacket_t`) |
| **Bidirectional** | Simultaneous TX (sensor data) and RX (command response) per transaction |
| **Command Channel** | If `rx_packet.timestamp == 0xAA55AA55`, triggers SMS send |

---

### GSM/SMS Module

| Parameter | Value |
|---|---|
| **Module** | SIM900 (compatible) |
| **Interface** | USART1 @ 9600 bps, 8N1 |
| **SMS Format** | Text mode (`AT+CMGF=1`) |
| **Trigger** | Physical button (PA8, active-low, internal pull-up) OR MQTT dashboard command |
| **Cooldown** | 5-second cooldown between SMS (prevents spam) |
| **SMS Content** | Formatted sensor report: Temperature, Pressure (hPa), Altitude, Vertical Speed |
| **Debounce** | 50 ms software debounce, wait-for-release loop |

---

### Independent Watchdog (IWDG)

| Parameter | Value |
|---|---|
| **Prescaler** | DIV_256 |
| **Reload** | 2500 |
| **Timeout** | `2500 × (256/32000) = 20 seconds` |
| **Refresh** | Highest priority RTOS task (Priority 5) refreshes every 1 second |

---

### RTOS Application Tasks

7 concurrent tasks managed by the custom preemptive RTOS kernel:

| Task | Priority | Interval | Description |
|---|---|---|---|
| `Task_Watchdog` | 5 (highest) | 1000 ms | Refreshes IWDG to prevent system reset |
| `Task_SensorAcquisition` | 3 | 100 ms | Reads raw temperature, pressure, altitude from BMP280 via I2C |
| `Task_DSP_Processing` | 3 | 100 ms | Runs 3-stage DSP filter on raw data, computes vertical speed |
| `Task_GSM_Handler` | 2 | 50 ms | Monitors button and MQTT dashboard SMS trigger, sends SMS with cooldown |
| `Task_Telemetry_SPI` | 2 | 500 ms | Packs filtered data into `TelemetryPacket_t` and sends via SPI1 to ESP32 |
| `Task_Telemetry_UART` | 1 | 500 ms | Prints RAW and FILTERED data to USART2 debug console |
| `Task_ExampleHeartbeat` | 1 (lowest) | 2000 ms | Prints heartbeat message to verify system liveness |

---

## ESP32 Firmware — `esp32-spi-mqtt-telemetry/`

> **Target:** ESP32 DevKit V1  
> **Framework:** ESP-IDF with FreeRTOS  
> **SPI and MQTT drivers are baremetal — no ESP-IDF SPI slave API, no MQTT client library**

### Baremetal SPI Slave Driver

Direct register manipulation of the ESP32's SPI3 (VSPI) peripheral — bypasses ESP-IDF's SPI slave driver entirely.

| Feature | Implementation |
|---|---|
| **GPIO Routing** | Manual GPIO matrix configuration via `func_in_sel_cfg` / `func_out_sel_cfg` registers |
| **Pin Mux** | Direct `IO_MUX_REG` writes to set `PIN_FUNC_GPIO` and enable input (`FUN_IE`) |
| **Peripheral Clock** | Enabled via `DPORT_PERIP_CLK_EN_REG`, reset cleared via `DPORT_PERIP_RST_EN_REG` |
| **Mode Config** | Supports SPI Modes 0-3 via `ck_idle_edge` and `ck_out_edge` register bits |
| **Full Duplex** | `doutdin = 1`, both `usr_mosi` and `usr_miso` enabled |
| **Buffer Length** | 160 bits (20 bytes × 8), set in `slv_rdbuf_dlen` and `slv_wrbuf_dlen` |
| **Transaction Detection** | Polls `SPI3.slave.trans_done` with 2-second timeout |
| **Data Access** | Direct read/write to `SPI3.data_buf[]` hardware FIFO (no byte swapping needed) |
| **Response Loading** | `SPI_Slave_LoadResponse()` pre-loads TX FIFO before next CS assertion |

---

### Baremetal MQTT Protocol Stack

Hand-crafted MQTT 3.1.1 packet builder — constructs binary protocol packets from scratch.

| Packet Type | Function | Features |
|---|---|---|
| **CONNECT** | `mqtt_build_connect_packet()` | Client ID, optional username/password, keep-alive, clean session |
| **PUBLISH** | `mqtt_build_publish_packet()` | Topic + payload, QoS 0/1/2 support, retain flag, packet ID |
| **SUBSCRIBE** | `mqtt_build_subscribe_packet()` | Topic filter, packet ID, requested QoS |
| **DISCONNECT** | `mqtt_build_disconnect_packet()` | Clean disconnect (2 bytes) |

**MQTT Configuration:**

| Parameter | Value |
|---|---|
| **Broker** | `broker.hivemq.com:1883` |
| **Client ID** | `ESP32_BMP280_Device` |
| **Publish Topic** | `telemetry/bmp280` |
| **Subscribe Topic** | `command/sms` |
| **Protocol** | MQTT 3.1.1 (protocol level 4) |
| **Keep-Alive** | 60 seconds |
| **Transport** | Raw TCP socket (BSD sockets via lwIP) |

---

### SMS Controller — Bidirectional Command Channel

Bridges the MQTT dashboard to the STM32's GSM module through the SPI link:

```
Dashboard Button ──► MQTT "command/sms" ──► ESP32 mqtt_rx_task ──► sms_trigger_pending = true
                                                                          │
ESP32 SPI Slave ◄── STM32 SPI Master (next transaction) ◄── Load magic word 0xAA55AA55
                                                                          │
STM32 reads rx_packet.timestamp == 0xAA55AA55 ──► trigger_sms_from_dashboard = 1 ──► GSM_SendSMS()
```

---

### Telemetry Task & MQTT Publishing

The `spi_slave_task` in `telemetry_stm32.c` performs:

1. **Receive** — Waits for SPI transaction from STM32 (blocking)
2. **Parse** — Extracts `temperature`, `pressure`, `altitude`, `vertical_speed` from `TelemetryPacket_t`
3. **Log** — Prints raw hex dump and formatted values to serial console
4. **Publish** — Builds JSON payload and sends MQTT PUBLISH to `telemetry/bmp280`
5. **Respond** — If SMS trigger is pending, loads magic word `0xAA55AA55` into SPI TX buffer for next transaction

**JSON Payload Format:**
```json
{
  "temperature": 25.43,
  "pressure": 1013.25,
  "altitude": 120.50,
  "vertical_speed": 0.12
}
```

---

## Node-RED Dashboard

A pre-configured Node-RED flow (`node_red_dashboard_flow.json`) provides real-time visualization:

| Widget | Type | Range | Topic |
|---|---|---|---|
| **Temperature** | Gauge | -40 to 85 °C | `telemetry/bmp280` → `.temperature` |
| **Pressure** | Gauge | 300 to 1100 hPa | `telemetry/bmp280` → `.pressure` |
| **Altitude** | Gauge | -50 to 5000 m | `telemetry/bmp280` → `.altitude` |
| **Vertical Speed** | Gauge | -20 to 20 m/s | `telemetry/bmp280` → `.vertical_speed` |
| **Send SMS** | Button | — | Publishes `trigger_sms` to `command/sms` |

**Setup:** Import `node_red_dashboard_flow.json` into Node-RED → Install `node-red-dashboard` → Deploy → Access at `http://localhost:1880/ui`

---

## Hardware Setup & Pin Mapping

### STM32F446RE Pin Assignments

| Function | Port:Pin | Peripheral | AF | Notes |
|---|---|---|---|---|
| I2C1 SCL | PB8 | I2C1 | AF4 | Open-drain, internal pull-up |
| I2C1 SDA | PB9 | I2C1 | AF4 | Open-drain, internal pull-up |
| SPI1 SCK | PA5 | SPI1 | AF5 | Push-pull, fast speed |
| SPI1 MISO | PA6 | SPI1 | AF5 | Push-pull, fast speed |
| SPI1 MOSI | PA7 | SPI1 | AF5 | Push-pull, fast speed |
| SPI1 CS | PB6 | GPIO | — | Software-managed, push-pull output |
| USART2 TX | PA2 | USART2 | AF7 | Debug console |
| USART2 RX | PA3 | USART2 | AF7 | Debug console |
| GSM TX | PA9 | USART1 | AF7 | SIM900 module |
| GSM RX | PA10 | USART1 | AF7 | SIM900 module |
| GSM Button | PA8 | GPIO | — | Active-low, internal pull-up |

### ESP32 Pin Assignments

| Function | GPIO | SPI3/VSPI Signal | Direction |
|---|---|---|---|
| MOSI | GPIO 23 | VSPID_IN | Input (from STM32) |
| MISO | GPIO 19 | VSPIQ_OUT | Output (to STM32) |
| SCLK | GPIO 18 | VSPICLK_IN | Input (from STM32) |
| CS | GPIO 5 | VSPICS0_IN | Input (from STM32) |

### Inter-MCU Wiring (5 Wires)

```
STM32F446RE                    ESP32 DevKit
┌──────────┐                  ┌──────────┐
│ PA5 (SCK)├─────────────────►│ GPIO 18  │  SCLK
│ PA7 (MOSI)├────────────────►│ GPIO 23  │  MOSI
│ PA6 (MISO)│◄────────────────┤ GPIO 19  │  MISO
│ PB6 (CS) ├─────────────────►│ GPIO 5   │  CS
│ GND      ├─────────────────►│ GND      │  Common Ground
└──────────┘                  └──────────┘
```

---

## SPI Protocol — Packet Format

Both MCUs exchange a **20-byte `TelemetryPacket_t`** per SPI transaction:

```c
typedef struct {
    float    temperature;     // Offset 0x00  (4 bytes)
    float    pressure;        // Offset 0x04  (4 bytes)
    float    altitude;        // Offset 0x08  (4 bytes)
    float    vertical_speed;  // Offset 0x0C  (4 bytes)
    uint32_t timestamp;       // Offset 0x10  (4 bytes)
} TelemetryPacket_t;          // Total: 20 bytes
```

| Direction | Content | Special Values |
|---|---|---|
| **STM32 → ESP32** | DSP-filtered sensor data + incrementing timestamp | Normal data |
| **ESP32 → STM32** | Normally all zeros | `timestamp = 0xAA55AA55` = SMS trigger command |

---

## Data Flow Pipeline

```
  BMP280 Sensor
       │
       │ I2C1 (100 kHz)
       ▼
  ┌────────────────┐
  │ Raw ADC Values │ ──► Temperature Compensation (t_fine)
  │ (20-bit T + P) │ ──► Pressure Compensation (64-bit math)
  └────────┬───────┘ ──► Hypsometric Altitude Calculation
           │
           ▼
  ┌────────────────┐    ┌────────────────┐    ┌─────────────────┐
  │ Median Filter  │───►│ IIR Low-Pass   │───►│ 2D Kalman Filter│
  │ (Spike reject) │    │ (HF smoothing) │    │ (State + Vel.)  │
  └────────────────┘    └────────────────┘    └────────┬────────┘
                                                        │
                              ┌──────────────────────────┤
                              │                          │
                              ▼                          ▼
                        ┌───────────┐          ┌──────────────┐
                        │ USART2    │          │ SPI1 Master  │
                        │ Debug Log │          │ (20B packet) │
                        └───────────┘          └──────┬───────┘
                                                       │
                                                       │ Full-Duplex SPI
                                                       ▼
                                               ┌──────────────┐
                                               │ ESP32 SPI3   │
                                               │ Slave Driver │
                                               └──────┬───────┘
                                                       │
                                          ┌────────────┤
                                          │            │
                                          ▼            ▼
                                    ┌──────────┐ ┌─────────────┐
                                    │ MQTT     │ │ SMS Trigger  │
                                    │ Publish  │ │ Response     │
                                    │ (JSON)   │ │ (0xAA55AA55) │
                                    └────┬─────┘ └─────────────┘
                                         │
                                         │ TCP:1883
                                         ▼
                                    ┌──────────┐
                                    │ HiveMQ   │
                                    │ Broker   │
                                    └────┬─────┘
                                         │
                                         ▼
                                    ┌──────────┐
                                    │ Node-RED │
                                    │ Gauges   │
                                    └──────────┘
```

---

## Build & Flash Instructions

### STM32 Firmware

```bash
# Open in STM32CubeIDE
# Project: stm32-rtos-dsp-telemetry-system/
# Target: STM32F446RE
# Build: Project → Build All (Ctrl+B)
# Flash: Run → Debug As → STM32 C/C++ Application
# Debug Console: Connect USART2 (PA2/PA3) at 115200 baud
```

### ESP32 Firmware

```bash
# Requires ESP-IDF v4.4+ installed and sourced
cd esp32-spi-mqtt-telemetry/

# Configure Wi-Fi credentials in main/main.c (WIFI_SSID, WIFI_PASS)

# Build
idf.py build

# Flash (adjust port as needed)
idf.py -p COM3 flash

# Monitor serial output
idf.py -p COM3 monitor
```

### Node-RED Dashboard

```bash
# Install Node-RED (if not already installed)
npm install -g node-red

# Start Node-RED
node-red

# Open browser: http://localhost:1880
# Import: Menu → Import → Select node_red_dashboard_flow.json
# Install dashboard: Manage palette → Install → node-red-dashboard
# Deploy and access dashboard: http://localhost:1880/ui
```

---

## License

This project is licensed under the **MIT License** — see the [LICENSE](LICENSE) file for details.

**Copyright © 2026 Athul S**

---

<p align="center">
  <b>Built from the ground up — every register, every byte, every algorithm.</b>
</p>
