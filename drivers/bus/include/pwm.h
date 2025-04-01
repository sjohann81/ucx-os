/* PWM device API */
int pwm_setup(const struct device_s *dev, int timer, int direction, int mode);
void pwm_get(const struct device_s *dev, int channel, unsigned long *period, unsigned long *pulse);
void pwm_set(const struct device_s *dev, int channel, unsigned long period, unsigned long pulse);

/* PWM configuration definitions */
enum pwm_direction {PWM_INPUT, PWM_OUTPUT};
enum pwm_mode {PWM_FAST, PWM_EDGE, PWM_PHASE_CORRECT, PWM_CENTER, PWM_ASYMMETRIC, PWM_PHASE_SHIFTED};
enum pwm_polarity {PWM_NORMAL, PWM_INVERTED};

struct pwm_api_s {
	int (*pwm_setup)(const struct device_s *dev, int timer, int direction, int mode);
	void (*pwm_get)(const struct device_s *dev, int channel, unsigned long *period, unsigned long *pulse);
	void (*pwm_set)(const struct device_s *dev, int channel, unsigned long period, unsigned long pulse);
};

enum pwm_timer_names {
	PWM_TIMER0, PWM_TIMER1, PWM_TIMER2, PWM_TIMER3,
	PWM_TIMER4, PWM_TIMER5, PWM_TIMER6, PWM_TIMER7,
	PWM_TIMER8, PWM_TIMER9, PWM_TIMER10, PWM_TIMER11,
	PWM_TIMER12, PWM_TIMER13, PWM_TIMER14, PWM_TIMER15
};

enum pwm_channel_names {
	PWM_CHANNEL0, PWM_CHANNEL1, PWM_CHANNEL2, PWM_CHANNEL3,
	PWM_CHANNEL4, PWM_CHANNEL5, PWM_CHANNEL6, PWM_CHANNEL7,
	PWM_CHANNEL8, PWM_CHANNEL9, PWM_CHANNEL10, PWM_CHANNEL11,
	PWM_CHANNEL12, PWM_CHANNEL13, PWM_CHANNEL14, PWM_CHANNEL15
};

struct pwm_config_values_s {
	int timer;
	int direction;
	int mode;
};

struct pwm_config_s {
	struct pwm_config_values_s config_values;
};

extern struct pwm_api_s pwm_api;
