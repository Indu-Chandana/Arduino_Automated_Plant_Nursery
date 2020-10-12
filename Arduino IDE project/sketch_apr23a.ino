#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <DHT.h>

#define TRIGGER_PIN  D5
#define ECHO_PIN     D3
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "114e3cc9183243d7ac49c82a70e57d23"; //Enter the Auth code which was send by Blink

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Dialog 4G New";  //Enter your WIFI Name
char pass[] = "budusaranai";  //Enter your WIFI Password

int led = D8;
#define DHTPIN 2          // Digital pin 4

// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

DHT dht(DHTPIN, DHTTYPE);
SimpleTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);  //V5 is for Humidity
  Blynk.virtualWrite(V6, t);  //V6 is for Temperature

   long duration, distance;
  digitalWrite(TRIGGER_PIN, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(TRIGGER_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration/2) / 29.1;
  Serial.print(distance);
  Serial.println(" cm");
  delay(200);

 Blynk.virtualWrite(V4 ,distance);
  delay(100);

   if (distance=='null') {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V4,distance);  //V5 is for Humidity
}

void setup()
{
  Serial.begin(9600); // See the connection status in Serial Monitor
  pinMode(D8, OUTPUT);
 

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUILTIN_LED, OUTPUT);

  Blynk.begin(auth, ssid, pass);
 
  dht.begin();

   // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
  delay(1000);

}
BLYNK_WRITE(V8)
{
 int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable

   if (pinValue == 1){
    digitalWrite(D8,HIGH);
   }else{
    digitalWrite(D8,LOW);
   }
}
void loop()
{
  Blynk.run(); // Initiates Blynk
  timer.run(); // Initiates SimpleTimer
}
