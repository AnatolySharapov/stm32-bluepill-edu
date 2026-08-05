# stm32-bluepill-edu

Educational STM32CubeIDE project for the **STM32 Blue Pill** development board based on the **STM32F103C6T6** microcontroller.

The project is intended as a minimal, clean starting point for learning STM32 development with **STM32CubeIDE**, **STM32CubeMX**, CMSIS and the STM32 Low Layer (LL) drivers.

## Features

- STM32CubeIDE project
- STM32CubeMX (`.ioc`) configuration included
- STM32F103C6T6 target MCU
- LL (Low Layer) drivers
- GCC ARM Embedded Toolchain
- Ready for Debug and Release builds
- Git-friendly project structure

### 📌 Project Status & Roadmap

* [x] **Project Setup:** Clean structure with automated build scripts.
* [x] **GPIO:** Basic LED blinking and push-button handling using Low-Layer (LL) drivers.
* [x] **USART:** Basic data exchange via USART1 interface.
* [ ] **Timers / PWM:** *(Planned)*
* [ ] **ADC / DAC:** *(Planned)*

## Hardware

- Board: STM32 Blue Pill
- MCU: STM32F103C6T6
- Package: LQFP48
- External HSE: 8 MHz

## Software Requirements

- STM32CubeIDE 2.x (or compatible)
- STM32CubeMX (included in STM32CubeIDE)
- GNU Arm Embedded Toolchain (bundled with STM32CubeIDE)

## Project Structure

```text
Core/               Application source code
Drivers/            CMSIS and STM32 LL drivers
Debug/              Debug build output (generated)
Release/            Release build output (generated)
stm32-bluepill-edu.ioc
.project
.cproject
```

## Building

### Using STM32CubeIDE

1. Open the project in STM32CubeIDE.
2. Select the desired build configuration:
   - **Debug**
   - **Release**
3. Build the project.

### Command Line

The project can also be built in headless mode using `stm32cubeidec.exe`.

Example:

```cmd
stm32cubeidec_cleanBuild.bat Debug
```

or

```cmd
stm32cubeidec_cleanBuild.bat Release
```

## Git

Generated files are excluded using `.gitignore`.

Typical generated directories:

- `Debug/`
- `Release/`
- `.settings/`
- build artifacts
- temporary files

## License

This project is released under the **MIT License**.

## Author

**Anatoly Sharapov**

Email: anatoly.a.sharapov@gmail.com