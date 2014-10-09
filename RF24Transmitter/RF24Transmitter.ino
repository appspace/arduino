#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

// Hardware configuration: Set up nRF24L01 radio on SPI bus plus 
// pins 9, 10 for UNO
// ping 48, 49 for Mega
RF24 radio(9, 10);

uint8_t addresses[][6] = {"1Node","2Node"};

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
  radio.openWritingPipe(addresses[1]);
}

void loop(void) {
  sendHeader();
  delay(100);  //Let the capasitor charge
  waitForHeaderResponse();
  delay(100);  //Let the capasitor charge
  //while (!waitForHeaderResponse()) {
  //  sendHeader();
  //}
  
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
  
  int dataCRC = calcCRC(data, sizeof(data));
  header[2] = dataCRC;
  header[3] = 'O';
  header[4] = 'K';
  printf("Sending %u|%u|%u|%c%c\n\r", header[0], header[1], header[2], header[3], header[4]);
  boolean sendOK = radio.write( &header, 5 );
  if (sendOK) { printf("Send success.\n\r"); }
  else        { printf("Send FAILED!!!\n\r"); }
}

boolean waitForHeaderResponse() {
  printf("Waiting for ACK\n\r");
  radio.openReadingPipe(1,addresses[0]);
  radio.startListening();
  while (!radio.available()) {
  }

  char response[2];
  radio.read( &response, sizeof(response) );
  printf("Received response: %s\n\r", response);
  
  return false;
}

  /*unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
  boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not
    
  while ( ! radio.available() ){                             // While nothing is received
    if (micros() - started_waiting_at > 1500000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
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
    } else {
      printf("Received unexpected response: %d\n\r", response);
    }
  }
  */
