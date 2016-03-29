// Function button debouncs
int debounce(const byte buttonPin) {
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == HIGH) {
        state = !state;
      }
    }
  }

  lastButtonState = reading;
  return state;
}

void reset_Variables() {
  CreateFile = false;
  WriteEncoding = false;
  WriteFolder = false;
  i = 0;
}

