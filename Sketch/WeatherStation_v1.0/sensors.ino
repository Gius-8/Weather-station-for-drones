//Functions DHT
void getDHTdata() {

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    digitalWrite(ledError, HIGH);
    while(true);
  }

  h = dht.readHumidity();
  t = dht.readTemperature();
}

void getBMP180data() {
  Status = pressure.startTemperature();
  if (Status != 0)
  {
    // Wait for the measurement to complete:
    delay(Status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Function returns 1 if successful, 0 if failure.

    Status = pressure.getTemperature(T);
    if (Status != 0)
    {
      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.

      Status = pressure.startPressure(3);
      if (Status != 0)
      {
        // Wait for the measurement to complete:
        delay(Status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        Status = pressure.getPressure(P, T);
        if (Status != 0)
        {
          // The pressure sensor returns abolute pressure, which varies with altitude.
          // To remove the effects of altitude, use the sealevel function and your current altitude.
          // This number is commonly used in weather reports.
          // Parameters: P = absolute pressure in mb, ALTITUDE = current altitude in m.
          // Result: p0 = sea-level compensated pressure in mb

          p0 = pressure.sealevel(P, ALTITUDE);
        }
        else Serial.println("BMP180: error retrieving pressure measurement\n");
      }
      else Serial.println("BMP180: error starting pressure measurement\n");
    }
    else Serial.println("BMP180: error retrieving temperature measurement\n");
  }
  else Serial.println("BMP180: error starting temperature measurement\n");
}

