#include <ch32v00x.h>
#include <debug.h>

#define BLINKY_GPIO_PORT GPIOD
#define BLINKY_GPIO_PIN GPIO_Pin_6
#define PIR_GPIO_PIN GPIO_Pin_2 // PIR sensor output connected to GPIO Pin 2
#define BLINKY_CLOCK_ENABLE RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE)

void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void Delay_Init(void);
void Delay_Ms(uint32_t n);

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  //
    SystemCoreClockUpdate();
    Delay_Init();

    GPIO_InitTypeDef GPIO_InitStructure = {0};

    BLINKY_CLOCK_ENABLE;
    GPIO_InitStructure.GPIO_Pin = BLINKY_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BLINKY_GPIO_PORT, &GPIO_InitStructure);

    // Configure PIR sensor input pin
    GPIO_InitStructure.GPIO_Pin = PIR_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // Input mode with pull-up
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    while (1)
    {
        // Read PIR sensor status
        uint8_t pirStatus = GPIO_ReadInputDataBit(GPIOD, PIR_GPIO_PIN);

        // Control the LED based on PIR sensor output
        if (pirStatus == 1) // PIR sensor detected motion
        {
            GPIO_WriteBit(BLINKY_GPIO_PORT, BLINKY_GPIO_PIN, SET); // Turn on LED
            Delay_Ms(100);
        }
        else
        {
            GPIO_WriteBit(BLINKY_GPIO_PORT, BLINKY_GPIO_PIN, RESET); // Turn off LED
        }
        Delay_Ms(100);

    }
}

void NMI_Handler(void) {}
void HardFault_Handler(void)
{
    while (1)
    {
    }
}