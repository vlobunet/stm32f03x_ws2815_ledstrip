#include "PCB705.h"

/***************** RGBLoop **************************/
void RGBLoop(void){
	int k, j;;
  for(j = 0; j < 3; j++ ) {
    for(k = 0; k < 256; k++) {
      switch(j) {
        case 0: setAll(k,0,0); break;
        case 1: setAll(0,k,0); break;
        case 2: setAll(0,0,k); break;
      }
      showStrip();
			delay_ms(3);
    }
    // Fade OUT
    for(k = 255; k >= 0; k--) {
      switch(j) {
        case 0: setAll(k,0,0); break;
        case 1: setAll(0,k,0); break;
        case 2: setAll(0,0,k); break;
      }
      showStrip();
			delay_ms(3);
    }
  }
}

void StandBy(uint8_t red, uint8_t green, uint8_t blue,
								int Count, int SpeedDelay, int OnlyOne) {
	int i;

	setAll(0,0,10);
	for (i = 0; i < Count; i++) {
		setPixel(random(SIZE_STRIP), red, green, blue);
		showStrip();
		delay_ms(SpeedDelay);
		if(OnlyOne) {
			setAll(0,0,0);
		}
	}
	delay_ms(SpeedDelay);
}
/************************* FadeInOut ********************/
void AlarmFadeInOut(uint8_t red, uint8_t green, uint8_t blue, int d){
	float r, g, b;
	int k, n;

	for(k = 0; k < 256; k = k + 15) {
		r = (k / 256.0) * red;
		g = (k / 256.0) * green;
		b = (k / 256.0) * blue;
		setAll(r, g, b);
		showStrip();
		delay_ms(d);
	}
	for (n = 1; n > 0; n--) {
		setAll(0,0,0);
		delay_ms(50);
		setAll(r, g, b);
		delay_ms(50);
	}
	setAll(0,0,0);
	delay_ms(50);

	for(k = 255; k >= 0; k = k - 2) {
		r = (k / 256.0) * red;
		g = (k / 256.0) * green;
		b = (k / 256.0) * blue;
		setAll(r, g, b);
		showStrip();
		delay_ms(d);
	}
	delay_ms(100);
}
/**************************************************************/
void FadeInOut(uint8_t red, uint8_t green, uint8_t blue, int d){
	float r, g, b;
	int k;

	for(k = 0; k < 256; k++) {
		r = (k / 256.0) * red;
		g = (k / 256.0) * green;
		b = (k / 256.0) * blue;
		setAll(r, g, b);
		showStrip();
		delay_ms(d);
	}

	for(k = 255; k >= 0; k = k - 2) {
		r = (k / 256.0) * red;
		g = (k / 256.0) * green;
		b = (k / 256.0) * blue;
		setAll(r, g, b);
		showStrip();
		delay_ms(d);
	}
}
/********************** Strobe ***************************/
void Strobe(uint8_t red, uint8_t green, uint8_t blue,
		int StrobeCount, int FlashDelay, int EndPause){
	int j;
	for(j = 0; j < StrobeCount; j++) {
		setAll(red,green,blue);
		showStrip();
		delay_ms(FlashDelay);
		setAll(0,0,0);
		showStrip();
		delay_ms(FlashDelay);
	}

	delay_ms(EndPause);
}
/****************** CylonBounce ***************************/
void CylonBounce(uint8_t red, uint8_t green, uint8_t blue,
	int EyeSize, int SpeedDelay, int ReturnDelay){
	
	int i, j;

  for(i = 0; i < SIZE_STRIP - EyeSize - 2; i++) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(j = 1; j <= EyeSize; j++) {
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + EyeSize + 1, red/10, green/10, blue/10);
    showStrip();
    delay_ms(SpeedDelay);
  }

  delay_ms(ReturnDelay);

  for(i = SIZE_STRIP - EyeSize - 2; i > 0; i--) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(j = 1; j <= EyeSize; j++) {
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + EyeSize + 1, red/10, green/10, blue/10);
    showStrip();
    delay_ms(SpeedDelay);
  }
 
  delay_ms(ReturnDelay);
}
/*************************************************************/
void Twinkle(uint8_t red, uint8_t green, uint8_t blue,
								int Count, int SpeedDelay, int OnlyOne) {
	int i;

	setAll(0,0,0x00);
	for (i = 0; i < Count; i++) {
		setPixel(random(SIZE_STRIP), red, green, blue);
		showStrip();
		delay_ms(SpeedDelay);
		if(OnlyOne) {
			setAll(0,0,0x00);
		}
	}
	delay_ms(SpeedDelay);
}

