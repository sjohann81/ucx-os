#define PWM_DEFAULT_TIMER	PWM_TIMER4
#define PWM_DEFAULT_CHANNEL	PWM_CHANNEL1
#define PWM_DEFAULT_ALT_CHANNEL	PWM_CHANNEL4

long pwm_ll_setup(struct pwm_config_values_s *cfg);
int pwm_ll_get(struct pwm_config_values_s *cfg, unsigned channel, unsigned *pulse);
int pwm_ll_set(struct pwm_config_values_s *cfg, unsigned channel, unsigned pulse);
