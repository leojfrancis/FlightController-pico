#include "setup.hpp"
#include "utils.hpp"

uint32_t wrap = 16;

uint32_t _pwm_set_freq_duty(uint slice_num,
                            uint chan, uint32_t f, double_t d)
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

uint32_t _pwm_set_duty(uint gpio, double_t d)
{
   // pwm_set_chan_level(slice_num, chan, wrap * d / 100);
   pwm_set_gpio_level(gpio, wrap * d / 100);
   return wrap;
}

uint _setup_motor(uint pin, uint chan)
{
   gpio_set_function(pin, GPIO_FUNC_PWM);
   sleep_ms(10);
   uint slice_num = pwm_gpio_to_slice_num(pin);
   _pwm_set_freq_duty(slice_num, chan, 50, 5);
   pwm_set_enabled(slice_num, true);
   return slice_num;
}

void calibrate_thrust(uint pin, uint pwm_chan, double_t power)
{
   _setup_motor(pin, pwm_chan);
   sleep_ms(2000);
   _pwm_set_duty(pin, 10);
   sleep_ms(2000);
   _pwm_set_duty(pin, map(power, 0, 10, 5, 10));
}

void calibrate_servo(uint pin, uint pwm_chan)
{
   _setup_motor(pin, pwm_chan);
   sleep_ms(100);
   _pwm_set_duty(pin, 10);
   sleep_ms(100);
   _pwm_set_duty(pin, map(0, -90, 90, 5, 10));
}

void thrust(uint pin, double_t power)
{
   _pwm_set_duty(pin, map(power, 0, 10, 5, 10));
}

/** \brief Set servo angle
 *  \ingroup motor pwm
 *
 * Control servo
 *
 * \param pin PWM slice number
 * \param pwm_chan  pwm channel
 * \param angle  angle to move
 */
void servo_angle(uint pin, double_t angle)
{
   _pwm_set_duty(pin, map(angle, -90, 90, 5, 10));
}
