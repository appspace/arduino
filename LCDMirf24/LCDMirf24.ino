#include <SPI.h>
#include <Mirf.h>
#include <MirfHardwareSpiDriver.h>
#include <MirfSpiDriver.h>
#include <nRF24L01.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(22, 24, 26, 28, 30, 32);

void setup() {
  Serial.begin(115200);
  Serial.println("Nrf2401 Controller");
  Serial.println("UART Initialized."); 

  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Serial.println("Radio initialized. Setting address to 'serv1'");
  Mirf.setRADDR((byte *)"serv1");

  /*
   * Set the payload length to sizeof(unsigned long) the
   * return type of millis().
   *
   * NB: payload on client and server must be the same.
   */
  Mirf.payload = sizeof(unsigned long);
  Mirf.config();

  byte stat = Mirf.getStatus();
  Serial.print("Status: ");
  Serial.println(stat, BIN);
  Serial.println("Powering up receiver");
  stat = Mirf.getStatus();
  Serial.print("Status: ");
  Serial.println(stat, BIN);

  byte rf_setup = 0;
  Mirf.readRegister( RF_SETUP, &rf_setup, sizeof(rf_setup) );
  Serial.print( "rf_setup = " );
  Serial.println( rf_setup, BIN );

  Serial.println("Waiting for data...");

  lcd.begin(16, 2);
}

void loop() {
  printStatus();
  byte data[Mirf.payload];
  //isSending() returns the chip to receving after returning true.
  if (Mirf.dataReady() && !Mirf.isSending()) {
    Mirf.getData(data);
    boolean nonEmpty = false;
    int i=0;
    for (i=0; i<Mirf.payload; i++) {
      if (data[i]>0) {
        nonEmpty = true;
        break;
      }
    }
    if (nonEmpty) {
      Serial.println("Got packet");
      
      Serial.write(data, Mirf.payload);
      for (i=0; i<Mirf.payload; i++) {
        lcd.setCursor(i*3, 1);
        lcd.print(data[i], HEX);
      }
      Serial.println("------------");
    }
  }

}

void printStatus() {
  lcd.setCursor(0, 0);
  lcd.print("Status:");
  lcd.setCursor(8, 0);
  byte stat = Mirf.getStatus();
  lcd.print(stat, BIN);
  lcd.setCursor(11, 0);
  lcd.print("Ch:");
  lcd.setCursor(14, 0);
  lcd.print(Mirf.channel, BIN);
}
