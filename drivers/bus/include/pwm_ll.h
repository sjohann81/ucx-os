int pwm_ll_setup(struct pwm_config_values_s *cfg);
int pwm_ll_get(struct pwm_config_values_s *cfg, int channel, unsigned long *period, unsigned long *pulse);
int pwm_ll_set(struct pwm_config_values_s *cfg, int channel, unsigned long period, unsigned long pulse);
