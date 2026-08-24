# stm32-bluepill-edu

Educational STM32CubeIDE project for the STM32 Blue Pill development board based on the **STM32F103C6T6** microcontroller.

The project is intended as a minimal, clean starting point for learning STM32 development with STM32CubeIDE, STM32CubeMX, CMSIS and the STM32 Low Layer (LL) drivers.

## Features

* STM32CubeIDE project
* STM32CubeMX (`.ioc`) configuration included
* STM32F103C6T6 target MCU
* LL (Low Layer) drivers
* GCC ARM Embedded Toolchain
* Ready for Debug and Release builds
* Git-friendly project structure

## 📌 Project Status & Roadmap

* [x] **Project Setup:** Clean structure with automated build scripts.
* [x] **Cortex Core:** Configured proper NVIC interrupt priority grouping for LL drivers.
* [x] **GPIO:** Basic LED blinking and push-button handling using Low-Layer (LL) drivers.
* [x] **USART & DMA:** Asynchronous data transmission via **USART1 TX using DMA (Channel 4)**. Basic data reception via UART RX interrupts.
* [x] **Timers / PWM:** Implemented hardware fading breathing pattern on PA8 using **TIM1 CH1 PWM**.
* [ ] **ADC / DAC:** (Planned)

## Hardware

* **Board:** STM32 Blue Pill
* **MCU:** STM32F103C6T6
* **Package:** LQFP48
* **External HSE:** 8 MHz

## Software Requirements

* STM32CubeIDE 2.x (or compatible)
* STM32CubeMX (included in STM32CubeIDE)
* GNU Arm Embedded Toolchain (bundled with STM32CubeIDE)

## Project Structure

```text
Core/          Application source code (main.c, stm32f1xx_it.c)
Drivers/       CMSIS and STM32 LL drivers
Debug/         Debug build output (generated)
Release/       Release build output (generated)
```

## Building

### Using STM32CubeIDE
1. Open the project in STM32CubeIDE.
2. Select the desired build configuration: **Debug** or **Release**.
3. Build the project.

### Command Line
The project can also be built in headless mode using `stm32cubeidec.exe`.
Example:
```bash
stm32cubeidec_cleanBuild.bat Debug
```
or
```bash
stm32cubeidec_cleanBuild.bat Release
```

## Git

Generated files are excluded using `.gitignore`.
Typical generated directories: `Debug/`, `Release/`, `.settings/`.

## License

This project is released under the **MIT License**.

## Author

Anatoly Sharapov
Email: anatoly.a.sharapov@gmail.com
