#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <LiquidCrystal_I2C.h>

const char* ssid = "";       
const char* password = ""; 
const char* sseUrl = "https://sce.sjsu.edu/api/listen";

LiquidCrystal_I2C lcd(0x27, 16, 2);  

void setup(){
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Connecting...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wifi...");
  }

  Serial.println("Connected to WiFi");
  lcd.clear();
  lcd.print("Connected");

  connectToSSE();
}

void connectToSSE() {
  WiFiClient client;
  HTTPClient http;
  http.begin(client, sseUrl);
  http.addHeader("Accept", "text/event-stream");

  int status = http.GET();

  if (status > 0) {
    Serial.printf("SSE connection status: %d\n", status);
    if (status == HTTP_CODE_OK) {
      WiFiClient* client = http.getStreamPtr();
      String line;

      while (client->connected()) {
        if (client->available()) {
          // if data is available
          char c = client->read();
          if (c == '\n' && line.length() > 0) {
            Serial.println(line);
            if (line.startsWith("data: ")) {
              String json = line.substring(6);
              String message = parseJSONMessage(json);
              displayMessage(message);
            }
            line = "";
          } else {
            line += c;
          }
        }
        delay(10);
      }
    }
  } else {  // request failed
    Serial.printf("SSE connection failed: %s\n", http.errorToString(status).c_str());
  }
  http.end();
}

String parseJSONMessage(String json) {
  int startIndex = json.indexOf(":\"") + 2; 
  int endIndex = json.lastIndexOf("\""); 
  return json.substring(startIndex, endIndex); 
}

void displayMessage(String msg) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Message: ");
  lcd.setCursor(0, 1);
  lcd.print(msg);

  if (msg.length() > 16) {
    scrollText(1, msg, 250, 16);
  }
}

void scrollText(int row, String message, int delayTime, int lcdColumns) {
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

void loop(){
  // reconnect if needed
  if (WiFi.status() == WL_CONNECTED) {
    connectToSSE();
  } else {
    Serial.println("Wifi disconnected, attempting to reconnect...");
    WiFi.reconnect();
    delay(5000);
  }
}
