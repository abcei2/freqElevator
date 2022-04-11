/*
  Primer periodo:
    - para leer el duty.
  Segundo periodo (si se logra leer el duty):
    - se calcula duty y compara con anterior.
    - duty_set = true , si el duty actual y el anterior no difieren mucho, de lo contrario false.
  Consecutivos (si duty_set=true)
    - se genera la onda a la frecuencia especificada y el duty calculado.
    - se calcula duty y compara con anterior.
    - duty_set = true , si el duty actual y el anterior no difieren mucho, de lo contrario false.
*/
#define inputFreq 50.0
#define dutyMarginError 5.0
#define inputPeriod (1/inputFreq * 1000 * 1000)

const byte interruptPin = 2;
volatile byte pinChanges = LOW;
volatile byte pinState = LOW;
float dutyTimer;
unsigned int duty = 0;
boolean dutySet = false;

void setup() {
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);
}

void loop() {
  if (pinChanges) {
    pinChanges = !pinChanges;
    dutyCalculation();
  } 
}


void dutyCalculation() {    
  if (pinState) {
    dutyTimer = micros();
  } else if (!pinState) {
    
    float dutyDuration = micros() - dutyTimer;
    if (dutyDuration <= inputPeriod) {
      if (abs(duty - (int)(dutyDuration / inputPeriod * 100)) <= dutyMarginError) {
        dutySet = true;
      } else {
        duty = (int)((dutyDuration / inputPeriod) * 100);
        dutySet = false;
      }   
    } else {
      dutySet = false;
    }
    
  } 
}
void blink() {
  pinChanges = !pinChanges;
  pinState = !pinState;
}
