/*
 * main.c
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>
#include <BTN.h>

#define SLEEP_TIME_MS 1000

#define SW0_NODE DT_ALIAS(sw0)
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(SW0_NODE, gpios);

static struct gpio_callback button_isr_data;

void button_isr(const struct device *dev, struct gpio_callback *cb, uint32_t pins){
  printk("Button 0 pressed!\n");
}

int main(void) {
  int ret;

  if(!gpio_is_ready_dt(&button)) {
    return 0;
  }
  if (0 > gpio_pin_configure_dt(&button, GPIO_INPUT)) {
    return 0;
  }
  if (0 > gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE)) {
    return 0;
  }
  if (0 > BTN_init()) {
    return 0;
  }
  if (0 > LED_init()) {
    return 0;
  }


  gpio_init_callback(&button_isr_data, button_isr, BIT(button.pin));
  gpio_add_callback(button.port, &button_isr_data);

  while(1) {
  }
	return 0;
}
