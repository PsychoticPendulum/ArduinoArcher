#include <FastLED.h>

#define PIN       0x02
#define LEDCOUNT  0x3c
#define MAXVAL    0x40
#define STEPVAL   0x08
#define SLEEPVAL  0x40

CRGB fg_led[LEDCOUNT];
CRGB bg_led[LEDCOUNT];

struct Color {
  short m;  // Max Value
  short n;  // Stepping Value
  short r;
  short g;
  short b;
};
struct Color col = {
  MAXVAL, STEPVAL, MAXVAL, 0, 0
};

void setup() {
  FastLED.addLeds<WS2812, PIN, GRB>(fg_led, LEDCOUNT);
  Clear();
}

/*
   _                  
  | | ___   ___  _ __  
  | |/ _ \ / _ \| '_ \ 
  | | (_) | (_) | |_) |
  |_|\___/ \___/| .__/ 
                |_| 
*/

void Cycle(struct Color *col) {
  // Error correction. If any of the values is bigger than $MAXVAL the value is set to $MAXVAL
  if (col->r > col->m) { col->r = col->m; }
  if (col->g > col->m) { col->g = col->m; }
  if (col->b > col->m) { col->b = col->m; }
  // Shift colors through the whole spectrum of $MAXVAL^3
  if (col->r == col->m && col->b > 0)  { col->b -= col->n; }
  if (col->r == col->m && col->b == 0) { col->g += col->n; }
  if (col->g == col->m && col->r > 0)  { col->r -= col->n; }
  if (col->g == col->m && col->r == 0) { col->b += col->n; }
  if (col->b == col->m && col->g > 0)  { col->g -= col->n; }
  if (col->b == col->m && col->g == 0) { col->r += col->n; }
}

void CopyLEDs() {
  for (int i = 0; i < LEDCOUNT; i++) { bg_led[i] = fg_led[i]; }  
}

void MoveLEDs() {
  CopyLEDs();
  Cycle(&col);
  fg_led[0] = CRGB(col.r,col.g,col.b); // Set the first LED to a base value. To wrap around use fg_led[0] = bg_led[MAXVAL];
  for (int i = 1; i < LEDCOUNT; i++) { fg_led[i] = bg_led[i-1]; }  
}

void Clear() {
  for (int i = 0; i < LEDCOUNT; i++) { fg_led[i] = CRGB(0,0,0); }  
  FastLED.show();
}

void loop(void) {
  MoveLEDs();
  FastLED.show();
  delay(SLEEPVAL);
}
