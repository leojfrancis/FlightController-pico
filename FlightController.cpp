#include "src/setup.hpp"
#include "src/mpu6050.cpp"
#include "src/motor.hpp"
#include "src/pid.hpp"
#include "src/utils.hpp"

double_t motor_temp = 0;
bool hitmax = false;

mpu6050_inst_t init_imu()
{
    gpio_set_function(IMU_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(IMU_SCL_PIN, GPIO_FUNC_I2C);

    gpio_pull_up(IMU_SDA_PIN);
    gpio_pull_up(IMU_SCL_PIN);
    mpu6050_inst_t mpu6050;
    if (mpu6050_init(&mpu6050, i2c_imu, PICO_DEFAULT_LED_PIN))
    {
        puts("mpu6050 not found\n");
    }
    return mpu6050;
}
void init_motor()
{
    // motor setup

    puts("Calibrating Motors...");
    calibrate_servo(RIGHT_ALIGN, PWM_CHAN_A);
    calibrate_servo(BACK_ALIGN, PWM_CHAN_B);
    calibrate_servo(LEFT_ALIGN, PWM_CHAN_A);
    calibrate_servo(FRONT_ALIGN, PWM_CHAN_B);
    sleep_ms(50);
    calibrate_thrust(MAIN_THRUST, PWM_CHAN_B, 0);
}

int64_t timer_callback(alarm_id_t id, void *user_data)
{
    if (motor_temp == 10.0)
        hitmax = true;
    if (hitmax == true)
    {
        motor_temp -= 0.5;
        if (motor_temp == 9.5)
            return FINAL_THRUST_STAY;
    }
    else
        motor_temp += 0.5;
    return THRUST_STEP;
}

int main()
{
    stdio_init_all();
    tight_loop_contents();
    mpu6050_inst_t mpuObj;
    mpuObj = init_imu();

    init_motor();

    // Time since boot
    absolute_time_t timer = get_absolute_time();

    add_alarm_in_ms(START_THRUST_AFTER, timer_callback, NULL, false);

    PID pidRoll(1.1, 29.3, 0.0000005, 0.0002);
    PID pidPitch(1.2, 29.3, 0.0000005, 0.0002);
    PID pidYaw(1.9, 29.3, 0.0000005, 0.0002);
    // PID pidH(1, 2930, 0.000005, 0.0002);

    pidRoll.set_clamp(-360, +360);
    pidPitch.set_clamp(-360, +360);
    pidYaw.set_clamp(-360, +360);
    // pidH.set_clamp(0, 10);

    // temp debug
    srand((unsigned)time(NULL));
    long max[4], min[4], temp[4] = {0.0, 0.0, 0.0, 0.0};

    Vector3 ROT = {
        mpu6050_get_roll(&mpuObj),  //  x -180 < roll < 180
        mpu6050_get_pitch(&mpuObj), // y -90 < pitch < 90
        mpu6050_get_yaw(&mpuObj)    // z -180 < yaw < 180
        // (double)-180 + rand() % 361, (double)-180 + rand() % 181, (double)-180 + rand() % 361;
    };

    sleep_ms(100);
    for (;;)
    {
        for (;;)
        {

            if (mpu6050_update_state(&mpuObj))
            {
                printf("mpu6050 not found or disconnected\n");
                puts("Trying Again ... \n");
                sleep_ms(1500);
                init_motor();
                motor_temp = 0;
                hitmax = false;
            }
            puts("MPU Done...");
            break;
        }
        Vector3 MPU = {
            mpu6050_get_roll(&mpuObj),  //  x -180 < roll < 180
            mpu6050_get_pitch(&mpuObj), // y -90 < pitch < 90
            mpu6050_get_yaw(&mpuObj)    // z -180 < yaw < 180
            // (double)-180 + rand() % 361, (double)-180 + rand() % 181, (double)-180 + rand() % 361;
        };
        printf("Roll:%.2f - Pitch:%.2f - Yaw:%.2f \n", MPU.roll, MPU.pitch, MPU.yaw);

        double abs_time = absolute_time_diff_us(timer, get_absolute_time());
        double updated_roll = pidRoll.update(ROT.roll, MPU.roll, abs_time);
        double updated_pitch = pidPitch.update(ROT.pitch, MPU.pitch, abs_time);
        double updated_yaw = pidYaw.update(ROT.yaw, MPU.yaw, abs_time);
        // double updated_height = pidH.update(ROT.yaw, MPU.yaw, abs_time);
        printf("updated_roll:%.2f - updated_pitch:%.2f - updated_yaw:%.2f \n", updated_roll, updated_pitch, updated_yaw);
        // printf("abs_time:%.2f \n", abs_time);

        // {
        //     max[0] = maximum(temp[0], tvX);
        //     max[1] = maximum(temp[1], tvY);
        //     max[2] = maximum(temp[2], tvXY);
        //     max[3] = maximum(temp[3], abs_time);
        //     min[0] = maximum(temp[0], tvX);
        //     min[1] = maximum(temp[1], tvY);
        //     min[2] = maximum(temp[2], tvXY);
        //     min[3] = maximum(temp[3], abs_time);
        //     temp[0] = tvX;
        //     temp[1] = tvY;
        //     temp[2] = tvXY;
        //     temp[3] = abs_time;
        //     for (int i = 0; i < 4; i++)
        //     {
        //         printf("max %d- %f ", i, max[i]);
        //         printf("min %d- %f \n", i, max[i]);
        //     }
        // }

        servo_angle(RIGHT_ALIGN, map(ANGLE_GAIN * (updated_roll + ROT_CORRECTION * updated_pitch), -360, +360, -90, 90)); // RY
        servo_angle(LEFT_ALIGN, map(-ANGLE_GAIN * (updated_roll + ROT_CORRECTION * updated_pitch), -360, +360, -90, 90)); // RX
        servo_angle(FRONT_ALIGN, map(ANGLE_GAIN * (updated_yaw + ROT_CORRECTION * updated_pitch), -360, +360, -90, 90));  // FY
        servo_angle(BACK_ALIGN, map(-ANGLE_GAIN * (updated_yaw + ROT_CORRECTION * updated_pitch), -360, +360, -90, 90));  // FX
        thrust(MAIN_THRUST, motor_temp);

        sleep_ms(10);
        timer = get_absolute_time();
    }
    return true;
}
