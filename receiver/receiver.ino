#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

// Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 9 & 10 
RF24 radio(48,49);

const uint64_t pipeAtoB = 0xE8E8F0F0E1LL;
const uint64_t pipeBtoA = 0xE8E8F0F0E2LL;


// Set up roles to simplify testing 
boolean role;                                    // The main role variable, holds the current role identifier
boolean role_ping_out = 1, role_pong_back = 0;   // The two different roles.

void setup() {
  Serial.begin(115200);
  printf_begin();
  printf("\n\rRF24-receiver\n\r");

  // Setup and configure rf radio
  radio.begin();                          // Start up the radio
  radio.setAutoAck(1);                    // Ensure autoACK is enabled
  radio.setRetries(15,15);                // Max delay between retries & number of retries
  radio.openWritingPipe(pipeBtoA);
  radio.openReadingPipe(1,pipeAtoB);
}

void loop(void) {
  radio.startListening();                                       // Now, resume listening so we catch the next packets.     
  if ( radio.available()) {
    char data[5];
    radio.read( &data[0], sizeof(data)+1 );             // Get the payload
    printf("Received %d%d\n\r", data[3], data[4]);
    if (data[0]==0x01 && data[3]=='O' && data[4]=='K') {
      printf("It looks like we received header");
      radio.stopListening();                                        // First, stop listening so we can talk      
      char ack = 0x06;
      radio.write( &ack, sizeof(char) );              // Send the final one back.      
    }
  }
}
