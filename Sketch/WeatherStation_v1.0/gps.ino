void getGPSdata() {

  if (gps.location.isValid()) {
    lat = gps.location.lat(), 6; // Latitude in degrees (double)
    lng = gps.location.lng(), 6; // Longitude in degrees (double)
    GPSstatus =  true;
  }
  else
    GPSstatus =  false;

  if (gps.date.isValid()) {
    year = gps.date.year(); // Year (2000+) (u16)
    month = gps.date.month(); // Month (1-12) (u8)
    day = gps.date.day(); // Day (1-31) (u8)
  }
  else
    GPSstatus =  false;

  if (gps.time.isValid()) {
    hour = gps.time.hour(); // Hour (0-23) (u8)
    minute = gps.time.minute(); // Minute (0-59) (u8)
    second = gps.time.second(); // Second (0-59) (u8)
  }
  else
    GPSstatus =  false;

  if (gps.speed.isValid()) {
    speed = gps.speed.mps(); // Speed in meters per second (double)
  }
  else
    GPSstatus =  false;

  if (gps.altitude.isValid()) {
    ALTITUDE = gps.altitude.meters(); // Altitude in meters (double)
  }
  else
    GPSstatus =  false;

  if (lat - lastLAT >= 0.00002 || lat - lastLAT <= -0.00002 || lng - lastLNG >= 0.00002 || lng - lastLNG <= -0.00002 && !changeLocation) {
    lastLAT = lat;
    lastLNG = lng;
    changeLocation = true;
  }
}
