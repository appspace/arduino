#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

// Hardware configuration: Set up nRF24L01 radio on SPI bus plus 
// pins 9, 10 for UNO
// ping 48, 49 for Mega
RF24 radio(48, 49);

uint8_t addresses[][6] = {"1Node","2Node"};

void setup() {
  Serial.begin(115200);
  printf_begin();
  printf("\n\rRF24-receiver\n\r");

  // Setup and configure rf radio
  radio.begin();                          // Start up the radio
  radio.setAutoAck(1);                    // Ensure autoACK is enabled
  radio.setRetries(15,15);                // Max delay between retries & number of retries
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1,addresses[1]);
}

void loop(void) {
  radio.startListening();                                       // Now, resume listening so we catch the next packets.     
  char header[5];
  if ( radio.available()) {
    radio.read( &header, 5 );             // Get the payload
    printf("Received %u|%u|%u|%c%c\n\r", header[0], header[1], header[2], header[3], header[4]);
    if (header[0]==0x01 && header[3]=='O' && header[4]=='K') {
      printf("It looks like we received a header. Sending back OK\n\r");
      
      radio.stopListening();                                      // First, stop listening so we can talk      
      delay(100);    //Let capacitor charge
      char ack[] = "OK";
      boolean sendOK = radio.write( &ack, sizeof(ack) );              // Send the final one back.
      if (sendOK) {
        printf("Succesfully sent packet\n\r");
      } else {
        printf("Response send failed\n\r");
        radio.printDetails();
      }
      delay(300);
    } else {
      printf("Received data packet %c%c%c%c%c\n\r", header[0], header[1], header[2], header[3], header[4]);
    }
  }

}
