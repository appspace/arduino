#include <Ultrasonic.h>
#include <LiquidCrystal.h>

#define TRIGGER_PIN  9
#define ECHO_PIN     8

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(17, 2);
  lcd.print("DISTANCE IS");
}

void loop() {
  float cmMsec, inMsec;
  long microsec = ultrasonic.timing();
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  // print the number of seconds since reset:
  lcd.print(cmMsec);
}

