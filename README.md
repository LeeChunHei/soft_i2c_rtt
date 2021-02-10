# Soft I2C Driver

[Chinese](README_ZH.md) | English

## 1. Introduction

This software package is a driver for soft I2C. This driver provide access through I2C bus device model.

### 1.1 License

Soft I2C driver package is licensed with MIT license.

### 1.2 Dependency

- RT-Thread 4.0+
- PIN device driver

## 2. How to use ST7735R TFT LCD Driver

To use soft I2C driver package, you need to select it in the package manager of RT-Thread. The specific path is as follows:

```
RT-Thread online packages
    peripheral libraries and drivers --->
        [*] Soft I2C driver package --->
            [*] Setup soft I2C in menuconfig --->
                (i2c10)     Soft I2C bus name
                (100000)    Baudrate (Hz)
                ()          GPIO port number for the SCL pin
                ()          GPIO pin number for the scl pin
                ()          GPIO port number for the SDA pin
                ()          GPIO pin number for the sda pin
                [ ]         Use rt_hw_us_delay function
```

The detailed description of the package options is as follows:

| Option | Description |
|-|-|
| Setup soft I2C in menuconfig | Whether the soft I2C bus device initalized when rt-thread boot up |
| Soft I2C bus name | Register name for the soft I2C bus |
| Baudrate (Hz) | The baudrate of the I2C bus in Hz |
| GPIO port number for the SCL pin | The scl pin is (pin number) of pin in the (port number) of GPIO port |
| GPIO pin number for the scl pin | The scl pin is (pin number) of pin in the (port number) of GPIO port |
| GPIO port number for the SDA pin | The sda pin is (pin number) of pin in the (port number) of GPIO port |
| GPIO pin number for the sda pin | The sda pin is (pin number) of pin in the (port number) of GPIO port |
| Use rt_hw_us_delay function | Select this if the project had provided the implementation of rt_hw_us_delay, or else the max baudrate the soft i2c can reach is 500Hz if RT_TICK_PER_SECOND is 1000 |

After selecting the options you need, use RT-Thread's package manager to automatically update, or use the `pkgs --update` command to update the package to the BSP.

## 3. Use soft I2C Driver

After opening the soft I2C driver package and selecting the corresponding function option, it will be added to the BSP project for compilation when the BSP is compiled.
Burn the program to the target development board, and the user can treat the bus as a real I2C bus, the check the documentaion of the API [here](https://www.rt-thread.org/document/site/programming-manual/device/i2c/i2c/)