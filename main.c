#include "stm32f4xx.h"
void Delay(__IO uint32_t nCount);

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
    PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 18000000) - 1;

    /* Setup timer defaults */
    TIM_TimeBaseStructure.TIM_Period = 665;
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    /* Configure timer for PWM - channel 1*/
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    //notice the number 1 in TIM_OC1Init
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

    /* Configure timer for PWM - channel 2 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC2Init(TIM4, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

    /* Configure timer for PWM - channel 3*/
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

    /* Configure timer for PWM - channel 4 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC4Init(TIM4, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM4, ENABLE);

    /* Start timer*/
    TIM_Cmd(TIM4, ENABLE);

    int power = 0;
    //for this timer configuration a CCR (power) value of 700 yields approximately 3V on the pin (and is the max)
    while(1)  //Loop forever
    {
        //The power calculations here are only for demonstration and are very rough!!!
        //I've devided teh power of the on board LED so you can see the differnt brightnesses
        TIM4->CCR1 = power; //CCR1 controls channel 1
        TIM4->CCR2 = power /2;//CCR2 controls channel 1
        TIM4->CCR3 = power /8;//CCR3 controls channel 1
        TIM4->CCR4 = power /16;//CCR4 controls channel 1

        Delay(80000);
        power++;
        if(power > 699){
            power = 0;
            Delay(900000);
        }
    }
}

/**
 * @brief  Delay Function.
 * @param  nCount:specifies the Delay time length.
 * @retval None
 */
void Delay(__IO uint32_t nCount)
{
    while(nCount--)
    {
    }
}
//int main(void)
//{
//    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//    TIM_OCInitTypeDef  TIM_OCInitStructure;
//
//    /* TIM config */
//
//    GPIO_InitTypeDef GPIO_InitStructure;
//
//
//    /* TIM4 clock enable */
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
//
//
//    /* LEDs are on GPIOG */
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
//
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
//    GPIO_Init(GPIOG, &GPIO_InitStructure);
//
//
//    GPIO_PinAFConfig(GPIOG, GPIO_PinSource12, GPIO_AF_TIM4);
//    GPIO_PinAFConfig(GPIOG, GPIO_PinSource13, GPIO_AF_TIM4);
//    GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_TIM4);
//    GPIO_PinAFConfig(GPIOG, GPIO_PinSource15, GPIO_AF_TIM4);
//
//    /* pwm set up */
//
//
//    /* Compute the prescaler value */
//    int PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 21000000) - 1;
//
//
//    /* Time base configuration */
//    TIM_TimeBaseStructure.TIM_Period = 665;
//    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
//    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//
//
//    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
//
//
//    /* PWM1 Mode configuration: Channel1 */
//    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
//    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//    TIM_OCInitStructure.TIM_Pulse = 600;
//    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//
//
//    TIM_OC1Init(TIM4, &TIM_OCInitStructure);
//
//
//    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
//
//
//    /* PWM1 Mode configuration: Channel2 */
//    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//    TIM_OCInitStructure.TIM_Pulse = 600;
//
//
//    TIM_OC2Init(TIM4, &TIM_OCInitStructure);
//
//
//    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
//
//
//    /* PWM1 Mode configuration: Channel3 */
//    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//    TIM_OCInitStructure.TIM_Pulse = 600;
//
//
//    TIM_OC3Init(TIM4, &TIM_OCInitStructure);
//
//
//    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
//
//
//    /* PWM1 Mode configuration: Channel4 */
//    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//    TIM_OCInitStructure.TIM_Pulse = 600;
//
//
//    TIM_OC4Init(TIM4, &TIM_OCInitStructure);
//
//
//    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
//
//
//    TIM_ARRPreloadConfig(TIM4, ENABLE);
//
//
//    /* TIM4 enable counter */
//    TIM_Cmd(TIM4, ENABLE);
//
//    /* DEMO */
//
////    int brightness = 0;
////    int i;
//    while(1)  // Do not exit
//    {
////        brightness++;
////
////
////        TIM4->CCR3 = 333 - (brightness + 0) % 333; // set brightness
////        TIM4->CCR4 = 333 - (brightness + 166/2) % 333; // set brightness
////        TIM4->CCR1 = 333 - (brightness + 333/2) % 333; // set brightness
////        TIM4->CCR2 = 333 - (brightness + 499/2) % 333; // set brightness
////
////
////        for(i=0;i<10000;i++);  // delay
////        for(i=0;i<10000;i++);  // delay
////        for(i=0;i<10000;i++);  // delay
//    }
//}

//void init_GPIO(void)
//{
//    GPIO_InitTypeDef GPIO_InitStruct;
//
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
//
//    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_13; // we want to configure all LED GPIO pins
//    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;      // we want the pins to be an output
//    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;  // this sets the GPIO modules clock speed
//    //GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;     // this sets the pin type to push / pull (as opposed to open drain)
//    //GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;   // this sets the pullup / pulldown resistors to be inactive
//    //GPIO_Init(GPIOG, &GPIO_InitStruct);             // this finally passes all the values to the GPIO_Init function which takes care of setting the corresponding bits.
//
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
//
//    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;        // we want to configure PA0
//    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;     // we want it to be an input
//    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//this sets the GPIO modules clock speed
//    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;   // this sets the pin type to push / pull (as opposed to open drain)
//    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;   // this enables the pulldown resistor --> we want to detect a high level
//    GPIO_Init(GPIOA, &GPIO_InitStruct);           // this passes the configuration to the Init function which takes care of the low level stuff
//}
//
//void init_TIM(void)
//{
//    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//    TIM_OCInitTypeDef TIM_OCInitStructure = {0,};
//
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
//
//    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
//    TIM_TimeBaseStructure.TIM_Prescaler = 40000;
//    TIM_TimeBaseStructure.TIM_Period = 500;
//    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
//    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
//    TIM_Cmd(TIM4, ENABLE);
//
////    TIM_OCStructInit(&TIM_OCInitStructure);
//    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
//    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//    TIM_OCInitStructure.TIM_Pulse = 400;
//    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//    TIM_OC1Init(TIM4, &TIM_OCInitStructure);
//    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
//
//    GPIO_PinAFConfig(GPIOG, GPIO_PinSource13, GPIO_AF_TIM4);
//}   
//
//int main(void)
//{
//    init_GPIO();
//    init_TIM();
//    while(1){}
//}
//
void assert_failed(uint8_t* file, uint32_t line)
{
    //printf("Wrong parameters value: file");// %d on line %d\r\n", (int)file, (int)line);
    while(1)
    {}
}
