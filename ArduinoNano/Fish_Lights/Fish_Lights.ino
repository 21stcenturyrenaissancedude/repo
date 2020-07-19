// Lights for an aquarium using NeoPixels
// Using short presses of the button changes the color.
// Using a long press of the button, turns off the lights.
// Code remembers last color, and turns on to that color
//   if off and a short button press.

// NEOPIXEL BEST PRACTICES for most reliable operation:
// - Add 1000 uF CAPACITOR between NeoPixel strip's + and - connections.
// - MINIMIZE WIRING LENGTH between microcontroller board and first pixel.
// - NeoPixel strip's DATA-IN should pass through a 300-500 OHM RESISTOR.
// - AVOID connecting NeoPixels on a LIVE CIRCUIT. If you must, ALWAYS
//   connect GROUND (-) first, then +, then data.

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN    11

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT  12

#define INPUT_PIN  2

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)


// setup() function -- runs once at startup --------------------------------

void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(255); // Set BRIGHTNESS to about 1/5 (max = 255)

  pinMode(INPUT_PIN, INPUT_PULLUP);
}

int val = 0;
bool pressed = false;
unsigned long startTime;
int state = 0;

// loop() function -- runs repeatedly as long as board is on ---------------

void loop() {

  val = digitalRead(INPUT_PIN);
  if ((val == 1) && pressed)
  {
    pressed = false;
    unsigned long duration;
    duration = millis() - startTime;
    if ((duration > 100) && (duration < 1000))
    {
      // Change lights
      if (state == 0)
      {
        colorWipe(strip.Color(255,   255,   255), 50); // White
      }
      else if (state == 1)
      {
        colorWipe(strip.Color(255,   255,   0), 50); // Yellow
      }
      else if (state == 2)
      {
        colorWipe(strip.Color(255,   0,   255), 50); // Purple
      }
      else if (state == 3)
      {
        colorWipe(strip.Color(0,   255,   255), 50); // Cyan
      }
      else if (state == 4)
      {
        colorWipe(strip.Color(255,   0,   0), 50); // Red
      }
      else if (state == 5)
      {
        colorWipe(strip.Color(0,   255,   0), 50); // Green
      }
      else if (state == 6)
      {
        colorWipe(strip.Color(0,   0,   255), 50); // Blue
      }
      else if (state == 7)
      {
        colorWipe(strip.Color(0,   0,   0), 50); // Blue
      }
      state++;
      if (state > 7)
      {
        state = 0;
      }
    }
    else if (duration >= 1000)
    {
      // Turn off lights
      colorWipe(strip.Color(  0,   0, 0), 50); // Off

      // Decrement state so remains in same state
      if (state == 0)
      {
        state = 7;
      }
      else
      {
        state--;
      }
    }
  }
  if ((val == 0) && !pressed)
  {
    startTime = millis();
    pressed = true;
  }
}


// Some functions of our own for creating animated effects -----------------

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}
