/*
 * main.c
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>
#include <BTN.h>
#include <LED.h>

#define SLEEP_TIME_MS 1000

#define SW0_NODE DT_ALIAS(sw0)
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(SW0_NODE, gpios);

static struct gpio_callback button_isr_data;

int main(void) {
  if (0 > BTN_init()) {
    return 0;
  }
  if (0 > LED_init()) {
    return 0;
  }
  uint8_t led_mask = 0;
  while(1) {
    if((led_mask & 0b0001) > 0){
      LED_set(LED0, LED_ON);}
    else if((led_mask & 0b0001) == 0)
      LED_set(LED0, LED_OFF);
      
    if((led_mask & 0b0010) > 0){
      LED_set(LED1, LED_ON);}
    else if((led_mask & 0b0010) == 0)
      LED_set(LED1, LED_OFF);

    if((led_mask & 0b0100) > 0){
      LED_set(LED2, LED_ON);}
    else if((led_mask & 0b0100) == 0)
      LED_set(LED2, LED_OFF);

    if((led_mask & 0b1000) > 0){
      LED_set(LED3, LED_ON);}
    else if((led_mask & 0b1000) == 0)
      LED_set(LED3, LED_OFF);

    if(BTN_check_clear_pressed(BTN0)){
      led_mask++;
      printk("\n%d\n", led_mask);
    }
      

    if(led_mask == 16)
      led_mask = 0;


  }
	return 0;
}
