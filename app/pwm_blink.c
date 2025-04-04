#include <ucx.h>
#include <device.h>
#include <pwm.h>
#include <pwm_ll.h>

/* PWM configuration */
const struct pwm_config_s pwm_config = {
	.config_values.timer = PWM_DEFAULT_TIMER,
	.config_values.direction = PWM_OUTPUT,
	.config_values.mode = PWM_FAST,
	.config_values.channels = PWM_DEFAULT_CHANNEL | PWM_DEFAULT_ALT_CHANNEL,
	.config_values.frequency = 1000
};

const struct device_s pwm_device1 = {
	.name = "pwmdevice1",
	.config = &pwm_config,
	.custom_api = &pwm_api
};

const struct device_s *pwm1 = &pwm_device1;


/* pwm_setup() returns the PWM number of steps, so to configure
 * the duty cycle in pwm_set(), consider the following formula:
 * 
 * pulse = (val * 100) / duty
 * 
 * Where pulse is the PWM value to be passed to pwm_set(), val
 * is the number of steps returned by pwm_setup() and duty is
 * the duty cycle (0% to 100%) in 1% steps.
 */

void task0(void)
{
	long val, delay;
	 
	val = pwm_setup(pwm1);
	
	printf("value: %d\n", val);
	delay = (65536 / val) * 10;
	
	while (1) {
		printf("PWM: increasing duty cycle\n");
		for (int i = 0; i < val; i++) {
			pwm_set(pwm1, PWM_DEFAULT_CHANNEL, i);
			_delay_us(delay);
		}
		printf("PWM: decreasing duty cycle\n");
		for (int i = val - 1; i >= 0; i--) {
			pwm_set(pwm1, PWM_DEFAULT_CHANNEL, i);
			_delay_us(delay);
		}
	}
}

int32_t app_main(void)
{
	ucx_task_spawn(task0, DEFAULT_STACK_SIZE);
	
	// start UCX/OS, preemptive mode
	return 1;
}
