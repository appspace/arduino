#include <SPI.h>
#include <Ethernet.h>
//#include <EthernetUdp.h>
#include <LiquidCrystal.h>

const byte LCD_RS = 48; const byte LCD_E = 49; 
const byte LCD_D4 = 44; const byte LCD_D5 = 45;
const byte LCD_D6 = 46; const byte LCD_D7 = 47;

int RELAY_ON = LOW;
int RELAY_OFF = HIGH;
int relayPin[] = {31, 33, 35, 37, 39}; 
boolean relayState[] = {RELAY_OFF, RELAY_OFF, RELAY_OFF, RELAY_OFF, RELAY_OFF};
const int pinsConnected = 5;
byte mac[] = { 0xEE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);
IPAddress gateway(192,168,1, 1);
IPAddress subnet(255, 255, 0, 0);

unsigned long port = 1981;

char packetBuffer[UDP_TX_PACKET_MAX_SIZE];

EthernetServer server(port);

boolean alreadyConnected = false; // whether or not the client was connected previously

LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

void setup() {
  lcd.begin(16, 2);
  initRelay();
  initDHCPNetwork();
  printNetworkInfo();
  
  Serial.begin(115200);
  
  Serial.print("Relay server address:");
  Serial.println(Ethernet.localIP());
}

void printNetworkInfo() {
  lcd.clear();
  lcd.print(Ethernet.localIP()[0], DEC);
  lcd.print(".");
  lcd.print(Ethernet.localIP()[1], DEC);
  lcd.print(".");
  lcd.print(Ethernet.localIP()[2], DEC);
  lcd.print(".");
  lcd.print(Ethernet.localIP()[3], DEC);
  lcd.setCursor(0, 1);
  lcd.print("Port: ");
  lcd.print(port, DEC);
}

void initStaticNetwork() {
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
}

void initDHCPNetwork() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DHCP Init");
  if (Ethernet.begin(mac)==0) {
    lcd.setCursor(0, 0);
    lcd.print("DHCP failure");
    lcd.setCursor(0, 1);
    lcd.print("Cannot obtain IP");
    for(;;) ;
  }
}

void initRelay() {
  lcd.setCursor(0,0);
  lcd.print("Relay Pins Init");
  for (int i=0; i<pinsConnected; i++) {
    pinMode(relayPin[i], OUTPUT);
    digitalWrite(relayPin[i], RELAY_OFF);
  }
}

void loop() {
  // wait for a new client:
  EthernetClient client = server.available();
  // when the client sends the first byte, say hello:
  if (client) {
  if (!alreadyConnected) {
      // clead out the input buffer:
      client.flush();    
      Serial.println("We have a new client");
      client.println("Hello, client!"); 
      alreadyConnected = true;
    } 

    if (client.available() > 0) {
      // read the bytes incoming from the client:
      char thisChar = client.read();
      // echo the bytes back to the client:
      server.write(thisChar);
      // echo the bytes to the server as well:
      Serial.write(thisChar);
      int relayInput = thisChar - 49;
      if (relayInput>=0 && relayInput<pinsConnected) {
          boolean state = relayState[relayInput];
          state = !state;
          Serial.print("Setting pin ");
          Serial.print(relayInput, DEC);
          Serial.print(" to ");
          Serial.println(state, BIN);
          relayState[relayInput] = state;
      }
    }
   
    activateRelays();
  }
}

void activateRelays() {
  for (int i=0; i<pinsConnected; i++) {
    digitalWrite(relayPin[i], relayState[i]);
  }
}
