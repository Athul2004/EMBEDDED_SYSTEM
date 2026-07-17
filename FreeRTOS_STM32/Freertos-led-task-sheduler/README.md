# FreeRTOS 3-LED Task Scheduler

**[▶️ Click here to watch the project demonstration video](https://drive.google.com/file/d/1x0JOdjZVTdh36-vWd1Ogwj-MHpAqoiz4/view?usp=sharing)**

This project demonstrates a basic FreeRTOS implementation on an STM32 microcontroller. It utilizes the FreeRTOS task scheduler to manage and toggle three different LEDs (Green, Blue, and Red) at different frequencies, alongside sending status messages over swv itm data console.

## Project Overview

The `main.c` file configures the hardware and creates three separate FreeRTOS tasks, each responsible for toggling a specific LED:

- **Green LED Task**: Toggles the Green LED every 1000 ms (1 second).
- **Blue LED Task**: Toggles the Blue LED every 500 ms (0.5 seconds).
- **Red LED Task**: Toggles the Red LED every 250 ms (0.25 seconds).

Each task runs in an infinite loop and also prints its respective color name via `printf` to the serial console (configured on USART2) each time it executes.

## FreeRTOS Implementation Details

This project relies on several core FreeRTOS APIs to achieve multitasking for the LED blinking:

1. **`xTaskCreate()`**: Used in `main()` to instantiate the three separate tasks (`Green LED Task`, `Blue LED Task`, `Red LED Task`). Each task is given a priority of `2`, a stack size of 100 words, and a specific handler function (e.g., `green_led_handler`).
2. **Task Parameters**: The name of the color (e.g., `"Green"`) is passed as a pointer parameter (`pvParameters`) into each task handler when created, which is then used by the `printf` statement.
3. **`vTaskStartScheduler()`**: Called after all tasks are created to hand over control to the FreeRTOS kernel, starting the time-slicing and execution of the initialized tasks.
4. **`vTaskDelay(pdMS_TO_TICKS(x))`**: Inside each task's infinite `while(1)` loop, this API is used instead of a standard blocking delay (like `HAL_Delay`). `vTaskDelay` blocks the current task for the specified time, yielding CPU time to other tasks that are ready to run, ensuring efficient multitasking.
5. **`taskYIELD()`**: Used after `vTaskDelay` to explicitly request a context switch (even though `vTaskDelay` already handles yielding for the blocking period).


## Hardware Configuration
- **Microcontroller**: STM32F4 series (STM32F446RETx).
- **LED Pins**: Configured as outputs on GPIO Port A (`Green_LED_Pin`, `Blue_LED_Pin`, `Red_LED_Pin`).
- **Debug Console**: `printf` is retargeted to the SWV (Serial Wire Viewer) ITM Data Console for outputting task status messages.