void TwinkleRnd(int Count, int SpeedDelay, int OnlyOne) {
	int i;

	setAll(0,0,0);
	for (i = 0; i < Count; i++) {
		setPixel(random(SIZE_STRIP), random(255), random(255), random(255));
		showStrip();
		delay_ms(SpeedDelay);
		if(OnlyOne) {
			setAll(0,0,0);
		}
	}
	delay_ms(SpeedDelay);
}
/************************* ********************************/
void Sparkle(uint8_t red, uint8_t green, uint8_t blue, int SpeedDelay) {
  int Pixel = random(SIZE_STRIP);
  setPixel(Pixel,red,green,blue);
  showStrip();
  delay_ms(SpeedDelay);
  setPixel(Pixel,0,0,0);
}
/*****************************************************************************/
void RunningLights(uint8_t red, uint8_t green, uint8_t blue, int WaveDelay) {
  int Position = 0;
	int j, i;
	float level;
 
  for(j = 0; j < SIZE_STRIP * 2; j++) {
      Position++;
      for(i = 0; i < SIZE_STRIP; i++) {
				level = ((sin(i + Position) * 128 + 127) / 255);
        setPixel(i, level * red, level * green, level * blue);
      }
      showStrip();
      delay_ms(WaveDelay);
  }
}
/********************************************************************************/
void colorWipe(uint8_t red, uint8_t green, uint8_t blue, int SpeedDelay) {
  int i;
	for(i=0; i< SIZE_STRIP; i++) {
      setPixel(i, red, green, blue);
      showStrip();
      delay_ms(SpeedDelay);
  }
}
/********************************************************************************/

uint8_t *Wheel(uint8_t WheelPos) {
	static uint8_t c[3];

	if(WheelPos < 85) {
		c[0] = WheelPos * 3;
		c[1] = 255 - WheelPos * 3;
		c[2] = 0;
	} else if(WheelPos < 170) {
		WheelPos -= 85;
		c[0] = 255 - WheelPos * 3;
		c[1] = 0;
		c[2] = WheelPos * 3;
	} else {
		WheelPos -= 170;
		c[0] = 0;
		c[1] = WheelPos * 3;
		c[2] = 255 - WheelPos * 3;
	}
	return c;
}

void rainbowCycle(int SpeedDelay) {
	uint8_t *c;
	uint16_t i, j;

	for(j = 0; j < 256 * 5; j++) {
		for(i = 0; i< SIZE_STRIP; i++) {
			c = Wheel(((i * 256 / SIZE_STRIP) + j) & 255);
			setPixel(i, *c, *(c + 1), *(c + 2));
		}
		showStrip();
		delay_us(SpeedDelay);
	}
}
/**************************************************************************************/
void theaterChaseRainbow(int SpeedDelay) {
  uint8_t *c;
	int i, j, q;
 
  for (j = 0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (q = 0; q < 3; q++) {
        for (i = 0; i < SIZE_STRIP; i=i+3) {
          c = Wheel( (i+j) % 255);
          setPixel(i + q, *c, *(c+1), *(c+2));    //turn every third pixel on
        }
        showStrip();
       
        delay_us(SpeedDelay);
       
        for (i=0; i < SIZE_STRIP; i=i+3) {
          setPixel(i+q, 0,0,0);        //turn every third pixel off
        }
    }
  }
}
/***************************************************************************************/
void fadeToBlack(int ledNo, uint8_t fadeValue) {
	uint32_t oldColor;
	uint8_t r, g, b;

	oldColor = SendBuffer[ledNo];
	r = (oldColor & 0x00ff0000UL) >> 16;
	g = (oldColor & 0x0000ff00UL) >> 8;
	b = (oldColor & 0x000000ffUL);

	r = (r <= 10) ? 0 : (int) r-(r * fadeValue / 256);
	g = (g <= 10) ? 0 : (int) g-(g * fadeValue / 256);
	b = (b <= 10) ? 0 : (int) b-(b * fadeValue / 256);

	setPixel(ledNo, r,g,b); 
}

void meteorRain(uint8_t red, uint8_t green, uint8_t blue,
	uint8_t meteorSize,
	uint8_t meteorTrailDecay, uint8_t meteorRandomDecay,
	int SpeedDelay) {  
	int i, j;
	
    setAll(0,0,0);
	for(i = 0; i < SIZE_STRIP * 2; i++) {
		for(j=0; j < SIZE_STRIP; j++) {
			if( (!meteorRandomDecay) || (random(10) > 5) ) {
				fadeToBlack(j, meteorTrailDecay );        
			}
		}

		for(j = 0; j < meteorSize; j++) {
			if( ( i - j < SIZE_STRIP) && (i - j >= 0) ) {
				setPixel(i - j, red, green, blue);
			}
		}

		showStrip();
		delay_ms(SpeedDelay);
	}
}
