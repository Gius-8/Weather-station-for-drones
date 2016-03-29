void init_WifiAP() {
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    digitalWrite(ledError, HIGH);
    while (true);       // don't continue
  }

  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  Serial.print("Creating Network named: ");
  Serial.println(ssidAP);                   // print the network name (SSID);
  status = WiFi.beginAP(ssidAP);
  // wait 10 seconds for connection:
  delay(10000);
  server.begin();                           // start the web server on port 80
  printWifiStatus();                        // you're connected now, so print out the status
}

void wifiLoop() {
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<center><h2 style=\"background-color:lightblue\">Weather Station for Drones</h2>");
          // output the value of each analog input pin
          client.print("Temperature: <b>");
          client.print(t); client.print(" <sup>o</sup>C</b>");
          client.println("<br>");
          client.print("\nHumidity: <b>");
          client.print(h); client.print("%</b>");
          client.println("<br>");
          client.print("\nAbsolute pressure: <b>");
          client.print(P, 2);
          client.print(" mb,  ");
          client.print(P * 0.0295333727, 2);
          client.print(" inHg</b>");
          client.println("<br>");
          client.print("\nRelative pressure: <b>");
          client.print(p0, 2);
          client.print(" mb,  ");
          client.print(p0 * 0.0295333727, 2);
          client.print(" inHg</b>");
          client.println("<br>");
          client.print("Speed: <b>");
          client.print(speed);
          client.print(" m/s");
          client.println("</b><br>");
          client.print("Altitude: <b>");
          client.print(ALTITUDE);
          client.println("</b><br>");
          client.print("Longitude: <b>");
          client.print(lng, 6);
          client.println("</b><br>");
          client.print("Latitude: <b>");
          client.print(lat, 6);
          client.println("</b><br>");
          client.println("<h5 style=\"background-color:lightgrey\">Powered by Giuseppe Caccavale<br>Site: www.giuseppecaccavale.it<br>Email: info@giuseppecaccavale.it</h5>");
          client.println("</center></html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);

    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

