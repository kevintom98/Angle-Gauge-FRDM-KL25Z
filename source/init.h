/*
 * init.h
 *
 *  Created on: 05-Oct-2021
 *      Author: Kevin Tom
 *
 *
 *  Header file for init file
 */



#ifndef INIT_H_
#define INIT_H_


#define RED_LED_PIN   (18)
#define GREEN_LED_PIN (19)
#define BLUE_LED_PIN  (1)

#define ALTERNATE_FUNCTION_3 (3)
#define ALTERNATE_FUNCTION_4 (4)

#define TPM0_CHANNEL_1 (1)
#define TPM2_CHANNEL_0 (0)
#define TPM2_CHANNEL_1 (1)



// SWITCH is connected to PORT D, pin 3
#define SWITCH_GPIO_PORT GPIOD
#define SWITCH_PIN 3
#define SWITCH_PIN_CTRL_REG PORTD->PCR[SWITCH_PIN]
#define SWITCH_SCGC5_MASK SIM_SCGC5_PORTD_MASK


#define INTERRUPT_WHEN_ZERO 8


void led_init();
void touch_init();
void tpm_config(uint16_t);
void init_switch();
void init_switch_interrupt();


#endif /* INIT_H_ */
