#include <WiFi.h>
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
char* WIFI_SSID = "Arvin"; //Replace with your wifi
char* WIFI_PASSWORD = "12345678"; //Replace with password

//Declaring objects from header file for tracker database and gps
FirebaseData firebaseData;
TinyGPSPlus gps;

void setup(){
  
  Initialization();
  WiFiConnection();
  
 }

float value1;
float value2;

void Initialization(){
  
  Serial.begin(115200); 
 }

void WiFiConnection(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void loop(){
  value1 = gps.location.lat();
  value2 = gps.location.lng();
  
  if(Firebase.setInt(firebaseData, "/Lattitude",value1)){
      //Serial.println(counter);
    }

  if(Firebase.setFloat(firebaseData, "/Longitude",value2)){
 
    }

    
  if(Firebase.setString(firebaseData, "/TimeStamp",String(gps.time.value()))){
    }


   if(Firebase.getInt(firebaseData, "/Lattitude")){
    if(firebaseData.dataType() == "float"){
      Serial.print("data = ");
      Serial.println(firebaseData.floatData());
    }
  }

   if(Firebase.getFloat(firebaseData, "/Longitude")){
    if(firebaseData.dataType() == "float"){
      Serial.print("data = ");
      Serial.println(firebaseData.floatData());
    }
  }

    if(Firebase.getString(firebaseData, "/Timestamp")){
    if(firebaseData.dataType() == "String"){
      Serial.print("data = ");
      Serial.println(firebaseData.stringData());
    }
  }
  
}
