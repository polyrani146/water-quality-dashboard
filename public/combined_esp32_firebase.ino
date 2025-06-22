#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Wi-Fi Credentials
const char* ssid = "Water Quality Monitor";
const char* password = "12345678";

// Firebase Credentials
#define API_KEY "AIzaSyBkaCYH1y0TXJW9n2UmeesGOrmy0N9zKOY"
#define DATABASE_URL "https://water-quality-monitor-cdf19-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define USER_EMAIL "mdsaiful000ms@gmail.com"
#define USER_PASSWORD "123456789"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// OLED Display Setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// DS18B20 Temp Sensor
const int oneWireBus = 4;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

// TDS Sensor
#define TdsPin 35
#define VREF 3.3
#define TDS_SAMPLES 30
int tdsBuffer[TDS_SAMPLES], tdsTemp[TDS_SAMPLES], tdsIndex = 0;
float tdsValue = 0, avgTdsVoltage = 0;

// pH Sensor
#define PhPin 34
#define PH_SAMPLES 10
float phValue = 0, avgPhVoltage = 0;
float temperature = 25.0;

// Turbidity Sensor
#define TURBIDITY_PIN 33
float turbidityValue = 0;

int getMedianNum(int bArray[], int len) {
  int bTab[len];
  for (int i = 0; i < len; i++) bTab[i] = bArray[i];
  for (int j = 0; j < len - 1; j++) {
    for (int i = 0; i < len - j - 1; i++) {
      if (bTab[i] > bTab[i + 1]) {
        int temp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = temp;
      }
    }
  }
  return (len & 1) ? bTab[len / 2] : (bTab[len / 2] + bTab[len / 2 - 1]) / 2;
}

void setup() {
  Serial.begin(115200);
  sensors.begin();
  analogReadResolution(12);

  pinMode(TdsPin, INPUT);
  pinMode(PhPin, INPUT);
  pinMode(TURBIDITY_PIN, INPUT);

  // OLED Init
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED failed");
    while (1);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Initializing...");
  display.display();
  delay(1000);

  // Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");

  // Firebase Config
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Serial.println("Firebase connected");
}

void loop() {
  // ----- Temperature -----
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  // ----- TDS Sampling -----
  static unsigned long tdsSampleTime = millis();
  if (millis() - tdsSampleTime > 40) {
    tdsSampleTime = millis();
    tdsBuffer[tdsIndex] = analogRead(TdsPin);
    tdsIndex = (tdsIndex + 1) % TDS_SAMPLES;
  }

  // ----- Every 800ms: calculate & display -----
  static unsigned long updateTime = millis();
  if (millis() - updateTime > 800) {
    updateTime = millis();

    // TDS
    for (int i = 0; i < TDS_SAMPLES; i++) tdsTemp[i] = tdsBuffer[i];
    int medianTDS = getMedianNum(tdsTemp, TDS_SAMPLES);
    avgTdsVoltage = medianTDS * VREF / 4095.0;
    float compCoeff = 1.0 + 0.02 * (temperature - 25.0);
    float compVoltage = avgTdsVoltage / compCoeff;
    tdsValue = (133.42 * pow(compVoltage, 3) - 255.86 * pow(compVoltage, 2) + 857.39 * compVoltage) * 0.5;

    Serial.print("TDS Value: ");
    Serial.print(tdsValue, 0);
    Serial.println(" ppm");

    // pH
    int phTotal = 0;
    for (int i = 0; i < PH_SAMPLES; i++) {
      phTotal += analogRead(PhPin);
      delay(20);
    }
    float avgPhADC = phTotal / float(PH_SAMPLES);
    avgPhVoltage = avgPhADC * VREF / 4095.0;
    phValue = 7 + ((avgPhVoltage - 2.6) / 0.1841);

    Serial.print("pH Value: ");
    Serial.println(phValue, 2);

    // Turbidity
    int turbidityADC = analogRead(TURBIDITY_PIN);
    float turbidityVolt = turbidityADC * (3.3 / 4095.0);
    turbidityValue = ((3.3 - turbidityVolt) * 100.0) - 100; // Adjust formula if needed

    Serial.print("Turbidity: ");
    Serial.print(turbidityValue, 1);
    Serial.println(" NTU");

    // ----- OLED Display -----
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Water Quality Monitor");
    display.drawLine(0, 10, SCREEN_WIDTH, 10, SSD1306_WHITE);

    display.setCursor(0, 16);
    display.print("Temperature: ");
    display.print(temperature, 1);
    display.println("C");

    display.setCursor(0, 26);
    display.print("TDS: ");
    display.print(tdsValue, 0);

    display.setCursor(0, 36);
    display.print("pH: ");
    display.print(phValue, 2);

    display.setCursor(0, 46);
    display.print("Turbidity: ");
    display.print(turbidityValue, 1);

    display.display();


    // ----- Firebase Upload -----
    Serial.println("Uploading to Firebase...");

    if (Firebase.RTDB.setFloat(&fbdo, "/water/temperature", temperature))
      Serial.println("Temp uploaded.");
    else Serial.println(fbdo.errorReason());

    if (Firebase.RTDB.setFloat(&fbdo, "/water/tds", tdsValue))
      Serial.println("TDS uploaded.");
    else Serial.println(fbdo.errorReason());

    if (Firebase.RTDB.setFloat(&fbdo, "/water/ph", phValue))
      Serial.println("pH uploaded.");
    else Serial.println(fbdo.errorReason());

    if (Firebase.RTDB.setFloat(&fbdo, "/water/turbidity", turbidityValue))
      Serial.println("Turbidity uploaded.");
    else Serial.println(fbdo.errorReason());

    Serial.println("------------------");
  }

  delay(100);
}
