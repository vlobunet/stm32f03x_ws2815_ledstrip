#include "PCB705.h"

uint32_t getTrueRandomNumber(void) {
  uint8_t i;
  ADC_InitTypeDef ADC_InitStructure;
  //enable ADC1 clock
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  // Initialize ADC 14MHz RC
  RCC_ADCCLKConfig(RCC_ADCCLK_HSI14);
  RCC_HSI14Cmd(ENABLE);
  while (!RCC_GetFlagStatus(RCC_FLAG_HSI14RDY))
    ;
  ADC_DeInit(ADC1);
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Backward;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_TRGO; //default
  ADC_Init(ADC1, &ADC_InitStructure);
  //enable internal channel
  ADC_TempSensorCmd(ENABLE);
  // Enable ADCperipheral
  ADC_Cmd(ADC1, ENABLE);
  while (ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN) == RESET)
    ;
  ADC1->CHSELR = 0; //no channel selected
  //Convert the ADC1 temperature sensor, user shortest sample time to generate most noise
  ADC_ChannelConfig(ADC1, ADC_Channel_TempSensor, ADC_SampleTime_1_5Cycles);
  // Enable CRC clock 
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
  
  for (i = 0; i < 8; i++) {
    //Start ADC1 Software Conversion
    ADC_StartOfConversion(ADC1);
    //wait for conversion complete
    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)) {
    }
    CRC_CalcCRC(ADC_GetConversionValue(ADC1));
    //clear EOC flag
    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
  }
  //disable ADC1 to save power
  ADC_Cmd(ADC1, DISABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, DISABLE);
  return CRC_CalcCRC(0xBADA55E5);
}

int random(int i) {
	//return((rand() + TIM16->SR) % i);
    return(getTrueRandomNumber() % i);
}

void delay_us(uint32_t delay_us) {
	volatile unsigned int num;
	volatile unsigned int t;

	for (num = 0; num < delay_us; num++) {
		t = 11;
		while (t != 0) t--;
	}
}

void delay_ms(uint16_t delay_ms) {
	volatile unsigned int num;

	for (num = 0; num < delay_ms; num++) {
		delay_us(1000);
	}
}

uint32_t RGB_to_BRG(uint32_t color) {
    return((color >> 8) | (color & 0xff) << 16);
}

void showStrip(void) {
	Fill_In_The_DMA_Bufer( USART1_TX_Bufer, SendBuffer, SIZE_STRIP );	
	Fill_In_The_DMA_Bufer( USART2_TX_Bufer, SendBuffer, SIZE_STRIP );

	DMA1_Channel2->CNDTR = SIZE_STRIP * 12;
	DMA_Cmd(DMA1_Channel2, ENABLE);						

	DMA1_Channel4->CNDTR = SIZE_STRIP * 12;
	DMA_Cmd(DMA1_Channel4, ENABLE);
}

void setPixel(int Pixel, uint8_t red, uint8_t green, uint8_t blue) {
	if (Pixel > SIZE_STRIP || Pixel < 0) return;
	
    SendBuffer[Pixel] = RGB_to_BRG(red << 16 | green << 8 | blue);
}

void setAll(uint8_t red, uint8_t green, uint8_t blue) {
	int i;
  for(i = 0; i < SIZE_STRIP; i++) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}

t_color RGBW_gradient(float progress, t_color start, t_color end) {
    t_color result;

    if (progress == 0.0f) return start;
    if (progress == 1.0f) return end;

    result.red = start.red + ((end.red - start.red) * progress);
    result.green = start.green + ((end.green - start.green) * progress);
    result.blue = start.blue + ((end.blue - start.blue) * progress);
    result.brightness = start.brightness + ((end.brightness - start.brightness) * progress);

    return result;
}

t_color RGB_gradient(float progress, t_color start, t_color end) {
    t_color result;

    if (progress == 0.0f) return start;
    if (progress == 1.0f) return end;

    result.red = start.red + ((end.red - start.red) * progress);
    result.green = start.green + ((end.green - start.green) * progress);
    result.blue = start.blue + ((end.blue - start.blue) * progress);
    result.brightness = start.brightness + ((end.brightness - start.brightness) * progress);

    return result;
}
