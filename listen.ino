#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>

// setup lcd display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// init the http and wifi objects
HTTPClient http;
WiFiClient wifi;

const char* ssid = "";
const char* password = "";

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

  // connect to sse
  http.setReuse(true);
  http.begin(wifi, "http://sce.sjsu.edu:8080/api/messages/listen");

  // wait for a message
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
        if (line.startsWith("event: error")) {
          lcd.clear();
          lcd.print("An error has");
          lcd.setCursor(0, 1);
          lcd.print("occurred.");
          flushWiFiBuffer();
        }
        if (line.startsWith("data: ")) {
          const char* currentMessage = line.substring(6).c_str();
          

          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Message: ")
          flushWiFiBuffer(); // flush the buffer so scrollMessage properly runs
          lcd.setCursor(0,1);
          scrollMessage(currentMessage);
        }
    }
  }
  else {
    // attempt to reconnect if the server crashes
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

void scrollMessage(const char* message) {
  int len = strlen(message);
  if (len <= 16) {
    lcd.setCursor(0, 1);
    lcd.print(message);
    delay(2000);
  } else {
    for (int i = 0; i < len - 15; i++) {
      // if new text is available, exit out of the function
      if (http.connected() && wifi.available()) {
        return;
      }

      lcd.setCursor(0, 1);
      lcd.print(message + i);
      delay(250); 
    }
    delay(2000);
  }
}

void flushWiFiBuffer() {
  while (wifi.available()) {
    wifi.read();
  }
}