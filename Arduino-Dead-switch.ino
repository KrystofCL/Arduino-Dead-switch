void(* Stop) (void) = 0; //Function at address 0, used to restart the Arduino

#pragma region Variables
/*Used pins*/
const int LED_PIN = 3;
const int BUTTON_PIN = 10;
const int RELAY_PIN = 2;
const int BUZZER_PIN = 6;

bool running;
bool ready;

int lastTime;
int deltaTime;

/*ALL TIMES ARE IN MILLISECONDS*/
int buttonTime;
const int BUTTON_INTERVAL = 2000; //How often is check-in required
const int BUTTON_TIME_WINDOW = 5000; //How much time is for pressing the button
int buttonHoldTime;
const int BUTTON_HOLD_LIMIT = 500; //How long are you allowed to hold the button

int ledTime;
const int LED_INTERVAL = 500; //Half time of LED cycle

int buzzerTime;
const int BUZZER_INTERVAL = 500; //Half time of buzzer cycle
const int BUZZER_DELAY = 1000; //How long will the buzzer wait before joining the blinking LED

const int BUZZER_TONE = 10; //Tone set in Hz
#pragma endregion

void setup() {
  #pragma region Pinmodes
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  #pragma endregion
  digitalWrite(LED_PIN, 1);
}

void loop() {
  digitalWrite(RELAY_PIN,running);
  deltaTime = millis() - lastTime; //Detlat time calcilations
  lastTime = millis();             //

  if(running && ready)                        CheckButton();
  else if(!digitalRead(BUTTON_PIN) && !ready) ready = true;
  else if( digitalRead(BUTTON_PIN) &&  ready) running = true;
  delay(10);
}

void BlinkLed(int interval) {
  if(ledTime > 0)digitalWrite(LED_PIN, 1);
  else           digitalWrite(LED_PIN, 0);
  ledTime += deltaTime;
  if(ledTime > interval) ledTime = -interval;
}

void BeepBuzzer(int interval, int _tone) {
  if(buzzerTime > 0) tone(BUZZER_PIN, _tone);
  else               noTone(BUZZER_PIN);
  buzzerTime += deltaTime;
  if(buzzerTime > interval) buzzerTime = -interval;
}

void CheckButton() {
  buttonTime -= deltaTime;
  if(buttonTime <= 0) {
    buttonHoldTime = 0;
    if(buttonTime < -BUTTON_TIME_WINDOW) Stop();
    else {
      BlinkLed(LED_INTERVAL);
      if(buttonTime <= -BUZZER_DELAY) BeepBuzzer(BUZZER_INTERVAL, BUZZER_TONE);
    }
  }

  if(digitalRead(BUTTON_PIN)) {
    buttonHoldTime += deltaTime;
    if(buttonHoldTime < BUTTON_HOLD_LIMIT) {
      buttonTime = BUTTON_INTERVAL;
      digitalWrite(LED_PIN, 0);
      noTone(BUZZER_PIN);
      ledTime = 0;
      buzzerTime = 0;
    }
    else Stop();
  }
}