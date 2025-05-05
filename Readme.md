# Library to control the Raspberry Pi 5.

## Description

Since the Raspberry Pi5 use the BCM2712 SoC, it separates the GPIO pins, I2C, SPI, and UART on a extra chip (RP1-C0). 
Therefore many of the libraries for the Raspberry Pi 4 and earlier such a BCM2835.h are not working anymore on this new SoC.

The inicial idea is to provide now a library to control the peripherals of the Raspberry Pi 5.

Until now, the library contains the following features:

- GPIO digital input and output control
- SPI communication 


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
   ```bash
   make
   make setuplib
   make install
   ```

With that process is now possible to use the Library in the headers or the Terminal Commands. Explained below

# Functions of the Library (Shell Commands)

Use the following commands to check the functionality of the Library
   ```bash
      raspi5            <-- To verify the correct instalation and version info
      raspi5 -pinout    <-- To see the PinOut of the Raspberry Pi 5
      raspi5 -help      <-- To see the functions of the Library

      raspi5 -testspi   <-- To test the SPI communication
   ```

Use the following commands to single control a GPIO

   ```bash
      raspi5 setgpio 13    <-- Put the GPIO13 in HIGH level
      raspi5 cleargpio 13  <-- Put the GPIO13 in LOW level
   ```

# Functions of the Library (Raspi5.h)

This is a extension library to control the Raspberry Pi 5 in C++ the following are the functions inside the library

## Digital Pin control

With this is posible to set, clear, read, toggle GPIOs.

   - setValue(1);
   - getValue();
   - Toggle();

## SPI protocol

With this is possible to control/read a external SPIs devices

   -setBitOrder(SPI_MSB_FIRST)
   -setDataMode(SPI_MODE_0)
   -set ClockDivider(divider = 500000)

   -transfer(tx,rx,length)
   -sendCommand(cmd)
   -sendData(data,length)
   -receiveData(rx,length)

# Information

This Library is in develop, so more functions and stuff will be added with the time. Thanks for support.