#include <Wire.h>
#include "Adafruit_GFX.h"
#include <Adafruit_SSD1306.h>
#include "Adafruit_HT1632.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Wire library buffer size
#define WIRE_BUFFER_SIZE 32

#define MAIN_LOOP_DELAY_MSEC 5u

/*
This is a basic demo program showing how to write to a HT1632
These can be used for up to 16x24 LED matrix grids, with internal memory
and using only 3 pins - data, write and select.
Multiple HT1632's can share data and write pins, but need unique CS pins.
*/

#define HT_DATA 2
#define HT_WR 3
#define HT_CS 4

const int buttonPin = 0;     // the number of the pushbutton pin
const int ledPin =  LED_BUILTIN;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

int i = 0;
int count = 5000;

Adafruit_HT1632 matrix = Adafruit_HT1632(HT_DATA, HT_WR, HT_CS);

void setup() {
  Serial.begin(9600);
  Serial.println("djjw ... test HT1632C");
//  matrix.begin(ADA_HT1632_COMMON_16NMOS);
  matrix.begin(ADA_HT1632_COMMON_16PMOS);

  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT_PULLUP);

  // initiate the Wire library and join I2C bus as Master (no parameter needed for Master)
  Wire.begin();  

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
  display.println(F(" LED # "));
  display.display(); // Show initial text
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  display.setCursor(10, 25);
  display.print(i, DEC);
  display.display(); // Show initial text

  delay(100);
  matrix.clearScreen();
}

void testMatrix(Adafruit_HT1632 matrix) {  

  for(count = 0; count < 2500; count++)
  {
    if(LOW == digitalRead(buttonPin))
    {
      delay(250);
      while(LOW == digitalRead(buttonPin))
        ;
      delay(150);
      while(HIGH == digitalRead(buttonPin))
        ;
//      count = 10000;
      delay(500);
      break;
    }
    delay(1);
  }
  
  display.setCursor(10, 25);
  display.print(i, DEC);
  display.display(); // Show initial text

  matrix.setPixel(i++);
  matrix.writeScreen();
  delay(2);

  if(i >= 24*16)
  {
    i = 0;
    display.setCursor(10, 25);
    display.print("        ");
  }

//  delay(5000);

  
//  for (int i=0; i<24*16; i++) {
//    matrix.setPixel(i);
//    matrix.writeScreen();
//    delay(2);
//  }
//
//  delay(100);

//  

  // blink!
//  matrix.blink(true);
//  delay(2000);
//  matrix.blink(false);

//  // Adjust the brightness down
//  for (int8_t i=15; i>=0; i--) {
//   matrix.setBrightness(i);
//   delay(100);
//  }
//  // then back up
//  for (uint8_t i=0; i<16; i++) {
//   matrix.setBrightness(i);
//   delay(100);
//  }

//  // Clear it out
//  for (int i=(24*16)-1; i>=0; i--) {
//    matrix.clrPixel(i);
//    matrix.writeScreen();
//    delay(2);
//  }

//  delay(2000);
}

void loop() {
  testMatrix(matrix);
}
