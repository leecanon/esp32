#include "DHT.h"
#include <SPI.h>
#include <FirebaseESP32.h>

#define FIREBASE_HOST "https://*****-****-*****-******-****.*******.***"
#define FIREBASE_AUTH "******************************"
#define WIFI_SSID "********" // Thay đổi tên wifi của bạn
#define WIFI_PASSWORD "*******" // Thay đổi password wifi của bạn
// Digital pin connected to the DHT sensor

FirebaseData fbdo;

const int sensorPin = 35;

  #define DHTPIN 4  

// Uncomment whatever DHT sensor type you're using
  #define DHTTYPE DHT11   // DHT 11
 

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);
// Variables to hold sensor readings
float temp;
float hum;
unsigned long previousMillis = 0;   // Stores last time temperature was published
const long interval = 10000;        // Interval at which to publish sensor readings

void setup() {
  
  Serial.begin(115200);
  WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Dang ");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  dht.begin();

  Serial.println ("");
  Serial.println ("Da ket noi WiFi!");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
}
}
void loop() {

  float hum = dht.readHumidity();
  float temp = dht.readTemperature();  // Đọc nhiệt độ theo độ C

  if (isnan(hum) || isnan(temp)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  } 
  
  int humedad = analogRead(sensorPin);
  humedad = map(humedad,0, 4095, 120, 0);

  if (isnan(humedad)) {
    Serial.println("Failed to read from SPI sensor!");
    return;
  }
  //(, 0, 1023, 100,0)
  unsigned long currentMillis = millis();
  // Every X number of seconds (interval = 10 seconds) 
  // it publishes a new MQTT message
  if (currentMillis - previousMillis >= interval) {
    // Save the last time a new reading was published
    previousMillis = currentMillis;
    // New DHT sensor readings
    hum = dht.readHumidity();
    // Read temperature as Celsius (the default)
    temp = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    //temp = dht.readTemperature(true);
    // Check if any reads failed and exit early (to try again).

      Serial.print("la temp es: ");
      Serial.print(temp);
      Serial.println();
      Serial.print("la hum es: ");
      Serial.print(hum);
      Serial.println();
      Serial.print("la humedad de suelo es ");
      Serial.print(humedad);
      Serial.print("%");
      Serial.println("");

      pinMode(sensorPin,INPUT);
//      pinMode(rele, OUTPUT );
//      float en = sensorPin;
//      float of = sensorPin;
      
//      if (en >= 0){
//    digitalWrite (rele,HIGH);
//    Serial.println("Encendido");
//   delay(10000);
//  }
//  if(of < 500){
//    digitalWrite (rele,LOW);
//    Serial.println("Apagado");
//    delay(1000);
//  }    
    // Publish an MQTT message on topic esp32/dht/temperature                       
    /*  Serial.print("Nhiet do: ");
      Serial.print(temp);
      Serial.print("*C  ");
      Serial.print("Do am: ");
      Serial.print(hum);
      Serial.println("%  ");
      Serial.print(humedad);
      Serial.println("   ");  */
    
      Firebase.setFloat( fbdo,"Temperatura", temp);
      Firebase.setFloat ( fbdo,"Humedad-Amb", hum);
      Firebase.setFloat ( fbdo,"Humedad-Sue", humedad);
      
      delay( 1000 );
  }
  
}
  


    // Publish an MQTT message on topic esp32/dht/humidity
