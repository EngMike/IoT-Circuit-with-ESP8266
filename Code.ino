#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "DHT.h"
#define DHTPIN D3        // What digital pin we're connected to select yours accordingly
#define DHTTYPE DHT11     // DHT 11
DHT dht(DHTPIN, DHTTYPE);
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display

// Replace with your network credentials
char auth[] = "token";
char ssid[] = "wifi id";
char pass[] = "wifi password";

// Virtual pin assignments for relays
#define RELAY_1_PIN V7
#define RELAY_2_PIN V8

// Relay pin assignments
const int RELAY_1 = D5;
const int RELAY_2 = D6;

void setup() {
  
  lcd.init();
  lcd.clear();         
  lcd.backlight();      
  Serial.begin(9600);

  // Connect to Wi-Fi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Connecting to WiFi...");
  }

  // Initialize Blynk
  Blynk.begin(auth, ssid, pass);

  // Initialize DHT sensor
  dht.begin();

   // Set relay pins as output
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);

  // Turn off relays initially
  digitalWrite(RELAY_1, HIGH);
  digitalWrite(RELAY_2, HIGH);

  // Register virtual button callbacks
  Blynk.virtualWrite(RELAY_1_PIN, HIGH);
  Blynk.virtualWrite(RELAY_2_PIN, HIGH);
  Blynk.virtualWrite(RELAY_1_PIN, LOW);
  Blynk.virtualWrite(RELAY_2_PIN, LOW);
  Blynk.virtualWrite(RELAY_1_PIN, HIGH);
  Blynk.virtualWrite(RELAY_2_PIN, HIGH);
  Blynk.virtualWrite(RELAY_1_PIN, LOW);
  Blynk.virtualWrite(RELAY_2_PIN, LOW);

  // Sync Blynk button states
  Blynk.syncVirtual(RELAY_1_PIN);
  Blynk.syncVirtual(RELAY_2_PIN); 

}

// Relays commands on Blynk
BLYNK_WRITE(RELAY_1_PIN)
{
  int relayState = param.asInt();

  // Control Relay 1 based on virtual button state
  if (relayState == HIGH)
  {
    digitalWrite(RELAY_1, LOW);
    Serial.println("Relay 1 OFF");
  }
  else
  {
    digitalWrite(RELAY_1, HIGH);
    Serial.println("Relay 1 ON");
  }
}

BLYNK_WRITE(RELAY_2_PIN)
{
  int relayState = param.asInt();

  // Control Relay 2 based on virtual button state
  if (relayState == HIGH)
  {
    digitalWrite(RELAY_2, LOW);
    Serial.println("Relay 2 OFF");
  }
  else
  {
    digitalWrite(RELAY_2, HIGH);
    Serial.println("Relay 2 ON");
  }
}

void loop() {

  // LCD Readings
  delay(1000); // wait a few seconds between measurements

  float humi  = dht.readHumidity();    // read humidity
  float tempC = dht.readTemperature(); // read temperature

  lcd.clear();
  // check if any reads failed
  if (isnan(humi) || isnan(tempC)) {
    lcd.setCursor(0, 0);
    lcd.print("Failed");
  } else {
    lcd.setCursor(0, 0);  // start to print at the first row
    lcd.print("Temp: ");
    lcd.print(tempC);     // print the temperature
    lcd.print((char)223); // print ° character
    lcd.print("C");

    lcd.setCursor(0, 1);  // start to print at the second row
    lcd.print("Umid: ");
    lcd.print(humi);      // print the humidity
    lcd.print("%");
  }

  // Blynk Readings
  Blynk.run();
  readDHTSensor();
}

void readDHTSensor()
{
  // Read temperature and humidity from DHT sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Display sensor data on Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("°C\tHumidity: ");
  Serial.print(humidity);
  Serial.println("%");

  // Send sensor data to Blynk app
  Blynk.virtualWrite(V6, temperature);
  Blynk.virtualWrite(V5, humidity);

}
