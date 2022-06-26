#include "setup.hpp"

uint setup_motor(uint pin, uint chan);

uint32_t wrap = 16;

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

uint32_t pwm_set_duty(uint gpio, uint8_t d)
{
   // pwm_set_chan_level(slice_num, chan, wrap * d / 100);
   pwm_set_gpio_level(gpio, wrap * d / 100);
   return wrap;
}

// int test_motor()
// {
//    stdio_init_all();

//    gpio_set_function(PWM_PIN, GPIO_FUNC_PWM);
//    uint slice_num = pwm_gpio_to_slice_num(PWM_PIN);
//    pwm_set_freq_duty(slice_num, PWM_CHAN_A, 50, 5);
//    pwm_set_enabled(slice_num, true);
//    sleep_ms(10000);
//    // pwm_set_duty(slice_num, PWM_CHAN_A, 7);

//    while (1)
//    {
//    }
//    return true;
// }

uint setup_motor(uint pin, uint chan)
{
   gpio_set_function(pin, GPIO_FUNC_PWM);
   sleep_ms(10);
   uint slice_num = pwm_gpio_to_slice_num(pin);
   printf("%d",slice_num);
   pwm_set_freq_duty(slice_num, chan, 50, 5);
   pwm_set_enabled(slice_num, true);
   sleep_ms(50);
   return slice_num;
}
