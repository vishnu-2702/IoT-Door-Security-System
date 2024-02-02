#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_FIRMWARE_VERSION "" 

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define DHTPIN D4
#define DHTTYPE DHT11
#define LEDPIN D7
#define DOORPIN D3
#define LCD_COLS 16
#define LCD_ROWS 2

char auth[] = ""; //blynk auth token

//wifi
char ssid[] = "";
char pass[] = "";

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);

// Function to connect to Wi-Fi network
void connectToWifi() {
  // set Wi-Fi mode to station mode
  WiFi.mode(WIFI_STA); 
  // do not store Wi-Fi settings in flash memory
  WiFi.persistent(false); 
  WiFi.begin(ssid, pass);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting...");
  Serial.print("Connecting to Wi-Fi...");

  int tries = 0;
  while (WiFi.status() != WL_CONNECTED && tries < 10) {
    delay(1000);
    lcd.setCursor(0, 1);
    lcd.print("Attempt ");
    lcd.print(tries + 1);
    Serial.print(".");
    tries++;
  }

  if (WiFi.status() != WL_CONNECTED) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Failed to connect");
    Serial.println("Failed to connect to Wi-Fi");
    ESP.restart();
  }

  Serial.println("Connected to Wi-Fi");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connected");
}

// Function to connect to Blynk server
void connectToBlynk() {
  // configure Blynk without Wi-Fi credentials
  Blynk.config(auth); 
  Blynk.connect(5000);

  int tries = 0;
  while (!Blynk.connected() && tries < 10) {
    delay(1000);
    Serial.print(".");
    tries++;
  }

  if (!Blynk.connected()) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Failed to connect");
    Serial.println("Failed to connect to Blynk");
    ESP.restart();
  }

  Serial.println("Connected to Blynk");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connected");
}

void setup() {
  Serial.begin(115200);
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Setting up");

  connectToWifi();
  connectToBlynk();

  dht.begin();
  pinMode(LEDPIN, OUTPUT);
  pinMode(DOORPIN, INPUT_PULLUP);
}

void loop() {

    // Check Wi-Fi connection
  if (WiFi.status() != WL_CONNECTED) {
    connectToWifi();
    connectToBlynk();
  }

  // Check Blynk connection
  if (!Blynk.connected()) {
    connectToBlynk();
  }

  Blynk.run(); 
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); 

  Blynk.virtualWrite(V1, temperature); 
  Blynk.virtualWrite(V2, humidity); 
  int doorStatus = digitalRead(DOORPIN); 

  if (doorStatus == LOW) { 

    digitalWrite(LEDPIN, HIGH); 
    lcd.clear(); 
    lcd.setCursor(0, 0);
    lcd.print("Door is closed!"); 
    lcd.setCursor(0,1);
    lcd.print("T:");
    lcd.print(temperature);
    lcd.print(" H:");
    lcd.print(humidity);
    Blynk.virtualWrite(V3, "Door is closed!"); 
  } else { // Door is closed

    digitalWrite(LEDPIN, LOW); 
    lcd.clear(); 
    lcd.setCursor(0, 0);
    lcd.print("Door is open!"); 
    lcd.setCursor(0,1);
    lcd.print("T:");
    lcd.print(temperature);
    lcd.print(" H:");
    lcd.print(humidity);
    Blynk.logEvent("door_open","Door Has Been Open");
    Blynk.virtualWrite(V3, "Door is open!"); 
  }
    // Potential fire detected
    if (temperature > 50) { 
      Blynk.logEvent("fire_alert", "High temperature detected");
    }
 
  delay(2000); 
}