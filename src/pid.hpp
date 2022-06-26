#define INTEGRAL_THRESHOLD 10
#define EPSILON 0.001

class PID
{
private:
    double integral = 0;
    double prev_err = 0;

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
        else
        {
            return val;
        }
        return 0;
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
    double kp = 0, ki = 0, kd = 0, max = infinity(), min = (-infinity());
    PID(double kp, double ki, double kd)
    {
        this->kp = kp;
        this->ki = ki;
        this->kd = kd;
    }

    void set_clamp(double min, double max)
    {
        this->min = min;
        this->max = max;
    }

    double update(double err, double delta_t)
    {
        double prop = err * kp;
        double diff = kd * (err - prev_err) / delta_t;
        integral += ki * err * delta_t;
        if (this->abs(integral - INTEGRAL_THRESHOLD) > EPSILON)
        {
            reset_integral();
        }
        return clamp(prop + diff + integral, min, max);
    }
};