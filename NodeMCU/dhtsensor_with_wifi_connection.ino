#include <DHT.h>  // DHT Sensors Library
#include <DHT_U.h>

#include<ESP8266WiFi.h> // Library for ESP8266 WiFi


#define dhtpin 5
#define sensortype DHT11

DHT dhtsen(dhtpin,sensortype);

const char *ssid="Write Your WiFi Username";
const char *pwd="Write Your WiFi Password";

void setup() {
  Serial.begin(9600);
  Serial.println("WiFi Connection with "+String(ssid)); // WiFi Connection with ACTFIBERNET
  WiFi.begin(ssid,pwd); // WiFi connection with SSID and Password
  while(WiFi.status()!=WL_CONNECTED) // WiFi Status Checking 
  {
    Serial.print("****");
    delay(300);
  }
  Serial.print("\n WiFi Connected with the IP address:"); // WiFi Connected and display the IP address
  Serial.println(WiFi.localIP());
  dhtsen.begin();

}

void loop() {
  float temp,hum;

  temp=dhtsen.readTemperature();
  hum=dhtsen.readHumidity();

  Serial.println("Temperature: "+String(temp));
  Serial.println("Humidity: "+String(hum));

  delay(2000);
  

}
