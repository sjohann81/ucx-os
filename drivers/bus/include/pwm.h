/* PWM device API */
long pwm_setup(const struct device_s *dev);
void pwm_get(const struct device_s *dev, unsigned channel, unsigned *pulse);
void pwm_set(const struct device_s *dev, unsigned channel, unsigned pulse);

/* PWM configuration definitions */
enum pwm_direction {PWM_INPUT, PWM_OUTPUT};
enum pwm_mode {PWM_FAST, PWM_EDGE, PWM_PHASE_CORRECT, PWM_CENTER, PWM_ASYMMETRIC, PWM_PHASE_SHIFTED};

struct pwm_api_s {
	long (*pwm_setup)(const struct device_s *dev);
	void (*pwm_get)(const struct device_s *dev, unsigned channel, unsigned *pulse);
	void (*pwm_set)(const struct device_s *dev, unsigned channel, unsigned pulse);
};

enum pwm_timer_names {
	PWM_TIMER0, PWM_TIMER1, PWM_TIMER2, PWM_TIMER3,
	PWM_TIMER4, PWM_TIMER5, PWM_TIMER6, PWM_TIMER7,
	PWM_TIMER8, PWM_TIMER9, PWM_TIMER10, PWM_TIMER11,
	PWM_TIMER12, PWM_TIMER13, PWM_TIMER14, PWM_TIMER15
};

enum pwm_channel_names {
	PWM_CHANNEL0 = (1 << 0), PWM_CHANNEL1 = (1 << 1),
	PWM_CHANNEL2 = (1 << 2), PWM_CHANNEL3 = (1 << 3),
	PWM_CHANNEL4 = (1 << 4), PWM_CHANNEL5 = (1 << 5),
	PWM_CHANNEL6 = (1 << 6), PWM_CHANNEL7 = (1 << 7),
	PWM_CHANNEL8 = (1 << 8), PWM_CHANNEL9 = (1 << 9),
	PWM_CHANNEL10 = (1 << 10), PWM_CHANNEL11 = (1 << 11),
	PWM_CHANNEL12 = (1 << 12), PWM_CHANNEL13 = (1 << 13),
	PWM_CHANNEL14 = (1 << 14), PWM_CHANNEL15 = (1 << 15)
};

struct pwm_config_values_s {
	char timer;
	char direction;
	char mode;
	unsigned channels;
	unsigned inv_polarity;
	unsigned long frequency;
};

struct pwm_config_s {
	struct pwm_config_values_s config_values;
};

extern struct pwm_api_s pwm_api;
