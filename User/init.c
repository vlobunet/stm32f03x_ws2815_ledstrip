#include "PCB705.h"

static void LED_GIPIO_Init(void) {
	GPIO_InitTypeDef GPIO_Init_LED;
  GPIO_Init_LED.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_15 | GPIO_Pin_1 | GPIO_Pin_12;
	GPIO_Init_LED.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init_LED.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init_LED.GPIO_OType = GPIO_OType_PP;
	GPIO_Init_LED.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOA, &GPIO_Init_LED);	
    
  GPIO_Init_LED.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_Init_LED.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init_LED.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init_LED.GPIO_OType = GPIO_OType_PP;
	GPIO_Init_LED.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOB, &GPIO_Init_LED);	
}



static void Button_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_Init_Button;  
	
	GPIO_Init_Button.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_Init_Button.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init_Button.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init_Button.GPIO_OType = GPIO_OType_PP;
	GPIO_Init_Button.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOB, &GPIO_Init_Button);	

	GPIO_Init_Button.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init_Button.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init_Button.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init_Button.GPIO_OType = GPIO_OType_PP;
	GPIO_Init_Button.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(GPIOB, &GPIO_Init_Button);

	GPIO_Init_Button.GPIO_Pin = GPIO_Pin_8 ;
	GPIO_Init_Button.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init_Button.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init_Button.GPIO_OType = GPIO_OType_PP;
	GPIO_Init_Button.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_Init(GPIOA, &GPIO_Init_Button);
}

static void USART_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_Init_USART;
	GPIO_Init_USART.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_9|GPIO_Pin_10;
	GPIO_Init_USART.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init_USART.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init_USART.GPIO_OType = GPIO_OType_PP;
	GPIO_Init_USART.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(GPIOA, &GPIO_Init_USART);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);
}

static void USART_Init_User(void) {
	USART_InitTypeDef USART_InitUser;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	USART_InitUser.USART_BaudRate = 2700000;
	USART_InitUser.USART_WordLength = USART_WordLength_9b;
	USART_InitUser.USART_StopBits = USART_StopBits_2;
	USART_InitUser.USART_Parity = USART_Parity_No;
	USART_InitUser.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
	USART_InitUser.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

	USART_Init(USART2, &USART_InitUser);
	USART_Init(USART1, &USART_InitUser);
	
	USART_OverSampling8Cmd(USART1,ENABLE);
	USART_OverSampling8Cmd(USART2,ENABLE);

	USART_Cmd(USART2, ENABLE);
	USART_Cmd(USART1, ENABLE);
}

static void DMA_Init_User(void) {
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->TDR);
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)&USART1_TX_Bufer;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStruct.DMA_BufferSize = sizeof(USART1_TX_Bufer);
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStruct.DMA_Priority = DMA_Priority_Low;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(DMA1_Channel2, &DMA_InitStruct);

	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);					
	NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);

	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->TDR);
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)&USART2_TX_Bufer;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStruct.DMA_BufferSize = sizeof(USART2_TX_Bufer);
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStruct.DMA_Priority = DMA_Priority_Low;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(DMA1_Channel4, &DMA_InitStruct);

	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);					
	NVIC_EnableIRQ(DMA1_Channel4_5_IRQn);
}

static void TIMER_Init(void) {
	uint32_t Counter = 0;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);
	
	TIMER_Init_USER.TIM_Prescaler = TIM_PRESCALER;
	TIMER_Init_USER.TIM_CounterMode = TIM_CounterMode_Up;							
	TIMER_Init_USER.TIM_Period = TIM_PERIOD;															
	TIMER_Init_USER.TIM_ClockDivision = 0;
	TIM_TimeBaseInit(TIM16, &TIMER_Init_USER);
	for (Counter = 0; Counter < 200000; Counter++) { }
	NVIC_EnableIRQ(TIM16_IRQn);			
	TIM_Cmd(TIM16, ENABLE);								
	TIM_ITConfig(TIM16, TIM_IT_Update, ENABLE);
}

void F4030C8_Setup(void) {
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	LED_GIPIO_Init();
	Button_GPIO_Init();
	USART_GPIO_Init();
	USART_Init_User();
	
	SysTick_Config(SystemCoreClock / 20000);
	GPIO_SetBits(GPIOA, GPIO_Pin_1 );		
	GPIO_SetBits(GPIOA, GPIO_Pin_12);

	DMA_Init_User();
	TIMER_Init();
}
