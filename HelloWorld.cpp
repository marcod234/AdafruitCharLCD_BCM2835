/*********************

Example code for the Adafruit RGB Character LCD Shield and Library

This code displays text on the shield, and also reads the buttons on the keypad.
When a button is pressed, the backlight changes color.

**********************/

// include the library code:
#include "Adafruit_RGBLCDShield.h"
#include "Adafruit_MCP23017.h"
#include <stdio.h>
#include <bcm2835.h>

// The shield uses the I2C SCL and SDA pins. On classic Arduinos
// this is Analog 4 and 5 so you can't use those for analogRead() anymore
// However, you can connect other I2C sensors to the I2C bus and share
// the I2C bus.


// These #defines make it easy to set the backlight color
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

uint8_t i=0;
int main() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):

  int i = bcm2835_init();

  if (!i)
  {
    printf("bcm2835_init failed. Are you running as root??\n");
    return 1;
  }
  
  if(!bcm2835_i2c_begin())
  {
    printf("i2c failed, run as root\n");
    return 1;
  }
  
  bcm2835_i2c_setSlaveAddress(0x20);
  bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_2500);
  
  Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
  lcd.begin(16, 2);
  lcd.setBacklight(WHITE);
  lcd.message("Hello World!");
  lcd.setCursor(0, 1);

  while(1)
  {
    uint8_t buttons = lcd.readButtons();
    if (buttons) {
      lcd.clear();
      lcd.setCursor(0,0);
      if (buttons & BUTTON_UP) {
		printf("UP\n");
        lcd.message("UP ");
        //lcd.setBacklight(RED);
      }
      if (buttons & BUTTON_DOWN) {
		printf("DOWN\n");
        lcd.message("DOWN ");
        //lcd.setBacklight(YELLOW);
      }
      if (buttons & BUTTON_LEFT) {
		  printf("LEFT\n");
        lcd.message("LEFT ");
        //lcd.setBacklight(GREEN);
      }
      if (buttons & BUTTON_RIGHT) {
		  printf("RIGHT\n");
        lcd.message("RIGHT ");
        //lcd.setBacklight(TEAL);
      }
      if (buttons & BUTTON_SELECT) {
		  printf("SELECT\n");
        lcd.message("SELECT ");
        //lcd.setBacklight(VIOLET);
      }
    }
  }
}
