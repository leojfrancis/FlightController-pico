#include "src/setup.hpp"
#include "src/mpu6050.cpp"
#include "src/motor.hpp"
#include "src/pid.hpp"
#include "src/utils.hpp"

mpu6050_inst_t init_imu()
{
    gpio_set_function(IMU_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(IMU_SCL_PIN, GPIO_FUNC_I2C);

    gpio_pull_up(IMU_SDA_PIN);
    gpio_pull_up(IMU_SCL_PIN);
    mpu6050_inst_t mpu6050;
    if (mpu6050_init(&mpu6050, i2c_imu, PICO_DEFAULT_LED_PIN))
    {
        printf("mpu6050 not found\n");
        for (;;)
        {
            printf("mpu6050 not found\n");
            sleep_ms(1);
        }
    }
    puts("mpu6050 found\n");
    return mpu6050;
}
void init_motor()
{
    // motor setup
    setup_motor(10, PWM_CHAN_A);
    setup_motor(11, PWM_CHAN_B);
    setup_motor(12, PWM_CHAN_A);
    setup_motor(13, PWM_CHAN_B);

    setup_motor(21, PWM_CHAN_B); // Main Thrust
}
int main()
{
    stdio_init_all();
    init_motor();

    uint counter = 0;
    mpu6050_inst_t mpu6050;
    // mpu6050 = init_imu();

    // Time since boot
    absolute_time_t timer = get_absolute_time();

    // Targets
    Vector3 ROT = {0, 0, 0};

    PID pid_tx(2, 0.5, 0.6);
    PID pid_ty(2, 0.5, 0.6);

    PID pid_rot_z(0, 0, 0);
    PID pid_pos_z(2, 0.5, 0.6);

    // pid_tx.set_clamp(-360.0, 360.0);
    // pid_ty.set_clamp(-180.0, 180.0);
    // pid_pos_z.set_clamp(0, 1);

    // temp debug
    srand((unsigned)time(NULL));
    long max[4], min[4], temp[4] = {0.0, 0.0, 0.0, 0.0};

    sleep_ms(100);
    for (;;)
    {
        // if (mpu6050_update_state(&mpu6050))
        // {
        //     printf("mpu6050 disconnected\n");
        //     return 1;
        // }
        Vector3 MPU_rpy = {
            // mpu6050_get_roll(&mpu6050),  //  x -180 < roll < 180
            // mpu6050_get_pitch(&mpu6050), // y -90 < pitch < 90
            // mpu6050_get_yaw(&mpu6050)    // z -180 < yaw < 180
            (double)-180 + rand() % 361, (double)-180 + rand() % 181, (double)-180 + rand() % 361};
        // printf("Roll:%f - Pitch:%f - Yaw:%f \n", MPU_rpy.x, MPU_rpy.y, MPU_rpy.z);

        double abs_time = absolute_time_diff_us(timer, get_absolute_time());
        double tvX = pid_tx.update(ROT.x - MPU_rpy.x, abs_time);
        double tvY = pid_ty.update(ROT.y - MPU_rpy.y, abs_time);
        double tvXY = pid_rot_z.update(ROT.z - MPU_rpy.z, abs_time);
        printf("tvX:%f - tvY:%f - tvXY:%f \n", tvX, tvY, tvXY);
        printf("%f \n", abs_time);

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

        pwm_set_duty(10, map(-ANGLE_GAIN * (tvX + ROT_CORRECTION * tvXY), 0, 180, 5, 10)); // RX
        pwm_set_duty(11, map(-ANGLE_GAIN * (tvX + ROT_CORRECTION * tvXY), 0, 180, 5, 10)); // RY
        pwm_set_duty(12, map(ANGLE_GAIN * (tvX + ROT_CORRECTION * tvXY), 0, 180, 5, 10));  // FY
        pwm_set_duty(13, map(ANGLE_GAIN * (tvX + ROT_CORRECTION * tvXY), 0, 180, 5, 10));  // FX
        sleep_ms(10);
        timer = get_absolute_time();
    }
    return true;
}
