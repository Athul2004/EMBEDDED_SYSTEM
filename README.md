# 🛰️ Embedded System Software Engineering

> **Upskilling Course Repository — [Entri App](https://entri.app)**

A comprehensive collection of projects, assignments, and driver implementations completed during the **Embedded System Software Engineering** course by Entri. The repository spans the full embedded stack — from foundational **C programming** and **AVR (ATmega328P)** bare-metal peripherals, through **ARM Cortex-M4 (STM32F446RE)** custom drivers and RTOS, to **IoT with ESP32 (ESP-IDF)** and cloud telemetry via MQTT.

---

## 📋 Table of Contents

- [Repository Structure](#-repository-structure)
- [Module 1 — C Programming Fundamentals](#-module-1--c-programming-fundamentals)
- [Module 2 — Assignment (C Problem Solving)](#-module-2--assignment-c-problem-solving)
- [Module 3 — AVR ATmega328P Bare-Metal Programming](#-module-3--avr-atmega328p-bare-metal-programming)
- [Module 4 — STM32 ARM Cortex-M4 (STM32F446RE)](#-module-4--stm32-arm-cortex-m4-stm32f446re)
- [Module 5 — FreeRTOS on STM32](#-module-5--freertos-on-stm32)
- [Module 6 — IoT with ESP32 & ESP-IDF](#-module-6--iot-with-esp32--esp-idf)
- [Mini Project — Energy Data Acquisition with Custom DSP (STM32)](#-mini-project--energy-data-acquisition-with-custom-dsp-stm32)
- [Main Project — STM32 + ESP32 Baremetal Data Acquisition & Telemetry System](#-main-project--stm32--esp32-baremetal-data-acquisition--telemetry-system)
- [Tech Stack & Tools](#-tech-stack--tools)
- [Learning Outcomes](#-learning-outcomes)

---

## 📁 Repository Structure

```
EMBEDDED_SYSTEM/
│
├── C_PROGRAMMING/                   # Module 1 — C language fundamentals
│   ├── basics_two_1/                # Variables, operators, control flow
│   ├── basics_three_2/              # Loops, conditionals
│   ├── functions_array_3/           # Functions & arrays
│   ├── B1_functions_array_4/        # Batch-1 functions & arrays practice
│   ├── B1_structure_pointer_5/      # Structures & pointers
│   ├── Bit_manipulation_Assessment_6/   # Bitwise operations assessment
│   ├── B_1Bit_manipulation_Assessment_7/# Batch-1 bitwise assessment
│   ├── Q_A_EMBEDDEDSYSTEM/          # 15 embedded C interview Q&A notes
│   └── interview_assesment/         # Interview coding assessment
│
├── Module_2_Assignment/             # Module 2 — C assignment problems
│   ├── Question_1/
│   ├── Question_2/
│   └── Question_3/
│
├── AVR_ATMEGA_328P/                 # Module 3 — AVR bare-metal peripherals
│   ├── PD3_BLINK_LED/               # Basic GPIO LED blink
│   ├── BLINK_TWO_LED_HW/            # Multi-LED toggling
│   ├── BLINK_5LED_USING_HEADER_FILE/# Modular LED control with headers
│   ├── ACTIVE_HIGH_SWITCH/          # Button input (active-high config)
│   ├── ACTIVE_LOW_SWITCH/           # Button input (active-low config)
│   ├── DC_MOTOR_SWITCH1/            # DC motor control via switch
│   ├── MOTOR_FUNC/ & MOTOR_FUNC_DELAY/ # Motor functions with timing
│   ├── DELAY_1024/ & DELAY_TIMER0/  # Software & hardware delay methods
│   ├── fast_pwm_timer_0/            # Fast PWM using Timer0
│   ├── timer0_CTM/                  # Timer0 CTC mode
│   ├── TIMER0_OVERFLOW_INTERRUPT/   # Timer0 overflow ISR
│   ├── FADING_LED_PWM/              # LED fading with PWM
│   ├── EXT_INTRRUPT/                # External interrupt handling
│   ├── ADC/                         # Analog-to-Digital Converter
│   ├── LM35/                        # LM35 temperature sensor
│   ├── 162_lcd/                     # 16×2 LCD display interfacing
│   ├── USART_TXT/                   # UART serial communication
│   ├── SPI/                         # SPI protocol
│   ├── SPI_BM280/ & BM280_pressure_sensing/ # BMP280 sensor via SPI
│   └── modular_1/                   # Modular code architecture
│
├── STM32-M4/                        # Module 4 — ARM Cortex-M4 drivers & projects
│   ├── LED_blink/ & led_blink_3/    # GPIO LED blink (register-level)
│   ├── STM32_drivers/               # Custom bare-metal peripheral drivers
│   │   └── drivers/                 #   GPIO, SPI, full MCU register map (45KB)
│   ├── I2C_realtime_clk/            # I2C real-time clock interfacing
│   ├── OLED_DISPLAY_I2C/            # SSD1306 OLED via I2C
│   ├── spi_bm280/                   # BMP280 sensor via SPI
│   ├── RTOS_SAMPLE_1/               # RTOS sample application
│   ├── Energy_Data_Aquisition_With_Custom_Dsp/ # DSP-based energy monitor
│   └── nucleo-f446re-scratch-master/ # Nucleo board scratch project
│
├── FreeRTOS/                        # Module 5 — Real-Time Operating System
│   └── Freertos-led-task-sheduler/  # 3-LED task scheduler with FreeRTOS
│
├── IOT_ESP-IDF/                     # Module 6 — IoT with ESP32
│   ├── esp_32_sample/               # Basic ESP32 blink (ESP-IDF)
│   ├── esp_spi_temp_pre/            # SPI temperature/pressure sensor
│   └── esp32-spi-mqtt-telemetry/    # MQTT telemetry with SPI slave driver
│
├── Mini_project_stm32/              # 🏆 Mini Project — Energy Data Acquisition
│   └── (STM32F446RE bare-metal energy monitor with custom DSP)
│
└── MAIN_PROJECT_STM32-ESP32/        # 🏆 Main Project — Dual-MCU Telemetry
    └── stm32-baremetal-data-acquisition-telemetry-system/
        ├── stm32-rtos-dsp-telemetry-system/   # STM32 firmware
        └── esp32-spi-mqtt-telemetry/          # ESP32 firmware
```

---

## 📘 Module 1 — C Programming Fundamentals

**Path:** [`C_PROGRAMMING/`](C_PROGRAMMING/)

Covers the foundational C programming concepts essential for embedded development:

| Sub-Module | Topics Covered |
|:---|:---|
| `basics_two_1` | Variables, data types, operators, basic I/O |
| `basics_three_2` | Loops, conditional statements, control flow |
| `functions_array_3` | Function declarations, arrays, passing by value |
| `B1_functions_array_4` | Additional practice — functions & arrays |
| `B1_structure_pointer_5` | Structures, pointers, pointer arithmetic |
| `Bit_manipulation_Assessment_6` | Bitwise AND/OR/XOR/NOT/Shift operations |
| `B_1Bit_manipulation_Assessment_7` | Advanced bitwise assessment |
| `Q_A_EMBEDDEDSYSTEM` | 15 embedded C interview Q&A notes (memory layout, volatile, malloc vs calloc, dangling pointers, structure padding, etc.) |
| `interview_assesment` | Interview-style coding problems (e.g., swap) |

---

## 📝 Module 2 — Assignment (C Problem Solving)

**Path:** [`Module_2_Assignment/`](Module_2_Assignment/)

Three structured C programming assignment problems with source code, compiled executables, and documented outputs.

---

## ⚡ Module 3 — AVR ATmega328P Bare-Metal Programming

**Path:** [`AVR_ATMEGA_328P/`](AVR_ATMEGA_328P/) — **23 projects**

Hands-on bare-metal programming on the ATmega328P covering the full peripheral set:

| Category | Projects | Key Concepts |
|:---|:---|:---|
| **GPIO** | `PD3_BLINK_LED`, `BLINK_TWO_LED_HW`, `BLINK_5LED_USING_HEADER_FILE` | Port registers (DDRx, PORTx, PINx), modular headers |
| **Switches** | `ACTIVE_HIGH_SWITCH`, `ACTIVE_LOW_SWITCH` | Digital input, pull-up/pull-down configurations |
| **Motor Control** | `DC_MOTOR_SWITCH1`, `MOTOR_FUNC`, `MOTOR_FUNC_DELAY` | H-bridge driving, function-based motor control |
| **Timers** | `DELAY_1024`, `DELAY_TIMER0`, `timer0_CTM`, `TIMER0_OVERFLOW_INTERRUPT` | Timer0 prescalers, CTC mode, overflow interrupts |
| **PWM** | `fast_pwm_timer_0`, `FADING_LED_PWM` | Fast PWM generation, LED brightness control |
| **Interrupts** | `EXT_INTRRUPT`, `TIMER0_OVERFLOW_INTERRUPT` | External interrupts (INT0/INT1), ISR handlers |
| **ADC** | `ADC`, `LM35` | Analog-to-Digital conversion, temperature sensing |
| **Display** | `162_lcd` | 16×2 character LCD interfacing (4-bit/8-bit mode) |
| **Communication** | `USART_TXT`, `SPI`, `SPI_BM280`, `BM280_pressure_sensing` | UART, SPI protocols, BMP280 sensor integration |
| **Architecture** | `modular_1` | Modular code organization with header files |

---

## 🔧 Module 4 — STM32 ARM Cortex-M4 (STM32F446RE)

**Path:** [`STM32-M4/`](STM32-M4/)

Register-level (bare-metal) driver development on the STM32F446RE Nucleo board — no HAL libraries:

| Project | Description |
|:---|:---|
| `LED_blink` / `led_blink_3` | GPIO output via direct register manipulation |
| `STM32_drivers` | **Custom peripheral driver library** — full MCU register map header (45 KB), GPIO driver, SPI driver |
| `I2C_realtime_clk` | I2C master communication with real-time clock module |
| `OLED_DISPLAY_I2C` | SSD1306 OLED display driver over I2C |
| `spi_bm280` | BMP280 pressure/temperature sensor via SPI |
| `RTOS_SAMPLE_1` | Introduction to RTOS concepts on STM32 |
| `Energy_Data_Aquisition_With_Custom_Dsp` | Real-time energy monitor with custom DSP pipeline |
| `nucleo-f446re-scratch-master` | Nucleo board from-scratch project template |

---

## 🔄 Module 5 — FreeRTOS on STM32

**Path:** [`FreeRTOS/`](FreeRTOS/)

| Project | Description |
|:---|:---|
| `Freertos-led-task-sheduler` | 3-LED task scheduler using FreeRTOS — demonstrates `xTaskCreate()`, `vTaskDelay()`, `vTaskStartScheduler()`, and preemptive multitasking with SWV ITM debug console output |

**MCU:** STM32F446RE &nbsp;|&nbsp; **RTOS:** FreeRTOS &nbsp;|&nbsp; [📖 Detailed README](FreeRTOS/Freertos-led-task-sheduler/README.md)

---

## 🌐 Module 6 — IoT with ESP32 & ESP-IDF

**Path:** [`IOT_ESP-IDF/`](IOT_ESP-IDF/)

| Project | Description |
|:---|:---|
| `esp_32_sample` | Basic ESP32 blink project using ESP-IDF |
| `esp_spi_temp_pre` | SPI-based temperature & pressure sensor reading |
| `esp32-spi-mqtt-telemetry` | **Bare-metal SPI slave driver** + **hand-crafted MQTT 3.1.1 protocol stack** — receives telemetry from STM32 and publishes JSON to HiveMQ cloud broker |

---

## 🏆 Mini Project — Energy Data Acquisition with Custom DSP (STM32)

**Path:** [`Mini_project_stm32/`](Mini_project_stm32/) &nbsp;|&nbsp; [📖 Detailed README](Mini_project_stm32/README.md)

A high-precision **Energy Data Acquisition & Monitoring System** on the STM32F446RE:

- **ADC + DMA** — high-speed sampling with zero-CPU-overhead DMA transfers
- **Hardware Timer Triggering** — TIM2 TRGO at 8000 Hz for jitter-free sampling
- **Double Buffering** — continuous DSP processing via half/complete DMA transfer interrupts
- **Custom DSP Pipeline** — RMS voltage/current, active power, power factor, frequency (zero-crossing), energy (kWh)
- **Bare-Metal Drivers** — custom ADC, Timer, I2C, UART, SSD1306 OLED drivers
- **Sensors** — ZMPT101B (AC voltage), ACS712 (AC current)
- **Output** — live metrics on SSD1306 OLED + UART serial logging

---

## 🏆 Main Project — STM32 + ESP32 Baremetal Data Acquisition & Telemetry System

**Path:** [`MAIN_PROJECT_STM32-ESP32/`](MAIN_PROJECT_STM32-ESP32/) &nbsp;|&nbsp; [📖 Detailed README](MAIN_PROJECT_STM32-ESP32/stm32-baremetal-data-acquisition-telemetry-system/README.md)

A **production-grade, dual-MCU embedded telemetry system** built entirely from scratch — **zero HAL, zero third-party RTOS, zero MQTT client libraries**:

| Component | Implementation |
|:---|:---|
| **Custom RTOS Kernel** | Preemptive kernel with PendSV context switching (ARM assembly), TCBs, semaphores, mutexes, queues, event groups, software timers, heap allocator |
| **DSP Pipeline** | 3-stage cascade — Median → IIR Low-Pass → 2D Kalman (with velocity estimation) |
| **Peripheral Drivers** | 100% register-level bare-metal: GPIO, I2C, SPI, USART, RCC, IWDG (STM32); SPI3 slave (ESP32) |
| **MQTT Stack** | Hand-crafted MQTT 3.1.1 packet builder (CONNECT, PUBLISH, SUBSCRIBE, DISCONNECT) |
| **SPI Telemetry Link** | Full-duplex SPI1 ↔ SPI3/VSPI with bidirectional command channel |
| **SMS Alerts** | SIM900 GSM module with AT commands — triggerable from button or MQTT dashboard |
| **Cloud Dashboard** | Node-RED with live gauges for temperature, pressure, altitude, vertical speed |
| **Watchdog** | Independent Watchdog (IWDG) with 20 s timeout, refreshed by highest-priority RTOS task |

---

## 🛠️ Tech Stack & Tools

| Category | Technologies |
|:---|:---|
| **Languages** | C, ARM Assembly |
| **MCUs** | ATmega328P (AVR), STM32F446RE (ARM Cortex-M4), ESP32 |
| **IDEs / Toolchains** | Atmel Studio, STM32CubeIDE, ARM GCC, ESP-IDF |
| **Protocols** | GPIO, ADC, PWM, UART, SPI, I2C, MQTT 3.1.1 |
| **RTOS** | FreeRTOS, Custom Bare-Metal RTOS Kernel |
| **Sensors** | BMP280, ZMPT101B, ACS712, LM35 |
| **Displays** | 16×2 LCD, SSD1306 OLED (I2C) |
| **IoT / Cloud** | ESP-IDF, HiveMQ MQTT Broker, Node-RED Dashboard |
| **Debugging** | UART Serial, SWV ITM Console, ST-Link |

---

## 🎯 Learning Outcomes

Upon completion of this course and all projects, the following skills and competencies were developed:

### Embedded C & Low-Level Programming
- ✅ Proficient in **C programming** for resource-constrained embedded systems
- ✅ Deep understanding of **memory layout** (stack, heap, data, BSS, text segments)
- ✅ Mastery of **bitwise operations** for register manipulation
- ✅ Understanding of **pointers, structures, unions**, and dynamic memory management
- ✅ Knowledge of **volatile, inline functions**, and **MISRA-C** coding standards

### Microcontroller Architecture & Peripherals
- ✅ Bare-metal programming on **AVR (ATmega328P)** — GPIO, Timers, ADC, PWM, Interrupts
- ✅ Register-level driver development on **ARM Cortex-M4 (STM32F446RE)** — GPIO, I2C, SPI, USART, RCC, DMA, ADC
- ✅ Understanding of **clock trees, bus architectures** (APB1/APB2), and **linker scripts**

### Communication Protocols
- ✅ Implemented **UART** serial communication for debugging and data logging
- ✅ Implemented **SPI** (Master/Slave, full-duplex) for inter-MCU and sensor communication
- ✅ Implemented **I2C** (Master mode) for sensor and display interfacing
- ✅ Built a **hand-crafted MQTT 3.1.1 protocol stack** from scratch (no libraries)

### Real-Time Operating Systems (RTOS)
- ✅ Built applications using **FreeRTOS** — task creation, scheduling, delays, multitasking
- ✅ Designed and implemented a **custom preemptive RTOS kernel** from scratch — PendSV context switching (ARM assembly), TCBs, semaphores, mutexes, queues, event groups, software timers, and heap allocation

### Digital Signal Processing (DSP)
- ✅ Designed a **multi-stage DSP filter pipeline** — Median, IIR Low-Pass, 2D Kalman Filter
- ✅ Implemented **RMS calculations, power factor, zero-crossing frequency detection**, and energy accumulation
- ✅ Leveraged the **Cortex-M4 FPU** for efficient floating-point DSP

### IoT & Cloud Integration
- ✅ Developed firmware on **ESP32 using ESP-IDF** framework
- ✅ Built **bare-metal SPI slave drivers** on ESP32 (register-level, no ESP-IDF SPI APIs)
- ✅ Designed **cloud telemetry pipelines** — sensor → SPI → MQTT → HiveMQ → Node-RED Dashboard
- ✅ Integrated **GSM/SMS alerting** (SIM900) with remote MQTT-triggered commands

### System Design & Integration
- ✅ Designed **dual-MCU architectures** (STM32 + ESP32) with robust inter-MCU SPI communication
- ✅ Implemented **watchdog timers (IWDG)** for system reliability
- ✅ Built **double-buffered DMA data acquisition** pipelines for zero data loss
- ✅ Created **modular, driver-layered code architectures** following industry best practices

---

<p align="center">
  <b>Built from the ground up — every register, every byte, every algorithm.</b>
  <br><br>
  <i>Course Platform: <a href="https://entri.app">Entri App</a></i>
</p>
