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

String dateString(){
  DateString = "";
  
  if (day < 10) {
    DateString = "0";
    DateString += day;
  }
  else
    DateString = day;
    
  DateString += "-";
  if (month < 10) {
    DateString += "0";
    DateString += month;
  }
  else
    DateString += month;
    
  DateString += "-";
  DateString += year;
    
  DateString += " ";
  if (hour < 10) {
    DateString += "0";
    DateString += hour;
  }
  else
    DateString += hour;

  DateString += ":";
  if (minute < 10) {
    DateString += "0";
    DateString += minute;
  }
  else
    DateString += minute;
  DateString += ":";
  if (second < 10) {
   DateString += "0";
    DateString += second;
  }
  else
    DateString += second;

  return DateString;
}

