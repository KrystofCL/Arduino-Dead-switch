void(* Stop) (void) = 0;

#pragma region Variables
const int LED_PIN = 3;
const int BUTTON_PIN = 10;
const int RELAY_PIN = 2;

bool running;
bool ready;

int lastTime;
int deltaTime;

/*ALL TIMES ARE IN MILIECONDS*/
int buttonTime; //Time untin next check in request
const int BUTTON_INTERVAL = 1000;
const int BUTTON_TIME_WINDOW = 10000; //How mutch time is for pressing the button
int buttonHoldTime;
const int BUTTON_HOLD_LIMIT = 2000;

int ledTime;
const int LED_INTERVAL = 500;
#pragma endregion

void setup() {
  #pragma region Pinmodes
  pinMode(LED_PIN,OUTPUT);
  pinMode(BUTTON_PIN,INPUT);
  pinMode(RELAY_PIN,OUTPUT);
  #pragma endregion

  digitalWrite(LED_PIN, 1);
  Serial.begin(9600); //Allows communication
}

void loop() {
  digitalWrite(RELAY_PIN,running);
  deltaTime = millis() - lastTime; //Detlat time calcilations
  lastTime = millis();             //
  
  if(running && ready) {
    CheckButton();
    BeepBuzzer();
  }
  else if(!digitalRead(BUTTON_PIN) && !ready) {
    ready = true;
  }
  else if(digitalRead(BUTTON_PIN) && ready) {
    running = true;
  }
}

void BlinkLed(int interval) {
  if(ledTime > 0)digitalWrite(LED_PIN, 1);
  else           digitalWrite(LED_PIN, 0);
  ledTime += deltaTime;
  if(ledTime > interval) ledTime = -interval;
}

void BeepBuzzer() {
  //TODO: Add buzzer logic!
}

void CheckButton() {
  buttonTime -= deltaTime;
  if(buttonTime <= 0) {
    buttonHoldTime = 0;
    if(buttonTime < -BUTTON_TIME_WINDOW) Stop();
    else {
      BlinkLed(LED_INTERVAL);
    }
  }

  if(digitalRead(BUTTON_PIN)) {
    buttonHoldTime += deltaTime;
    if(buttonHoldTime < BUTTON_HOLD_LIMIT) {
      buttonTime = BUTTON_INTERVAL;
      digitalWrite(LED_PIN, 0);
      ledTime = 0;
    }
    else Stop();
  }
}