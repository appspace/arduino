#include <LiquidCrystal.h>
#include <Wire.h>

#define DEVICE (0x53)    //ADXL345 device address
#define TO_READ (6)        //num of bytes we are going to read each time (two bytes for each axis)
#define LCD_EN_PIN   11
#define LCD_RS_PIN   12

byte buff[TO_READ] ;    //6 bytes buffer for saving data read from the device
char dataOutput[16];          //string buffer to transform data before sending it to the serial port

LiquidCrystal lcd(LCD_RS_PIN, LCD_EN_PIN, 5, 4, 3, 2);

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(17, 2);
  lcd.print("Gyro data:");
  
  Wire.begin();        // join i2c bus (address optional for master)

  //Turning on the ADXL345
  writeTo(DEVICE, 0x2D, 0);      
  writeTo(DEVICE, 0x2D, 16);
  writeTo(DEVICE, 0x2D, 8);
}

void loop() {
  int regAddress = 0x32;    //first axis-acceleration-data register on the ADXL345
  int x, y, z;
  float xF, yF, zF;
  
  readFrom(DEVICE, regAddress, TO_READ, buff); //read the acceleration data from the ADXL345
  
  //each axis reading comes in 10 bit resolution, ie 2 bytes.  Least Significat Byte first!!
  //thus we are converting both bytes in to one int
  lcd.setCursor(0, 1);
  
  x = (((int)buff[1]) << 8) | buff[0];
  xF = (float)x/25.6;
  lcd.print(xF, 2);
  
  lcd.print('|');
  y = (((int)buff[3])<< 8) | buff[2];
  yF = (float)y/25.6;
  lcd.print(yF, 2);

  lcd.print('|');    
  z = (((int)buff[5]) << 8) | buff[4];
  zF = (float)z/25.6;
  lcd.print(zF, 2);
    
  //we send the x y z values as a string to the serial port
//  snprintf(dataOutput, 16, "%+d %+d %+d", x, y, z);  
//  snprintf(dataOutput, 16, "%4.2f %4.2f %4.2f", xF, yF, zF);
  //snprintf(dataOutput, 16, "%e", (float)3.1415926);
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  
  // print the distance in CM
  //lcd.print(dataOutput);
  //lcd.print(3.1415926, 5);
  
  delay(100);
  
}

void writeTo(int device, byte address, byte val) {
   Wire.beginTransmission(device); //start transmission to device 
   Wire.write(address);        // send register address
   Wire.write(val);        // send value to write
   Wire.endTransmission(); //end transmission
}

void readFrom(int device, byte address, int num, byte buff[]) {
  Wire.beginTransmission(device); //start transmission to device 
  Wire.write(address);        //sends address to read from
  Wire.endTransmission(); //end transmission
  
  Wire.beginTransmission(device); //start transmission to device (initiate again)
  Wire.requestFrom(device, num);    // request num bytes from device
  
  int i = 0;
  while(Wire.available())    //device may send less than requested (abnormal)
  { 
    buff[i] = Wire.read(); // receive a byte
    i++;
  }
  Wire.endTransmission(); //end transmission
}
