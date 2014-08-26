#include "stm32f4xx.h"
void Delay(__IO uint32_t nCount)
{
    while(nCount--)
    {
    }
}
int main(void)
{
    /*Structures used in the configuration*/
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable TIM4 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    /* Enable GPIOG Pins that are used for on board LED's */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

    //Enabled GPIOB we are going to use PB6 which is linked to TIM4_CH1 according to the
    //documentation
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    /* Initialise  pins 13, 14 and 15 D - relating to on board LED's*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    //initalise pin 6 B - relating to timer 4 channel 1
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource13, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource15, GPIO_AF_TIM4);

    /* Setup PWM */
    uint16_t PrescalerValue = 0;
    PrescalerValue = (uint16_t) 1;

    /* Setup timer defaults */
    TIM_TimeBaseStructure.TIM_Period = 665;
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    /* Configure timer for PWM - channel 1*/
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 500;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    //notice the number 1 in TIM_OC1Init
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

    /* Configure timer for PWM - channel 2 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 500;
    TIM_OC2Init(TIM4, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

    /* Configure timer for PWM - channel 3*/
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 500;
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

    /* Configure timer for PWM - channel 4 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 500;
    TIM_OC4Init(TIM4, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM4, ENABLE);

    /* Start timer*/
    TIM_Cmd(TIM4, ENABLE);

    //for this timer configuration a CCR (power) value of 700 yields approximately 3V on the pin (and is the max)
    TIM4->CCR1 = 500; //CCR1 controls channel 1
    TIM4->CCR2 = 500;//CCR2 controls channel 1
    TIM4->CCR3 = 500;//CCR3 controls channel 1
    TIM4->CCR4 = 500;//CCR4 controls channel 1
    while(1)  //Loop forever
    {
        //The power calculations here are only for demonstration and are very rough!!!
        //I've devided teh power of the on board LED so you can see the differnt brightnesses

    }
}

//void init_GPIO(void)
//{
//
//    GPIO_InitTypeDef GPIO_InitStruct;
//
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
//
//    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_13; // we want to configure all LED GPIO pins
//    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;      // we want the pins to be an output
//    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;  // this sets the GPIO modules clock speed
//    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;     // this sets the pin type to push / pull (as opposed to open drain)
//    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;   // this sets the pullup / pulldown resistors to be inactive
//    GPIO_Init(GPIOG, &GPIO_InitStruct);             // this finally passes all the values to the GPIO_Init function which takes care of setting the corresponding bits.
//
//}
//
//void INTTIM_Config(void)
//{
//    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//    NVIC_InitTypeDef NVIC_InitStructure;
//    /* Enable the TIM2 gloabal Interrupt */
//    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
//
//    /* TIM2 clock enable */
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
//    /* Time base configuration */
//    TIM_TimeBaseStructure.TIM_Period = 2400 - 1; // 1 MHz down to 1 KHz (1 ms)
//    TIM_TimeBaseStructure.TIM_Prescaler = 1000 - 1; // 24 MHz Clock down to 1 MHz (adjust per your clock)
//    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
//    /* TIM IT enable */
//    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
//    /* TIM2 enable counter */
//    TIM_Cmd(TIM2, ENABLE);
//}
//
//void TIM2_IRQHandler(void)
//{
//    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
//    {
//        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//        GPIO_ToggleBits(GPIOG, GPIO_Pin_13);
//        GPIO_ToggleBits(GPIOG, GPIO_Pin_14);
//    }
//}
//
//int main()
//{
//    init_GPIO();
//    GPIO_ToggleBits(GPIOG, GPIO_Pin_13);
//    INTTIM_Config();
//    while(1){}
//    return 0;
//}
