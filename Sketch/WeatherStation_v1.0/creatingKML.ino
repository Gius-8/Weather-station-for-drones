void writeKmlFile() {
  if (CreateFile) {
    if (myFile) {
      if (!WriteEncoding) {
        Serial.print("Write on: ");
        Serial.print(Combine);
        Serial.println("...");
        Encoding(); Serial.print("Write Encoding...");
        style();    Serial.print("Write style...");
        WriteEncoding = true;
      }

      if (!WriteFolder && WriteEncoding) {
        folder();
        WriteFolder = true;
        Serial.println("Write Folder...");
      }

      if (WriteEncoding && WriteFolder) {
        if (changeLocation) {
          Placemark_Point(lng, lat, ALTITUDE, t, h, P, p0, speed);
          Serial.print("Write in pos: ");
          Serial.println(i);
          i++;
          changeLocation = false;
        }
      }
    }
    else {
      // if the file didn't open, print an error:
      Serial.print("error opening ");
      Serial.print(Combine);
      Serial.println(" :( ...");
      digitalWrite(ledError, HIGH);
      while (1);
    }
  }
  else {
    CreatingFileKML(); //Crea il file KML
  }
}

void CreatingFileKML() {
  Combine = file + num + ".kml";
  while (SD.exists(Combine)) {
    Combine = "";
    num++;
    Combine = file + num + ".kml";
    delay(1);
  }
  delay(500);
  myFile = SD.open(Combine, FILE_WRITE);
  Serial.print("Creato il file: ");
  Serial.println(Combine);
  CreateFile = true;
}

void Encoding() {
  myFile.println("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
  myFile.println("<kml xmlns=\"http://www.opengis.net/kml/2.2\" xmlns:gx=\"http://www.google.com/kml/ext/2.2\" xmlns:kml=\"http://www.opengis.net/kml/2.2\" xmlns:atom=\"http://www.w3.org/2005/Atom\">");
  myFile.println("<Document>");

  myFile.flush();
}

void style() {
  myFile.println("<name>Luoghi temporanei.kml</name>");
  myFile.println("<StyleMap id=\"m_ylw-pushpin\">");
  myFile.println("<Pair>");
  myFile.println("<key>normal</key>");
  myFile.println("<styleUrl>#s_ylw-pushpin</styleUrl>");
  myFile.println("</Pair>");
  myFile.println("<Pair>");
  myFile.println("<key>highlight</key>");
  myFile.println("<styleUrl>#s_ylw-pushpin_hl</styleUrl>");
  myFile.println("</Pair>");
  myFile.println("</StyleMap>");
  myFile.println("<Style id=\"s_ylw-pushpin\">");
  myFile.println("<IconStyle>");
  myFile.println("<scale>1.1</scale>");
  myFile.println("<Icon>");
  myFile.println("<href>http://maps.google.com/mapfiles/kml/pushpin/ylw-pushpin.png</href>");
  myFile.println("</Icon>");
  myFile.println("<hotSpot x=\"20\" y=\"2\" xunits=\"pixels\" yunits=\"pixels\"/>");
  myFile.println("</IconStyle>");
  myFile.println("<LineStyle>");
  myFile.println("<color>ff1b1bff</color>");
  myFile.println("<width>2</width>");
  myFile.println("</LineStyle>");
  myFile.println("</Style>");

  myFile.flush();
}

void folder() {
  myFile.println("<Folder>");
  myFile.println("<name>Misurazioni</name>");
  myFile.println("<open>1</open>");

  myFile.flush();
}

void Placemark_Point(double longitude, double latitude, double Altit, float temp, float hum, double Press, double PressSea, double speedGPS) {
  myFile.println("<Placemark>");
  myFile.print("<name>Point ");
  myFile.print(i);
  myFile.println("</name>");
  myFile.print("<description>Temperature: ");
  myFile.print(temp); myFile.print(" °C");
  myFile.print("\nHumidity: ");
  myFile.print(hum); myFile.print("%");
  myFile.print("\nAbsolute pressure: ");
  myFile.print(Press, 2);
  myFile.print(" mb, ");
  myFile.print(Press * 0.0295333727, 2);
  myFile.print(" inHg");
  myFile.print("\nRelative pressure: ");
  myFile.print(PressSea, 2);
  myFile.print(" mb, ");
  myFile.print(PressSea * 0.0295333727, 2);
  myFile.print(" inHg");
  myFile.print("\nSpeed: ");
  myFile.print(speedGPS);
  myFile.print(" m/s");
  myFile.println("</description>");
  myFile.println("<LookAt>");
  myFile.print("<gx:TimeStamp><when>");
  myFile.print(year);
  myFile.print("-");
  if (month < 10) {
    myFile.print("0");
    myFile.print(month);
  }
  else
    myFile.print(month);
  myFile.print("-");
  if (day < 10) {
    myFile.print("0");
    myFile.print(day);
  }
  else
    myFile.print(day);
  myFile.print("T");
  if (hour < 10) {
    myFile.print("0");
    myFile.print(hour);
  }
  else
    myFile.print(hour);
  myFile.print(":");
  if (minute < 10) {
    myFile.print("0");
    myFile.print(minute);
  }
  else
    myFile.print(minute);
  myFile.print(":");
  if (second < 10) {
    myFile.print("0");
    myFile.print(second);
  }
  else
    myFile.print(second);
  myFile.print("Z");
  myFile.println("</when></gx:TimeStamp>");
  myFile.print("<longitude>");
  myFile.print(longitude, 6);
  myFile.println("</longitude>");
  myFile.print("<latitude>");
  myFile.print(latitude, 6);
  myFile.println("</latitude>");
  myFile.println("<altitude>5.926978577421624</altitude>");
  myFile.println("<range>209.9993794574608</range>");
  myFile.println("<gx:altitudeMode>relativeToSeaFloor</gx:altitudeMode>");
  myFile.println("</LookAt>");
  myFile.println("<styleUrl>#m_ylw-pushpin</styleUrl>");
  myFile.println("<Point>");
  myFile.println("<extrude>1</extrude>");
  myFile.println("<altitudeMode>absolute</altitudeMode>");
  myFile.println("<gx:drawOrder>1</gx:drawOrder>");
  myFile.print("<coordinates>");
  myFile.print(longitude, 6); myFile.print(",");
  myFile.print(latitude, 6); myFile.print(",");
  myFile.print(Altit);
  myFile.println("</coordinates>");
  myFile.println("</Point>");
  myFile.println("</Placemark>");

  myFile.flush();
}

void close_myFile() {
  myFile.println("</Folder>");
  myFile.println("</Document>");
  myFile.println("</kml>");
  myFile.close();
}

