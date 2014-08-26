#include "stm32f4xx.h"

void init_GPIO(void)
{

    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_13; // we want to configure all LED GPIO pins
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;      // we want the pins to be an output
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;  // this sets the GPIO modules clock speed
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;     // this sets the pin type to push / pull (as opposed to open drain)
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;   // this sets the pullup / pulldown resistors to be inactive
    GPIO_Init(GPIOG, &GPIO_InitStruct);             // this finally passes all the values to the GPIO_Init function which takes care of setting the corresponding bits.

}

void INTTIM_Config(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Enable the TIM2 gloabal Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* TIM2 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 24000 - 1; // 1 MHz down to 1 KHz (1 ms)
    TIM_TimeBaseStructure.TIM_Prescaler = 1000 - 1; // 24 MHz Clock down to 1 MHz (adjust per your clock)
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    /* TIM IT enable */
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    /* TIM2 enable counter */
    TIM_Cmd(TIM2, ENABLE);
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        GPIO_ToggleBits(GPIOG, GPIO_Pin_13);
        GPIO_ToggleBits(GPIOG, GPIO_Pin_14);
    }
}

int main()
{
    init_GPIO();
    INTTIM_Config();
    while(1){}
    return 0;
}
