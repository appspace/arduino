#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

// Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 9 & 10 
RF24 radio(48,49);

const uint64_t pipe = 0xE8E8F0F0E1LL;

// Set up roles to simplify testing 
boolean role;                                    // The main role variable, holds the current role identifier
boolean role_ping_out = 1, role_pong_back = 0;   // The two different roles.

void setup() {
  Serial.begin(115200);
  printf_begin();
  printf("\n\rRF24/examples/GettingStarted/\n\r");
  printf("*** PRESS 'T' to begin transmitting to the other node\n\r");

  // Setup and configure rf radio
  radio.begin();                          // Start up the radio
  radio.setAutoAck(1);                    // Ensure autoACK is enabled
  radio.setRetries(15,15);                // Max delay between retries & number of retries
  //radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1,pipe);

  radio.startListening();                 // Start listening
  radio.printDetails();                   // Dump the configuration of the rf unit for debugging
}

void loop(void) {
  //radio.startListening();                                       // Now, resume listening so we catch the next packets.     
    if( radio.available()){
      char data[5];
      radio.read( &data[0], sizeof(data)+1 );             // Get the payload
      printf("Received %s\n\r", data);      
      //radio.stopListening();                                        // First, stop listening so we can talk   
      //radio.write( &got_time, sizeof(unsigned long) );              // Send the final one back.      
      //radio.startListening();                                       // Now, resume listening so we catch the next packets.     
      //printf("Sent response %lu \n\r", got_time);  
   }
}
