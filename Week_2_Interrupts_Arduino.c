#define LED0 6
#define LED1 7
#define ONBOARD_LED 13
#define BUTTON 2

volatile byte button_flag = 0;

void setup() {
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(ONBOARD_LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  // Set up Timer1 for approximately a 1-second interval.
  TCCR1A = 0;
  TCCR1B = _BV(WGM12) | _BV(CS12) | _BV(CS10); // CTC mode, prescaler of 1024
  OCR1A = 15624; // approx 1 second with 16 MHz clock and 1024 prescaler
  TIMSK1 = _BV(OCIE1A); // Enable Timer1 compare match A interrupt
  
  attachInterrupt(digitalPinToInterrupt(BUTTON), button_ISR, RISING);
}

void loop() {
  // Empty loop.
}

void button_ISR() {
  if (button_flag == 0) {
    digitalWrite(LED0, HIGH);
    digitalWrite(LED1, HIGH);
    button_flag = 1;
  } else {
    digitalWrite(LED0, LOW);
    digitalWrite(LED1, LOW);
    digitalWrite(ONBOARD_LED, LOW);
    button_flag = 0;
  }
}

ISR(TIMER1_COMPA_vect) {
  if (button_flag == 0) {
    digitalWrite(LED0, !digitalRead(LED0));
    digitalWrite(LED1, !digitalRead(LED1));
  } else {
    digitalWrite(ONBOARD_LED, !digitalRead(ONBOARD_LED));
  }
}
