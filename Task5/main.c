#include "ch32v00x.h"
#include <debug.h>

#define MOTION_SENSOR_PIN GPIO_Pin_1   // PD2
#define MOTION_SENSOR_PORT GPIOD

#define BUZZER_PIN GPIO_Pin_1          // PC1
#define BUZZER_PORT GPIOC

#define LED_PIN GPIO_Pin_0             // PC0
#define LED_PORT GPIOC

void GPIO_Config(void) {
    // Enable clocks for GPIOD and GPIOC
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOC, ENABLE);

    // Configure PD2 as input (Motion Sensor)
    GPIO_InitTypeDef GPIO_InitStructure;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);  // Enable clock for GPIO port

GPIO_InitStructure.GPIO_Pin = MOTION_SENSOR_PIN;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // Pull-up input (change to IN_FLOATING if push-pull)
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOD, &GPIO_InitStructure);


    // Configure PC0 and PC1 as output (LED and Buzzer)
    GPIO_InitStructure.GPIO_Pin = LED_PIN | BUZZER_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // Ensure LED and Buzzer are off initially
    GPIO_ResetBits(GPIOC, LED_PIN | BUZZER_PIN);
}

int main(void) {
    SystemInit();
    GPIO_Config();

    while (1) {
        if (GPIO_ReadInputDataBit(GPIOD, MOTION_SENSOR_PIN) == Bit_RESET) {
            // Motion detected: Turn on LED and Buzzer
            GPIO_SetBits(GPIOC, LED_PIN | BUZZER_PIN);
        } else {
            // No motion: Turn off LED and Buzzer
            GPIO_ResetBits(GPIOC, LED_PIN | BUZZER_PIN);
        }
    }
}