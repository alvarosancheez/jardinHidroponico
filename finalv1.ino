#include <math.h>
#include "WiFi.h"
//#include "ESPAsyncWebServer.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <ThingsBoard.h>
#define LIGHT_SENSOR A0//Grove - Light Sensor is connected to A0 of Arduino
#define THINGSBOARD_SERVER "iot.etsisi.upm.es"
#define TOKEN "4S3cHWz3qVkR9fc6st3y"
#define uS_TO_S_FACTOR 1000000
#define TIME_TO_SLEEP 5
WiFiClient espClient;
ThingsBoard tb(espClient);
               //Connect the LED Grove module to Pin12, Digital 12
const int thresholdvalue=1;         //The treshold for which the LED should turn on. Setting it lower will make it go on at more light, higher for more darkness
float Rsensor; 

const char* ssid = "vodafone4A78";
const char* password = "8VA9BHYX39SEQW";

#define DHTPIN 32    
#define DHTTYPE    DHT22     // DHT 22 (AM2302)


DHT dht(DHTPIN, DHTTYPE);


void readDHT() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  Serial.println(t);
  if (isnan(t) || isnan(h) ) {    
    Serial.println("Failed to read from DHT sensor!");
  }
  else {
    Serial.println(t);
    Serial.println(h);
    tb.sendTelemetryFloat("Temperatura",t);
    tb.sendTelemetryFloat("Humedad",h);
  }
  
  delay(500);
}

void SensorDeAgua()
{
    
    Serial.println(analogRead(34));
    tb.sendTelemetryFloat("Agua",analogRead(34));
    delay(100);
}

void readLuminosidad(){
  int luz = analogRead(35)/9;
  Serial.println(luz);
  tb.sendTelemetryFloat("Luminosidad",luz);
  
  delay(500);
}


void setup() 
{
                                         //Start the Serial connection
  //  pinMode(ledPin,OUTPUT);            //Set the LED on Digital 12 as an OUTPUT
   // Serial.begin(9600);
   Serial.begin(115200);
  dht.begin();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println(WiFi.localIP());
 
}




void loop() {
   // esp_sleep_enable_timer_wakeup(uS_TO_S_FACTOR*TIME_TO_SLEEP);
   // Serial.println("ESP SE DESPIERTA");
  // reads the input on analog pin A0 (value between 0 and 1023)
      if (!tb.connected()) {
  // Connect to the ThingsBoard
      Serial.print("Connecting to: ");
      Serial.print(THINGSBOARD_SERVER);
      Serial.print(" with token ");
      Serial.println(TOKEN);
      if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
            Serial.println("Failed to connect");
            return;
          }
  }

  //Leemos del sensor de Temperatura y Humedad
  readDHT();

  //Leemos del sensor de 
  readLuminosidad();
  SensorDeAgua();
  //Serial.println("ESP SE DUERME");
  //esp_deep_sleep_start();
  
  
  
}
