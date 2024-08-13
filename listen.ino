#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

// setup lcd display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// init the http and wifi objects
HTTPClient http;
WiFiClientSecure wifi;

const char* ssid = "AAAA";
const char* password = "BBBB";
const char* url = "https://sce.sjsu.edu/api/messages/listen?id=XXXX&apiKey=YYYY";

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);

  // connect to wifi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    lcd.setCursor(0, 0);
    lcd.print("Connecting");
    lcd.setCursor(0, 1);
    lcd.print("to the WiFi...");
  }

  lcd.clear();
  lcd.print("Connected.");
  delay(1000);

  wifi.setInsecure();

  // Connect to the SSE endpoint
  http.setReuse(true);
  http.begin(wifi, url);
  http.addHeader("Connection", "keep-alive");

  // This part of the code waits for the first message to be typed in the room.
  int responseCode;
  while (responseCode < 0) {
    lcd.clear();
    lcd.print("Waiting for a");
    lcd.setCursor(0, 1);
    lcd.print("message....");
    responseCode = http.GET();
    delay(500);
  }
}

void loop() {
  if (http.connected()) {
    if (wifi.available()) {
      String line = wifi.readStringUntil('\n');
      if (line.startsWith("data: ")) {
        String jsonString = line.substring(6).c_str();

        JsonDocument doc;
        deserializeJson(doc, jsonString);

        const char* currentTime = doc["timestamp"];
        const char* currentMessage = doc["message"];

        // Logic to display the message
        lcd.clear();
        flushWiFiBuffer();  // flush the buffer so scrollMessage properly runs
<<<<<<< HEAD
        lcd.setCursor(0, 0);
        lcd.print(currentTime);
=======
        lcd.setCursor(0, 1);
>>>>>>> 54acdcaab0024fb30ca110649e65a13f0330d34a
        scrollMessage(1, currentMessage, 250, 16);
        lcd.setCursor(0, 1);
        lcd.print(currentMessage);
      }
    }
  } else {
    // Attempt to reconnect if the server crashes
    int responseCode = -1;
    while (responseCode < 0) {
      lcd.clear();
      lcd.print("Reconnecting");
      lcd.setCursor(0, 1);
      lcd.print("and waiting...");
      responseCode = http.GET();
      delay(500);
    }
  }



  delay(500);
}

// Function to scroll the message
void scrollMessage(int row, String message, int delayTime, int lcdColumns) {
  for (int i = 0; i < lcdColumns; i++) {
    message = " " + message;
  }
  message = message + " ";

  for (int pos = 0; pos < message.length(); pos++) {
    lcd.setCursor(0, row);
    lcd.print(message.substring(pos, pos + lcdColumns));
    delay(delayTime);
  }
}

// flush whatever is in the buffer so that the next messages that are supposed to display actually displays properly
void flushWiFiBuffer() {
  while (wifi.available()) {
    wifi.read();
  }
}