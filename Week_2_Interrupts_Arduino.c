#define LED1 6
#define LED2 7
#define PIN_BUTTON 2

volatile bool button_flag = false; 
bool led_state = false;
unsigned long previousMillis = 0; 
const long interval = 1000; 

void setup()
{
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_BUTTON, INPUT);
  
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON), buttonPress, CHANGE);
}

void loop()
{
  unsigned long currentMillis = millis();
  
  if(currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    if (button_flag) {
      digitalWrite(LED_BUILTIN, led_state);
    } else {
      digitalWrite(LED1, led_state);
      digitalWrite(LED2, !led_state);
    }
    
    led_state = !led_state;
  }
}

void buttonPress() {
  if (digitalRead(PIN_BUTTON) == HIGH) {
    button_flag = true;
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
  } else {
    button_flag = false;
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED_BUILTIN, LOW);
  }
}
