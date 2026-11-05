#include <stdlib.h>
#include <zephyr/syscalls/time_units.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(blink, LOG_LEVEL_INF);

K_THREAD_STACK_DEFINE(b_thread_stack, 4096);
static struct k_thread b_thread;
static k_tid_t b_thread_id = NULL;

#define LED_DELAY_MS (1000)

static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios);

static void blink_thread_entry(void *p1, void *p2, void *p3)
{
    uint8_t index = 0;

    ARG_UNUSED(p1);
    ARG_UNUSED(p2);
    ARG_UNUSED(p3);

    while(1)
	{
        switch(index)
        {
            case 0:
                gpio_pin_set_dt(&led0, 0);
                gpio_pin_set_dt(&led1, 1);
                gpio_pin_set_dt(&led2, 0);
                break;
            case 1:
                gpio_pin_set_dt(&led0, 1);
                gpio_pin_set_dt(&led1, 0);
                gpio_pin_set_dt(&led2, 0);
                break;
            case 2:
                gpio_pin_set_dt(&led0, 0);
                gpio_pin_set_dt(&led1, 0);
                gpio_pin_set_dt(&led2, 1);
                break;
            default:
                index = 0;
                break;
        }

        index++;
        if(index > 2)
        {
            index = 0;
        }

		k_sleep(K_MSEC(LED_DELAY_MS));
	}
}

int blink_init(void)
{
    gpio_pin_configure_dt(&led0, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&led1, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&led2, GPIO_OUTPUT_INACTIVE);

	if(b_thread_id != NULL)
	{
		LOG_WRN("thread has been created.");
		return 0;
	}
    b_thread_id = k_thread_create(&b_thread,
										b_thread_stack,
                     					4096,
                     					(k_thread_entry_t)blink_thread_entry,
                     					NULL, NULL, NULL,
                     					K_PRIO_PREEMPT(9), 0, K_NO_WAIT);

	if(b_thread_id != NULL)
  	{
    	k_thread_name_set(b_thread_id, "blink");
  	}
  	else
  	{
		LOG_ERR("blink thread create failed.");
  	}

	return 0;
}

