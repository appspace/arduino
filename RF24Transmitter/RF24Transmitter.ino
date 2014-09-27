#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

// Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 9 & 10 
RF24 radio(9,10);
//test
const uint64_t pipeAtoB = 0xE8E8F0F0E1LL;
const uint64_t pipeBtoA = 0xE8E8F0F0E2LL;

char data[] = {"Helo"};
char buf[5];
char *ptr = &data[0];
boolean EndOfLine = false;
char header[3];
boolean waitingForACK = false;

void setup() {
  Serial.begin(115200);
  printf_begin();
  printf("\n\rRF24-transmitter\n\r");
  printf("Data: %s %d\n\r", data, sizeof(data));

  // Setup and configure rf radio
  radio.begin();                          // Start up the radio
  radio.setAutoAck(1);                    // Ensure autoACK is enabled
  radio.setRetries(15,15);                // Max delay between retries & number of retries
  radio.openWritingPipe(pipeAtoB);
  radio.openReadingPipe(1,pipeBtoA);
  //radio.printDetails();                   // Dump the configuration of the rf unit for debugging
  printf("*** WORKING IN TRANSMIT ROLE\n\r");
}

void loop(void) {
  sendHeader();
  while (!waitForHeaderResponse()) {
    sendHeader();
  }
  
  strcpy(buf,"\0\0\0\0\0");
  for (int i = 0; i<5 ; i++){
    if (*ptr == '\0') {
      EndOfLine = true;
      break;
    }
    buf[i] = *ptr;
    *ptr++;
    printf("%c", buf[i]);
    delay(300);
  }
 if (EndOfLine) {
   ptr = &data[0];
   EndOfLine = false;
 }
  
  if (!radio.write( &buf, sizeof(buf) )){
    printf("failed.\n\r");
  }
     // Try again 500ms later
  delay(500);
  
  
}

int calcCRC(char arr[],int len) {
  long sum = 0;
  int isum;
  while(len-->0) {
    sum += (arr[len]);
  }
  isum = sum % 100;
  return sum;
}

void sendHeader() {
  radio.stopListening();  // First, stop listening so we can talk.

  header[0]=0x01;
  uint8_t dataSize = sizeof(data);

  header[1]=dataSize;
  
  //int dataCRC = calcCRC(data, sizeof(data));
  //header[2] = dataCRC;
  //header[3] = 'O';
  //header[4] = 'K';
  printf("Sending len=%d and waiting for ack\n\r", header[1]);
  radio.write( &header, sizeof(header) );
}

boolean waitForHeaderResponse() {
  radio.startListening();                                    // Now, continue listening
    
  unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
  boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not
    
  while ( ! radio.available() ){                             // While nothing is received
    if (micros() - started_waiting_at > 200000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
      timeout = true;
      break;
    }
  }
        
  if ( timeout ) {                                             // Describe the results
     printf("Header response timed out.\n\r");
  } else {
    char response;                                 // Grab the response, compare, and send to debugging spew
    radio.read( &response, sizeof(char) );
    if (response == 0x06) {
      printf("Header succesfully received!\n\r");
      return true;
    }
  }
  return false;
}
