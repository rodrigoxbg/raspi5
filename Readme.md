# Library to control the Raspberry Pi 5.

## Description

Since the Raspberry Pi5 use the BCM2712 SoC, it separates the GPIO pins, I2C, SPI, and UART on a extra chip (RP1-C0). 
Therefore many of the libraries for the Raspberry Pi 4 and earlier such a BCM2835.h are not working anymore on this new SoC.

The inicial idea is to provide now a library to control the peripherals of the Raspberry Pi 5.

Until now, the library contains the following features:

- GPIO digital input and output control


## Installation

The process of installation requires some steps to let the library access to the RP1-C0 chip.
We support in native libraries for Linux versions.

1. Install the required packages:

   ```bash
   sudo apt update
   sudo apt install gpiod libgpiod-dev
   ```

2. Clone the repository
3. Enter to the folder created "raspi5" and execute the following commands
   '''bash
   make
   make setuplib
   make install
   '''

With that process is now possible to use the Library in the headers or the Terminal Commands. Explained below

# Functions of the Library
