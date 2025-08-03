#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED Setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// DHT11 Setup
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// WiFi Credentials
#define WIFI_SSID "SpectrumSetup-76E2"
#define WIFI_PASSWORD "internalfuel987"

// Firebase Config
#define FIREBASE_HOST "esp32-cloud-iot-sensor-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "AIzaSyBYrSqo192mwmfkqA579TEP86CKRYKrcYw" // API Key

unsigned long sendDataPrevMillis = 0;
const long sendInterval = 60000; // 15 seconds

void updateDisplay(float temp, float hum, const char* status = nullptr) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Temp: ");
  display.print(temp, 1);
  display.println(" C");
  display.print("Hum:  ");
  display.print(hum, 1);
  display.println(" %");
  if (status) {
    display.setCursor(0,40);
    display.println(status);
  }
  display.display();
}

void sendToFirebase(float temp, float hum) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected!");
    return;
  }

  HTTPClient http;
  String url = "https://" + String(FIREBASE_HOST) + "/sensor.json?auth=" + String(FIREBASE_AUTH);
  
  String payload = "{\"temperature\":" + String(temp) + 
                   ",\"humidity\":" + String(hum) + 
                   ",\"timestamp\":{\".sv\":\"timestamp\"}}";

  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  
  int httpCode = http.PUT(payload);
  
  if (httpCode == HTTP_CODE_OK) {
    Serial.println("Data sent successfully");
    updateDisplay(temp, hum, "Sent OK");
  } else {
    Serial.printf("Error: %s\n", http.errorToString(httpCode).c_str());
    updateDisplay(temp, hum, "Send Failed");
  }
  
  http.end();
}

void setup() {
  Serial.begin(115200);
  
  // Initialize OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED init failed!");
    while(1);
  }
  display.display();
  delay(500);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Initializing...");
  display.display();
  
  // Initialize DHT
  dht.begin();
  
  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected with IP: " + WiFi.localIP().toString());
  updateDisplay(0, 0, "System Ready");
}

void loop() {
  if (millis() - sendDataPrevMillis > sendInterval) {
    sendDataPrevMillis = millis();
    
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Sensor read failed!");
      updateDisplay(0, 0, "Sensor Error!");
      return;
    }

    updateDisplay(temperature, humidity, "Sending...");
    sendToFirebase(temperature, humidity);
  }
}