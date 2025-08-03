ESP32 Firebase IoT Sensor System - Program Description
This program enables an ESP32 microcontroller to:

Read temperature & humidity from a DHT11 sensor

Display data on an OLED screen (SSD1306)

Send sensor data to Firebase Realtime Database securely over Wi-Fi

📌 Key Features
✔ Wi-Fi Connectivity – Connects to a local network (SpectrumSetup-76E2 in this example)
✔ DHT11 Sensor Integration – Measures temperature (°C) and humidity (%)
✔ OLED Display – Shows real-time sensor data and system status
✔ Firebase Realtime Database – Stores sensor data with timestamps
✔ Error Handling – Detects sensor failures and network issues

⚙️ Hardware Components
ESP32 (Wi-Fi & Bluetooth-enabled microcontroller)

DHT11 (Temperature & Humidity Sensor)

SSD1306 OLED Display (128x64 resolution, I2C interface)

Breadboard & Jumper Wires (For connections)

📡 Data Flow
ESP32 reads sensor data (every 60 seconds by default)

Data is displayed on OLED (Temperature, Humidity, Status)

Data is sent to Firebase via HTTPS (PUT request)

Firebase stores data in JSON format:

json
{
  "temperature": 25.5,
  "humidity": 60.2,
  "timestamp": { ".sv": "timestamp" }  // Auto-generated server timestamp
}
🔧 Configuration
1. Wi-Fi Settings
cpp
#define WIFI_SSID "Your_WiFi_SSID"  
#define WIFI_PASSWORD "Your_WiFi_Password"  
2. Firebase Setup
cpp
#define FIREBASE_HOST "your-project-id.firebaseio.com"  
#define FIREBASE_AUTH "Your_Firebase_API_Key"  
3. Sensor & Display Pins
DHT11 → GPIO 4

OLED (SSD1306) → I2C (Default SDA=21, SCL=22 on ESP32)

⚠️ Important Notes
Security Considerations
🔒 Exposed API Key: The Firebase API key is hardcoded (not secure for production).
✅ Recommended Fix: Use Firebase Authentication or Environment Variables (Secure Wi-Fi credentials too).

Firebase Free Tier Limits
📊 1 GB Storage & 10 GB/month Bandwidth → Enough for small-scale IoT monitoring.
⏳ Data Retention: Old entries should be cleaned up (via Cloud Functions).

🚀 Possible Improvements
Deep Sleep Mode (Save battery if running on portable power)

OTA Updates (Wireless firmware updates)

Multiple Sensors (Add CO₂, Pressure, etc.)

Firebase Alerts (Notify if temperature exceeds limits)

📂 Firebase Database Structure
text
/sensor_data  
   └── {auto-id}  
       ├── temperature: 25.5  
       ├── humidity: 60.2  
       └── timestamp: 1234567890  
🔗 Next Steps
Set up Firebase Realtime Database

Secure your Firebase API key

Extend with Firebase Cloud Functions
