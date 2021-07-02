#include "PCB705.h"

DMA_InitTypeDef DMA_InitStruct;
TIM_TimeBaseInitTypeDef TIMER_Init_USER;

uint16_t USART1_TX_Bufer[SIZE_STRIP * 12];
uint16_t USART2_TX_Bufer[SIZE_STRIP * 12];

uint32_t SendBuffer[SIZE_STRIP];
double BaseArray[SIZE_TAIL + 1];
double TempArray[SIZE_TAIL + 1];

int HeadPosition = -1;
uint8_t flagIsNewCircle = 0;

t_color Color;

t_color sColor = {0x00, 0x00, 0x8, 0x00};
t_color eColor = {0x00, 0xff, 0xff, 0x00};
volatile uint8_t status_strip = 0;



e_status LEDstatus = STATUS_BLU_to_GRN;
volatile uint32_t counter = 0;
void SysTick_Handler(void) {
    /*if(counter >= DIVIDER) LEDstatus = (LEDstatus + 1) % 10;
    counter = (counter + 1) & DIVIDER;*/
}

void SetPanelColor(uint32_t color, int n) {
	if (n > SIZE_STRIP || n < 0) return;

    SendBuffer[n] = RGB_to_BRG(color);
}

void SetAllStrip(uint32_t color) {
	int i;

	for(i = 0; i < SIZE_STRIP; i++)
		SetPanelColor(color, i);
}

void SetTempArray (int step, int i, int s) {
	double tmp;

	while (++i <= SIZE_TAIL) TempArray[i] = BaseArray[i];					    // копируем значения опорного массива во временный
	while (++s <= step) {                                                       // пока не достигнем нужной глубины
		i = SIZE_TAIL + 1;                                                      // начинаем с конца
		while (--i >= 0) {                                                      // до тех пор пока не дойдем до 0
			tmp = (BaseArray[i] - BaseArray[i - 1]) / SIZEE_STEP;               // получаем значение делителя
			if (tmp < 0) {                                                      // если значение отрицательное
				TempArray[i] += (tmp * -1);                                     // увеличиваем значение яркости на делитель
				if (TempArray[i] > MAX_LIGHT) TempArray[i] = MAX_LIGHT;         // если будет пересвет - записываем максимальное значение
			} else {                                                            // но если делитель положительный
				TempArray[i] -= tmp;                                            // уменьшаем яркость пикселя на делитель
				if (TempArray[i] <= MIN_LIGHT) TempArray[i] = 5;                // если получили отрицательное значение - ставим минимальное
			}
		}
	}
}

void setArrayPixel(int headPos, int step) {
    signed int i = headPos;
    signed int size = headPos - SIZE_TAIL;
    
    SetTempArray(step, -1, 0);
    while (i >= 0 && i > size) {
        int p = SIZE_TAIL - (headPos - i);
        SetPanelColor(TempArray[p], i);
        if (headPos >= SIZE_STRIP) {
                SetPanelColor(TempArray[p], i - SIZE_STRIP);
        }
        i--;
    }
    SetPanelColor(0, i);
}

void active_effect(void) {
    static int isIncrement = 0;
    if (isIncrement == 0)
            HeadPosition = (HeadPosition + 1) % (SIZE_STRIP + SIZE_TAIL);
    if (HeadPosition == SIZE_STRIP + SIZE_TAIL - 1)
                flagIsNewCircle = 1;
    if (HeadPosition == 0 && flagIsNewCircle == 1) HeadPosition = SIZE_TAIL;
    setArrayPixel(HeadPosition, isIncrement);
    isIncrement = (isIncrement + 1) % (SIZEE_STEP + 1);
}

void TIM16_IRQHandler(void) {
    

    showStrip();
    TIM16->SR = (uint16_t)~TIM_IT_Update;
}


void CreateSrcArray_static() {
	BaseArray[0] = 1;
	BaseArray[1] = 7;
	BaseArray[2] = 25;
	BaseArray[3] = 90;
	BaseArray[4] = 150;
	BaseArray[5] = 25;
	BaseArray[6] = 2;
}

void CreateSrcArray_auto() {
    int i = SIZE_TAIL;
    BaseArray[i] = 0;
    while (--i > SIZE_TAIL - SIZE_HEAD)
        BaseArray[i] = BaseArray[i + 1] + (70 / SIZE_HEAD);
		BaseArray[i] = MAX_LIGHT;
    while (--i >= 0) {
        BaseArray[i] = BaseArray[i + 1] - (MAX_LIGHT / (SIZE_TAIL - SIZE_HEAD));
        if (BaseArray[i] < 0) {
            while (i-- >= 0) BaseArray[i] = 2;
        }
    }
}

// 0xBBGGRR
int main(void) {
	F4030C8_Setup();
	//CreateSrcArray_static();
	//CreateSrcArray_auto();
	//SetAllStrip(0);
    /********************************************************/
    FirstSetArray(-1);
    SetArray(-1);
    /********************************************************/
	while (1) {
        switch (LEDstatus) {
            case STATUS_STANDBY:    setSendBufferArray(-1, sColor, eColor);     break;
            case STATUS_ACTIV:      active_effect();                            break;
            case STATUS_BLOCK:      Strobe(0xff, 0xff, 0xff, 10, 20, 500);      break;
            case STATUS_ERROR:      AlarmFadeInOut(0xff, 0x00, 0x00, 4);        break;
            case STATUS_BLU_to_GRN: meteorRain(0xff,0xff,0xff, 2, 127, 1, 20);  break;
            case STATUS_BLU_to_RED: theaterChaseRainbow(2000);                  break;
            case STATUS_GRN_to_BLU: rainbowCycle(1000);                         break;
            case STATUS_GRN_to_RED: RunningLights(0,0,0xff, 20);                break;
            case STATUS_RED_to_BLU: RGBLoop();                                  break;
            case STATUS_RED_to_GRN: colorWipe(random(255), random(255), random(255), 10); break;
            default: break;
        }
		//AlarmFadeInOut(0xff, 0x00, 0x00, 4);
		//StandBy(0x00, 0, 0xff, 10, 100, 0);
		/*****************************************************/
		//RGBLoop();
		/*****************************************************/
		 //FadeInOut(0xff, 0x00, 0x00, 4); // red
		 //FadeInOut(0xff, 0xff, 0xff); // white
		 //FadeInOut(0x00, 0x00, 0xff); // blue
		/*****************************************************/
		// Strobe(0xff, 0xff, 0xff, 10, 20, 500);
		/*****************************************************/
		// CylonBounce(0xff, 0, 0, 8, 5, 10);
		/*****************************************************/
		// Twinkle(0x00, 0, 0xff, 10, 200, 0);
		// TwinkleRnd(15, 2, 0);
		/*****************************************************/
		// Sparkle(0xff, 0xff, 0xff, 0);
		// Sparkle(random(255), random(255), random(255), 0);
		/*****************************************************/
		// RunningLights(0,0,0xff, 20);        // blue		
		/****************************************************/
		// colorWipe(random(255), random(255), random(255), 10);
		// colorWipe(0,0,0, 10);
		/****************************************************/
		// rainbowCycle(1000);
		/*****************************************************/
		//theaterChaseRainbow(2000);
		/*****************************************************/
		// meteorRain(0xff,0xff,0xff, 2, 127, 1, 10);
		/*****************************************************/
	}	
}
