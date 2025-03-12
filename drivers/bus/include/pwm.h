/* PWM device API */
int pwm_init(const struct device_s *dev, uint8_t timer, uint8_t mode);
int pwm_deinit(const struct device_s *dev, uint8_t timer);
int pwm_set(const struct device_s *dev, uint8_t channel, uint32_t period, uint32_t pulse);
int pwm_get(const struct device_s *dev, uint8_t channel, uint32_t *period, uint32_t *pulse);

extern struct pwm_api_s pwm_api;

enum {PWM_INPUT, PWM_OUTPUT};

