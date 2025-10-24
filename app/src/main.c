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
  uint32_t password_mask = 0;
  uint32_t password_key = 0x00421421;
  uint8_t  num_in = 0;
  uint8_t  intialize = 0;
  uint8_t  timeout = 0;
  uint32_t mode_count = 0;
  LED_set(LED0, LED_ON);
  LED_set(LED1, LED_ON);
  while(1) {
    if(intialize == 0)
    {
      if(BTN_check_clear_pressed(BTN3))
      {
        LED_set(LED1, LED_OFF);
        LED_set(LED2, LED_ON);
        intialize = 2;
        password_key = 0;
        printk("PASSWORD CHANGE!\n");
      }

      if(timeout)
      {
        LED_set(LED1, LED_OFF);
        LED_set(LED2, LED_OFF);
        intialize = 1;
        timeout = 0;
        printk("LOCKED, ENTER PASSWORD!\n");
      }

      if(mode_count > 3000)
      {
        timeout = 1;
        mode_count = 0;
      }

      mode_count++;
      k_msleep(1);
    }


    if(intialize == 1){
      if(BTN_check_clear_pressed(BTN0))
      {
        password_mask = password_mask | (0x1 << 4*num_in);
        num_in++;
      }
      if(BTN_check_clear_pressed(BTN1))
      {
        password_mask = password_mask | (0x2 << 4*num_in);
        num_in++;
      }
      if(BTN_check_clear_pressed(BTN2))
      {
        password_mask = password_mask | (0x4 << 4*num_in);
        num_in++;
      }
      if(num_in > 8){
        printk("Incorrect!\n");
        num_in = 0;
        password_mask = 0;
      }

      if(BTN_check_clear_pressed(BTN3))
      {
        if(password_mask == password_key){
          printk("Correct!\n");
          LED_set(LED0, LED_OFF);
          while(0 < BTN_check_clear_pressed(BTN0) ||
                      BTN_check_clear_pressed(BTN1) ||
                      BTN_check_clear_pressed(BTN2) ||
                      BTN_check_clear_pressed(BTN3)){}
          LED_set(LED0, LED_ON);
          printk("WAIT TO LOCK, OR BUTTON3 FOR PASSCHANGE!\n");
          intialize = 0;
        }
        else{
          printk("Incorrect!\n");
        }
        num_in = 0;
        password_mask = 0;
      }
    }

    if(intialize == 2){
      
      if(BTN_check_clear_pressed(BTN0))
      {
        password_key = password_key | (0x1 << 4*num_in);
        num_in++;
      }
      if(BTN_check_clear_pressed(BTN1))
      {
        password_key = password_key | (0x2 << 4*num_in);
        num_in++;
      }
      if(BTN_check_clear_pressed(BTN2))
      {
        password_key = password_key | (0x4 << 4*num_in);
        num_in++;
      }
      if(num_in > 8){
        printk("Max Password Size!\n");
        while(!BTN_check_pressed(BTN3)){}
      }

      if(BTN_check_clear_pressed(BTN3))
      {
        num_in = 0;
        intialize = 1;
        printk("NEW PASSWORD SAVED, LOCKED!\n");
      }
    }
      


  }
	return 0;
}
