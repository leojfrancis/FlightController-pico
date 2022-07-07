/* Integrator limits */
#define INTEGRAL_MIN -90
#define INTEGRAL_MAX 90

class PID
{
private:
    /* Controller "memory" */
    double integral = 0;
    double prev_err = 0;
    double diffrential = 0;
    double prev_measurement = 0;

    void reset_integral()
    {
        this->integral = 0;
    }

    double clamp(double val, double min, double max)
    {
        if (val < min)
        {
            return min;
        }
        else if (val > max)
        {
            return max;
        }
        return val;
    }

    double abs(double value)
    {
        if (value < 0)
        {
            return -1 * value;
        }
        else
        {
            return value;
        }
    }

public:
    double kp = 0, ki = 0, kd = 0, tau = 0, max = infinity(), min = (-infinity()), out = 0;
    PID(double kp, double kd, double ki, double tau)
    {
        this->kp = kp;
        this->ki = ki;
        this->kd = kd;
        this->tau = tau;
    }

    void set_clamp(double min, double max)
    {
        this->min = min;
        this->max = max;
    }

    double update(double setpoint, double measurement, double delta_time)
    {
        double err = setpoint - measurement;                                    // Error functions
        double prop = err * kp;                                                 // Proportional set
        double integr = integral + (0.5f * ki * delta_time * (err + prev_err)); // Integral Set
        integral = clamp(integr, INTEGRAL_MIN, INTEGRAL_MAX);                 // Anti wind up with clamping
        diffrential = -(2.0f * kd * (measurement - prev_measurement)            /* Note: derivative on measurement, therefore minus sign in front of equation! */
                        + (2.0f * tau - delta_time) * diffrential) /
                      (2.0f * tau + delta_time);

        /* Store error and measurement */
        prev_err = err;
        prev_measurement = measurement;
        out = clamp(prop + diffrential + integral, min, max);
        // printf("prop:%.2f , diffrential:%.2f , integral:%.2f, out:%.2f\n", prop, diffrential, integr, out);
        return out;
    }
};