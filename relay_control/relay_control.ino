const int relayPin_1 =  31;
const int relayPin_2 =  33;
const int relayPin_3 =  35;
const int relayPin_4 =  37;
const int relayPin_5 =  39;

int relayNumActive = 0;
int RELAY_ON = LOW;
int RELAY_OFF = HIGH;

void setup() {
  pinMode(relayPin_1, OUTPUT);  digitalWrite(relayPin_1, RELAY_OFF);
  pinMode(relayPin_2, OUTPUT);  digitalWrite(relayPin_2, RELAY_OFF);
  pinMode(relayPin_3, OUTPUT);  digitalWrite(relayPin_3, RELAY_OFF);
  pinMode(relayPin_4, OUTPUT);  digitalWrite(relayPin_4, RELAY_OFF);
  pinMode(relayPin_5, OUTPUT);  digitalWrite(relayPin_5, RELAY_OFF);
}

void loop()
{
    // set the LED with the ledState of the variable:
    //digitalWrite(relayPin_1, ledState);
    //digitalWrite(relayPin_2, ledState);
    //digitalWrite(relayPin_3, ledState);
    //digitalWrite(relayPin_4, ledState);
    //digitalWrite(relayPin_5, ledState);
  
}

