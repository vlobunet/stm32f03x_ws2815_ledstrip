#include "PCB705.h"

#define MAX_pixel 8
float set[MAX_pixel][4];

#define POSITION 0
#define DIRECTION 1
#define STEP 2
#define PROGRESS 3

#define DIR_UP 1
#define DIR_DWN 0

#define SLOW_STEP   (random(2) + 1)
#define MIDDLE_STEP (random(2) + 3)
#define FAST_STEP   (random(2) + 5)
#define SFAST_STEP  (random(2) + 7)

void FirstSetArray(int i) {
  while (++i < MAX_pixel)
    set[i][POSITION] = SIZE_STRIP + 1;
  i = -1;
  while (++i < SIZE_STRIP)
    SendBuffer[i] = sColor.red << 16 | sColor.green << 8 | sColor.blue;
}

static int checkArray(uint8_t rnd, uint8_t pos) {
  int i = -1;

  while (++i < MAX_pixel) {
    if (i == pos)
      continue;
    else if (set[i][POSITION] == rnd)
      return (-1);
  }
  return(rnd);
}

void SetArray(int i) {
  int r;
  uint8_t step;

  while (++i < MAX_pixel) {
      float n;
    if (set[i][POSITION] > SIZE_STRIP) {
      while ((r = checkArray((random(SIZE_STRIP)), i)) < 0) ;
      set[i][POSITION] = r;
      set[i][DIRECTION] = DIR_UP;
      step = random(4);
      switch (step) {
        case 0: n = SLOW_STEP; break;
        case 1: n = MIDDLE_STEP; break;
        case 2: n = FAST_STEP; break;
        case 3: n = SFAST_STEP; break;
      }
      set[i][STEP] = 0.003 * n;
      set[i][PROGRESS] = 0.0;
    }
  }
}

static float *searchPixel(int i) {
  int j = -1;

  while (++j < MAX_pixel) {
    if (set[j][POSITION] == i)
      return (set[j]);
  }
  return(NULL);
}

static uint32_t getColor(t_color sColor, t_color eColor, float *t) {
    t_color ret;

    if (t[DIRECTION] == 1) ret =  RGB_gradient(t[PROGRESS], sColor, eColor);
    else if (t[DIRECTION] == 0) ret =  RGB_gradient(t[PROGRESS], eColor, sColor);
    t[PROGRESS] += 0.006;
    if (t[PROGRESS] >= 1) {
        t[PROGRESS] = 0;
        if (t[DIRECTION == DIR_UP]) {
            t[DIRECTION] = DIR_DWN;
            ret = eColor;
        } else {
            t[POSITION] = SIZE_STRIP + 1;
            SetArray(-1);
            ret = sColor;
        }   
    } else t[PROGRESS] += t[STEP];
    return(ret.red << 16 | ret.green << 8 | ret.blue);
}

void setSendBufferArray(int i, t_color bColor, t_color tColor) {
    while (++i < SIZE_STRIP) {
        float *t = searchPixel(i);
        if (t != NULL) {
                SendBuffer[i] = RGB_to_BRG(getColor(bColor, tColor, t));
        } else SendBuffer[i] = RGB_to_BRG(bColor.red << 16 | bColor.green << 8 | bColor.blue);
    }
    delay_ms(5);
}

