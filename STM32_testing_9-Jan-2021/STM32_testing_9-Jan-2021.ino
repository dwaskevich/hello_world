/*
 * Author:  David Waskevich
 * Date:    3-Jan-2021
 * Name:    sketch_jan03a_STM32_BluePill_test
 * 
 * Description: Testing one of the ebay Blue Pill boards
 *              - Flashed it with generic_boot20_pc13.bin using ST-Link v2
 *              - It enumerated as Maple Serial (COM8)
 *              - Blinking LED_BUILTIN - working
 *              
 * Update 9-Jan-2021:
 *              - Trying to get things working on my STM32 Blue Pill setup
 *              - LED blink - working
 *              - Added RGB LED - PWM working
 *              - Added String class to parse terminal input - working
 *                - Note - using Serial.available() to build input string
 *              - Added Rotary Encoder + Button - working
 *              - Added POT analog in - not tested yet
 *              - Added .96" SSD1306 OLED display
 *                - Note - had to downgrade Adafruit_GFX library to 1.7.2
 *                - Note - need pullups on I2C lines
 *              
 * Update 10-Feb-2022:
 *              - modified on ISC in Notepad++
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306_STM32.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define LED LED_BUILTIN
#define LED_ON LOW
#define LED_OFF HIGH

/* Define Rotary Encoder inputs (Clock, Data, Push Button) */
#define CLK PA5
#define DATA PA4
#define BUTTON PB9

#define POT PA0

/* Define tri-color LED pin assignments (can be driven with digital I/O or PWM for brightness control) */
#define RED PB1
#define GND PB0
#define GREEN PA7
#define BLUE PA6

/* Define an enum for FSM (Finite State Machine) */
typedef enum States {
  Red_LED,
  Green_LED,
  Blue_LED,
  MAX_NUM_STATES  
} States_t;

int currentState = Red_LED;

String inputString = "";      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
 
void setup() {
  // put your setup code here, to run once:

  pinMode(GND, OUTPUT);
  digitalWrite(GND, LOW);
  
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  
  pinMode(LED, OUTPUT);

  pinMode(CLK, INPUT);
  pinMode(CLK, INPUT_PULLUP);
  pinMode(DATA, INPUT);
  pinMode(DATA, INPUT_PULLUP);
  pinMode(BUTTON, INPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  
  Serial.begin(9600);  
  Serial.println("DJJW - STM32 setup - demo apps on Lenovo");
  // reserve 80 bytes for the inputString:
  inputString.reserve(80);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  // Clear the buffer
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(BLACK, WHITE);
  display.drawFastHLine(0, 0, 19, WHITE);
  display.drawFastHLine(40, 0, 19, WHITE);
  display.drawFastHLine(80, 0, 19, WHITE);
  display.drawFastVLine(0, 0, 9, WHITE);
  display.drawFastVLine(40, 0, 9, WHITE);
  display.drawFastVLine(80, 0, 9, WHITE);
  display.setCursor(1, 1);
  display.print("RED");
  display.setCursor(41, 1);
  display.print("GRN");
  display.setCursor(81, 1);
  display.println("BLU");
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0, 12);
  display.print(0, DEC);
  display.setCursor(40, 12);
  display.print(0, DEC);
  display.setCursor(80, 12);
  display.print(0, DEC);
  display.setCursor(0, 30);
  display.print("                      ");
  display.setCursor(0, 30);
  display.print("Current state RED");
  display.display(); // Show initial text
  display.setCursor(0, 12);
  
  delay(1000);
  
}

static uint8_t prevNextCode = 0;
static uint16_t store=0;

