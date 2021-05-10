#include <TinyGsmClient.h>
#include <FirebaseESP32.h>
#include <TinyGPS++.h>

//sample NMEA stream for reference of tracker
const char *gpsStream =
  "$GPRMC,045103.000,A,3014.1984,N,09749.2872,W,0.67,161.46,030913,,,A*7C\r\n"
  "$GPGGA,045104.000,3014.1985,N,09749.2873,W,1,09,1.2,211.6,M,-22.5,M,,0000*62\r\n"
  "$GPRMC,045200.000,A,3014.3820,N,09748.9514,W,36.88,65.02,030913,,,A*77\r\n"
  "$GPGGA,045201.000,3014.3864,N,09748.9411,W,1,10,1.2,200.8,M,-22.5,M,,0000*6C\r\n"
  "$GPRMC,045251.000,A,3014.4275,N,09749.0626,W,0.51,217.94,030913,,,A*7D\r\n"
  "$GPGGA,045252.000,3014.4273,N,09749.0628,W,1,09,1.3,206.9,M,-22.5,M,,0000*6F\r\n";

#define FIREBASE_HOST "tracker-20e05-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "LQU4HbWJMIEtzjMKVC9cSaUKe4j4KS9F7EUgPMgQ"

#define TINY_GSM_MODEM_SIM800      // Modem is SIM800
#define TINY_GSM_RX_BUFFER   1024  // Set RX buffer to 1Kb

//TTGO T-Call Modem Pins (Used for mobile network connection)
//We need to use sims that support 2G network
//Serial for Modem and It's AT Commands is Serial1
#define MODEM_RST            5
#define MODEM_PWKEY          4
#define MODEM_POWER_ON       23
#define MODEM_TX             27
#define MODEM_RX             26

//Declaring objects from header file for tracker database and gps
FirebaseData firebaseData;
TinyGPSPlus gps;

const char apn[]      = ""; // APN (example: internet.vodafone.pt) use https://wiki.apnchanger.org
const char gprsUser[] = ""; // GPRS User
const char gprsPass[] = ""; // GPRS Password

// SIM card PIN (leave empty, if not defined)
const char simPIN[]   = "";


float value1;
float value2;

void setup() {
  Serial.begin(115200);

  // Set modem reset, enable, power pins
  pinMode(MODEM_PWKEY, OUTPUT);
  pinMode(MODEM_RST, OUTPUT);
  pinMode(MODEM_POWER_ON, OUTPUT);
  digitalWrite(MODEM_PWKEY, LOW);
  digitalWrite(MODEM_RST, HIGH);
  digitalWrite(MODEM_POWER_ON, HIGH);

  // Set GSM module baud rate and UART pins
  Serial1.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(3000);

  Serial.println("Initializing modem...");
  modem.restart();
  // use modem.init() -> if you don't need the complete restart

  // Unlock your SIM card with a PIN if needed
  if (strlen(simPIN) && modem.getSimStatus() != 3 ) {
    modem.simUnlock(simPIN);
  }

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

}




void loop() {
  value1 = gps.location.lat();
  value2 = gps.location.lng();

  if (Firebase.setInt(firebaseData, "/Lattitude", value1)) {
    //Serial.println(counter);
  }

  if (Firebase.setFloat(firebaseData, "/Longitude", value2)) {

  }


  if (Firebase.setString(firebaseData, "/TimeStamp", String(gps.time.value()))) {
  }


  if (Firebase.getInt(firebaseData, "/Lattitude")) {
    if (firebaseData.dataType() == "float") {
      Serial.print("data = ");
      Serial.println(firebaseData.floatData());
    }
  }

  if (Firebase.getFloat(firebaseData, "/Longitude")) {
    if (firebaseData.dataType() == "float") {
      Serial.print("data = ");
      Serial.println(firebaseData.floatData());
    }
  }

  if (Firebase.getString(firebaseData, "/Timestamp")) {
    if (firebaseData.dataType() == "String") {
      Serial.print("data = ");
      Serial.println(firebaseData.stringData());
    }
  }

}
