#define PWM_DEFAULT_TIMER	PWM_TIMER1
#define PWM_DEFAULT_CHANNEL	PWM_CHANNEL0
#define PWM_DEFAULT_ALT_CHANNEL	PWM_CHANNEL3

long pwm_ll_setup(struct pwm_config_values_s *cfg);
int pwm_ll_get(struct pwm_config_values_s *cfg, unsigned channel, unsigned *pulse);
int pwm_ll_set(struct pwm_config_values_s *cfg, unsigned channel, unsigned pulse);
