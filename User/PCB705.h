#ifndef __PCB705_H_
	#define __PCB705_H_

	#include "stm32f0xx.h"
	#include "stm32f0xx_gpio.h"
	#include "stm32f0xx_rcc.h"
	#include "stm32f0xx_usart.h"
    #include "stm32f0xx_adc.h"
    #include "stm32f0xx_crc.h"
	
	#include <stdlib.h>
	#include <math.h>

	/******************************************************************************/
	#define SIZE_HEAD 3
	#define SIZE_TAIL (4 + SIZE_HEAD)
	#define SIZEE_STEP 40
	#define MAX_LIGHT 150
	#define MIN_LIGHT 0
	#define SIZE_STRIP 37
    
    #define SET_GRN_COLOR(x) (uint32_t)(0x00 | ( x << 8 ) | ( 0x00 << 16 ))
	#define SET_RED_COLOR(x) (uint32_t)(x | ( 0x00 << 8 ) | ( 0x00 << 16 ))
	#define SET_BLUE_COLOR(x) (uint32_t)(0x00 | ( 0x00 << 8 ) | ( x << 16 ))
    
	/******************************************************************************/
	#define OUT1_ON() GPIO_SetBits(GPIOA, GPIO_Pin_0)
	#define OUT1_OFF() GPIO_ResetBits(GPIOA, GPIO_Pin_0)

	#define OUT2_ON() GPIO_SetBits(GPIOB, GPIO_Pin_9)
	#define OUT2_OFF() GPIO_ResetBits(GPIOB, GPIO_Pin_9)

	#define OUT3_ON() GPIO_SetBits(GPIOB, GPIO_Pin_8)
	#define OUT3_OFF() GPIO_ResetBits(GPIOB, GPIO_Pin_8)

	#define OUT4_ON() GPIO_SetBits(GPIOA, GPIO_Pin_15)
	#define OUT4_OFF() GPIO_ResetBits(GPIOA, GPIO_Pin_15)

	#define OUT5_ON() GPIO_SetBits(GPIOB, GPIO_Pin_3)
	#define OUT5_OFF() GPIO_ResetBits(GPIOB, GPIO_Pin_3)

	#define OUT6_ON() GPIO_SetBits(GPIOB, GPIO_Pin_4)
	#define OUT6_OFF() GPIO_ResetBits(GPIOB, GPIO_Pin_4)

	#define JUMPER_1() GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)
	#define JUMPER_2() GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)

	#define INP_1_READ() GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)
	#define INP_2_READ() GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6)
	#define INP_3_READ() GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)
	
	#define TIM_PRESCALER 1000
	#define TIM_PERIOD 1000
    
    typedef struct {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
        uint8_t brightness;
    } t_color;

    extern t_color Color;

	extern DMA_InitTypeDef DMA_InitStruct;
	extern TIM_TimeBaseInitTypeDef TIMER_Init_USER;

	extern uint16_t USART1_TX_Bufer[SIZE_STRIP * 12];
	extern uint16_t USART2_TX_Bufer[SIZE_STRIP * 12];
	
	extern uint32_t SendBuffer[SIZE_STRIP];
	extern double BaseArray[SIZE_TAIL + 1];
	extern double TempArray[SIZE_TAIL + 1];
    
    extern t_color sColor;
    extern t_color eColor;
    
    #define DIVIDER 5000
    typedef enum {
        STATUS_STANDBY = 0,
        STATUS_BLOCK,
        STATUS_ACTIV,
        STATUS_ERROR,
        STATUS_RED_to_BLU,
        STATUS_RED_to_GRN,
        STATUS_GRN_to_RED,
        STATUS_GRN_to_BLU,
        STATUS_BLU_to_GRN,
        STATUS_BLU_to_RED
    }e_status;
    
    extern e_status LEDstatus;
   
    void F4030C8_Setup(void);
	void Fill_In_The_DMA_Bufer(uint16_t* USART_TX_Bufer, uint32_t* Input_Colour_Bufer, uint16_t n);
	void DMA1_Channel4_5_IRQHandler(void);
	void DMA1_Channel2_3_IRQHandler(void);
	
	/****************************************************/
	// FastLed function
	int random(int i);
	void showStrip(void);
	void setPixel(int Pixel, uint8_t red, uint8_t green, uint8_t blue);
	void setAll(uint8_t red, uint8_t green, uint8_t blue);
	void delay_us(uint32_t delay_us);
	void delay_ms(uint16_t delay_ms);
    t_color RGBW_gradient(float progress, t_color start, t_color end);
    t_color RGB_gradient(float progress, t_color start, t_color end);
    uint32_t RGB_to_BRG(uint32_t color);
	/****************************************************/
	// effects
	void RGBLoop(void);
	void FadeInOut(uint8_t red, uint8_t green, uint8_t blue, int d);
	void AlarmFadeInOut(uint8_t red, uint8_t green, uint8_t blue, int d);
	void StandBy(uint8_t red, uint8_t green, uint8_t blue, int Count, int SpeedDelay, int OnlyOne);
	void Strobe(uint8_t red, uint8_t green, uint8_t blue,
								int StrobeCount, int FlashDelay, int EndPause);
	void CylonBounce(uint8_t red, uint8_t green, uint8_t blue,
								int EyeSize, int SpeedDelay, int ReturnDelay);
	void Twinkle(uint8_t red, uint8_t green, uint8_t blue,
								int Count, int SpeedDelay, int OnlyOne);
	void TwinkleRnd(int Count, int SpeedDelay, int OnlyOne);
	void Sparkle(uint8_t red, uint8_t green, uint8_t blue, int SpeedDelay);
	void RunningLights(uint8_t red, uint8_t green, uint8_t blue, int WaveDelay);
	void colorWipe(uint8_t red, uint8_t green, uint8_t blue, int SpeedDelay);
	void rainbowCycle(int SpeedDelay);
	void theaterChaseRainbow(int SpeedDelay);
	void Fire(int Cooling, int Sparking, int SpeedDelay);
	void meteorRain(uint8_t red, uint8_t green, uint8_t blue, uint8_t meteorSize, uint8_t meteorTrailDecay, uint8_t meteorRandomDecay, int SpeedDelay);
    
    void FirstSetArray(int i);
    void SetArray(int i);
    void setSendBufferArray(int i, t_color bColor, t_color tColor);
    
    
 #endif
