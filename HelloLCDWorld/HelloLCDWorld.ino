#include <LiquidCrystal.h>
#include <Ultrasonic.h>

LiquidCrystal lcd(22, 24, 26, 28, 30, 32);

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Hello, world!");

}

void loop() {
  for (int number = 0; number < 256; number ++) {
    lcd.setCursor(0, 1);
    lcd.print(number, BIN);
  
    lcd.setCursor(9, 1);
    lcd.print(number, DEC);
  
    lcd.setCursor(13, 1);
    lcd.print(number, HEX);
    
    delay(1000);
  }
}

