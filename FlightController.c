#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
// #include "hardware/spi.h"
// #include "hardware/i2c.h"
// #include "hardware/pio.h"
// #include "hardware/timer.h"

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT spi0
#define PIN_MISO 16
#define PIN_CS 17
#define PIN_SCK 18
#define PIN_MOSI 19

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9

// PWM defines
#define PWM_PIN 2
#define PWM_CLC "8ns"
uint32_t wrap = 16;

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint32_t pwm_set_freq_duty(uint slice_num,
                           uint chan, uint32_t f, int d)
{
    uint32_t clock = 125000000;
    uint32_t divider16 = clock / f / 4096 +
                         (clock % (f * 4096) != 0); // Understand whats happining in the calculation for clk_divider.
    if (divider16 / 16 == 0)
        divider16 = 16;
    wrap = clock * 16 / divider16 / f - 1;
    pwm_set_clkdiv_int_frac(slice_num, divider16 / 16,
                            divider16 & 0xF);
    pwm_set_wrap(slice_num, wrap);
    pwm_set_chan_level(slice_num, chan, wrap * d / 100);
    return wrap;
}

uint32_t pwm_set_duty(uint slice_num, uint chan, int d)
{
    pwm_set_chan_level(slice_num, chan, wrap * d / 100);
    return wrap;
}

int main()
{
    stdio_init_all();

    gpio_set_function(PWM_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(PWM_PIN);
    pwm_set_freq_duty(slice_num, PWM_CHAN_A, 50, 5);
    pwm_set_enabled(slice_num, true);
    sleep_ms(10000);
    pwm_set_duty(slice_num, PWM_CHAN_A, 7);

    while (1)
    {
    }
    return true;
}
