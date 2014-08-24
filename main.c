#include "stm32f4xx.h"

void init_GPIO(void){

    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_13; // we want to configure all LED GPIO pins
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;      // we want the pins to be an output
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;  // this sets the GPIO modules clock speed
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;     // this sets the pin type to push / pull (as opposed to open drain)
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;   // this sets the pullup / pulldown resistors to be inactive
    GPIO_Init(GPIOG, &GPIO_InitStruct);             // this finally passes all the values to the GPIO_Init function which takes care of setting the corresponding bits.

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;        // we want to configure PA0
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;     // we want it to be an input
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//this sets the GPIO modules clock speed
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;   // this sets the pin type to push / pull (as opposed to open drain)
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;   // this enables the pulldown resistor --> we want to detect a high level
    GPIO_Init(GPIOA, &GPIO_InitStruct);           // this passes the configuration to the Init function which takes care of the low level stuff
}
//Quick hack, approximately 1ms delay
void ms_delay(int ms)
{
    while (ms-- > 0) {
        volatile int x=91;
        while (x-- > 0)
            __asm("nop");
    }
}

int i = 0;
int j = 0;
int rising = 1;

//Flash orange LED at about 1hz
int main(void)
{
    int butPressed;
    init_GPIO();

    while(1)
    {

        for (j = 0; j < 60; j++)
        { 
            butPressed = GPIOA->IDR & 0x0001;
            ms_delay(40-i);
            if (butPressed)
            {
                GPIOG->BSRRL = 0x4000;           // Toggle the pin 
                GPIOG->BSRRH = 0x2000;           // Toggle the pin 
                ms_delay(i);
                GPIOG->BSRRH = 0x4000;           // Toggle the pin 
                GPIOG->BSRRL = 0x2000;           // Toggle the pin 
            }
            else
            {
                GPIOG->BSRRH = 0x6000;
                ms_delay(i);
            }
        }
        if (i == 40)
        {
            rising = 0;
        }
        else if (i == 0)
        {
            rising = 1;
        }
        if (rising)
        {
            i++;
        }
        else
        {
            i--;
        }

    }
}

void assert_failed(uint8_t* file, uint32_t line)
{
    //printf("Wrong parameters value: file");// %d on line %d\r\n", (int)file, (int)line);
    while(1)
    {}
}