void loop() {
  // put your main code here, to run repeatedly:
  static int8_t val;
  static int c;
  static unsigned long previousMillis;
  static int16_t redVal, greenVal, blueVal = 0;
  char incomingByte = ' ';

  if(millis() - previousMillis >= 1000)
  {
    previousMillis = millis();
    digitalWrite(LED, !digitalRead(LED));
  }

  if(Serial.available() > 0)
  {
    // get the new byte:
    incomingByte = Serial.read();
    
    // add it to the inputString:
    inputString += incomingByte;
    // if the incoming character is a newline, set a flag so the main loop can do something about it
    if (incomingByte == '\n')
    {
      Serial.print(inputString);
      stringComplete = true;
    }
  }

  if (true == stringComplete) {
    Serial.println(inputString);

    if('r' == inputString.charAt(0))
    {
      inputString.remove(0, 2);
      Serial.println("red = " + inputString);
      redVal = inputString.toInt();
      if(redVal >= 255)
        redVal = 255;
      if(redVal <= 0)
        redVal = 0;
      Serial.println(redVal, DEC);
      display.setCursor(0, 12);
      display.print("   ");
      display.setCursor(0, 12);
      display.print(redVal, DEC);
      display.setCursor(0, 30);
      display.print("                      ");
      display.setCursor(0, 30);
      display.print("Current state RED");
      display.setCursor(0, 41);
      display.print("   ");
      display.setCursor(0, 41);
      display.print(redVal, DEC);
      display.display();
      analogWrite(RED, redVal);
      currentState = Red_LED;
    }
    if('g' == inputString.charAt(0))
    {
      inputString.remove(0, 2);
      Serial.println("green = " + inputString);
      greenVal = inputString.toInt();
      if(greenVal >= 255)
        greenVal = 255;
      if(greenVal <= 0)
        greenVal = 0;
      Serial.println(greenVal, DEC);
      display.setCursor(40, 12);
      display.print("   ");
      display.setCursor(40, 12);
      display.print(greenVal, DEC);
      display.setCursor(0, 30);
      display.print("                      ");
      display.setCursor(0, 30);
      display.print("Current state GREEN");
      display.setCursor(0, 41);
      display.print("   ");
      display.setCursor(0, 41);
      display.print(greenVal, DEC);
      display.display();
      analogWrite(GREEN, greenVal);
      currentState = Green_LED;
    }
    if('b' == inputString.charAt(0))
    {
      inputString.remove(0, 2);
      Serial.println("blue = " + inputString);
      blueVal = inputString.toInt();
      if(blueVal >= 255)
        blueVal = 255;
      if(blueVal <= 0)
        blueVal = 0;
      Serial.println(blueVal, DEC);
      display.setCursor(80, 12);
      display.print("   ");
      display.setCursor(80, 12);
      display.print(blueVal, DEC);
      display.setCursor(0, 30);
      display.print("                      ");
      display.setCursor(0, 30);
      display.print("Current state BLUE");
      display.setCursor(0, 41);
      display.print("   ");
      display.setCursor(0, 41);
      display.print(blueVal, DEC);
      display.display();
      analogWrite(BLUE, blueVal);
      currentState = Blue_LED;
    }
        
    // clear the string:
    inputString = "";
    stringComplete = false;    
  }

  if( val=read_rotary() ) {
      c +=val;
      
    switch(currentState)
    {
      case  Red_LED:
        redVal += val;
        if(redVal < 0)
          redVal = 255;
        if(redVal > 255)
          redVal = 0;
        analogWrite(RED, redVal);
        Serial.print("Red = ");
        Serial.println(redVal, DEC);
        display.setCursor(0, 12);
        display.print("   ");
        display.setCursor(0, 12);
        display.print(redVal, DEC);
        display.setCursor(0, 41);
        display.print("   ");
        display.setCursor(0, 41);
        display.print(redVal, DEC);
        display.display();
        break;
  
      case  Green_LED:
        greenVal += val;
        if(greenVal < 0)
          greenVal = 255;
        if(greenVal > 255)
          greenVal = 0;
        analogWrite(GREEN, greenVal);
        Serial.print("Green = ");
        Serial.println(greenVal, DEC);
        display.setCursor(40, 12);
        display.print("   ");
        display.setCursor(40, 12);
        display.print(greenVal, DEC);
        display.setCursor(0, 41);
        display.print("   ");
        display.setCursor(0, 41);
        display.print(greenVal, DEC);
        display.display();
        break;
  
      case  Blue_LED:
        blueVal += val;
        if(blueVal < 0)
          blueVal = 255;
        if(blueVal > 255)
          blueVal = 0;
        analogWrite(BLUE, blueVal);
        Serial.print("Blue = ");
        Serial.println(blueVal, DEC);
        display.setCursor(80, 12);
        display.print("   ");
        display.setCursor(80, 12);
        display.print(blueVal, DEC);
        display.setCursor(0, 41);
        display.print("   ");
        display.setCursor(0, 41);
        display.print(blueVal, DEC);
        display.display();
        break;
  
      default:
        break;
     }
  }

  if(0 == digitalRead(BUTTON))
   {
    delay(100);
    if(0 == digitalRead(BUTTON))
    {
      while(0 == digitalRead(BUTTON))
        digitalWrite(LED, HIGH);
      digitalWrite(LED, LOW);
      currentState++;
      if(currentState >= MAX_NUM_STATES)
        currentState = 0;
      switch(currentState)
      {
      case  Red_LED:
        Serial.print("Current state is RED ... ");
        Serial.print("redVal = ");
        Serial.println(redVal, DEC);
        display.setCursor(0, 30);
        display.print("                      ");
        display.setCursor(0, 30);
        display.print("Current state RED");
        display.setCursor(0, 41);
        display.print("   ");
        display.setCursor(0, 41);
        display.print(redVal, DEC);
        display.display();        
        break;
  
      case  Green_LED:
        Serial.print("Current state is GREEN ... ");
        Serial.print("greenVal = ");
        Serial.println(greenVal, DEC);
        display.setCursor(0, 30);
        display.print("                      ");
        display.setCursor(0, 30);
        display.print("Current state GREEN");
        display.setCursor(0, 41);
        display.print("   ");
        display.setCursor(0, 41);
        display.print(greenVal, DEC);
        display.display();
        break;
  
      case  Blue_LED:
        Serial.print("Current state is BLUE ... ");
        Serial.print("blueVal = ");
        Serial.println(blueVal, DEC);
        display.setCursor(0, 30);
        display.print("                      ");
        display.setCursor(0, 30);
        display.print("Current state BLUE");
        display.setCursor(0, 41);
        display.print("   ");
        display.setCursor(0, 41);
        display.print(blueVal, DEC);
        display.display();
        break;
  
      default:
        break;
      }
    }
  }
}


// A vald CW or CCW move returns 1, invalid returns 0.
int8_t read_rotary() {
  static int8_t rot_enc_table[] = {0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0};

  prevNextCode <<= 2;
  if (digitalRead(DATA)) prevNextCode |= 0x02;
  if (digitalRead(CLK)) prevNextCode |= 0x01;
  prevNextCode &= 0x0f;

   // If valid then store as 16 bit data.
   if  (rot_enc_table[prevNextCode] ) {
      store <<= 4;
      store |= prevNextCode;
      //if (store==0xd42b) return 1;
      //if (store==0xe817) return -1;
      if ((store&0xff)==0x2b) return -1;
      if ((store&0xff)==0x17) return 1;
   }
   return 0;
}
