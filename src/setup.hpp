#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include <random>

#ifndef _SETUP_H
#define _SETUP_H

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(__clang__)
#define __is_trivially_destructible(arg) __has_trivial_destructor(arg)
#include <type_traits>
#endif

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define i2c_imu i2c0
#define IMU_SDA_PIN 16
#define IMU_SCL_PIN 17

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define spi_lora spi0
#define PIN_MISO 16
#define PIN_CS 17
#define PIN_SCK 18
#define PIN_MOSI 19

// PWM defines
#define PWM_PIN 2
#define PWM_CLC "8ns" // just for reference!

    // PID stuff
    typedef struct
    {
        double x;
        double y;
        double z;
    } Vector3;
#define ANGLE_GAIN 10
#define ROT_CORRECTION 0.1

#ifdef __cplusplus
}
#endif
#endif