#include <SPI.h>
#include <Ethernet.h>

int RELAY_ON = LOW;
int RELAY_OFF = HIGH;
int relayPin[] = {31, 33, 35, 37, 39}; 
boolean relayState[] = {RELAY_OFF, RELAY_OFF, RELAY_OFF, RELAY_OFF, RELAY_OFF};
const int pinsConnected = 5;
byte mac[] = { 0xEE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1, 177);
IPAddress gateway(192,168,1, 1);
IPAddress subnet(255, 255, 0, 0);

EthernetServer server(23);
boolean alreadyConnected = false; // whether or not the client was connected previously

void setup() {
  for (int i=0; i<pinsConnected; i++) {
    pinMode(relayPin[i], OUTPUT);
    digitalWrite(relayPin[i], RELAY_OFF);
  }

  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  
  Serial.begin(115200);
  
  Serial.print("Relay server address:");
  Serial.println(Ethernet.localIP());
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
          Serial.print(state, BIN);
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
