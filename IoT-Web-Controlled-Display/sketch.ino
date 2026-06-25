#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the LCD address to 0x27 for a 16 chars and 2 line display

AsyncWebServer server(80);

const char* ssid = "WIFI-SSID";
const char* password = "WIFI-PASSWORD";

const char* PARAM_INPUT_1 = "input1";

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
    <title>Smart Notice Board</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <style>
        body { 
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; 
            text-align: center; 
            margin-top: 50px; 
            background-color: #f7f9fc;
        }
        h1 { 
            color: #333; 
            font-size: 2.5rem; 
            margin-bottom: 30px;
        }
        form { 
            background: white; 
            padding: 20px; 
            display: inline-block; 
            border-radius: 8px; 
            box-shadow: 0 4px 6px rgba(0,0,0,0.1);
        }
        input[type="text"] { 
            padding: 10px; 
            font-size: 16px; 
            border: 1px solid #ccc; 
            border-radius: 4px; 
            width: 200px;
        }
        input[type="submit"] { 
            padding: 10px 20px; 
            font-size: 16px; 
            background-color: #007bff; 
            color: white; 
            border: none; 
            border-radius: 4px; 
            cursor: pointer; 
        }
        input[type="submit"]:hover { 
            background-color: #0056b3; 
        }
    </style>
</head>
<body>
    <h1>IoT Notice Board</h1>
    <form action="/get">
        <label for="input1">Enter Text to Display: </label>
        <input type="text" id="input1" name="input1" required>
        <input type="submit" value="Send">
    </form>
</body>
</html>
)rawliteral";

void notFound(AsyncWebServerRequest* request) {
  request->send(404, "text/plain", "Not found");
}

char ip_addr[16] = { 0 };

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("IoT Notice Board");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  WiFi.localIP().toString().toCharArray(ip_addr, 16);
  lcd.setCursor(0, 1);
  lcd.print(ip_addr);

  // Home Route
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/html", index_html);
  });

  // Handle Form Submission
  server.on("/get", HTTP_GET, [](AsyncWebServerRequest* request) {
    String message;
    if (request->hasParam(PARAM_INPUT_1)) {
      message = request->getParam(PARAM_INPUT_1)->value();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(message);
    } else {
      message = "No message sent";
    }
    Serial.println(message);

    request->send_P(200, "text/html", index_html);
  });

  server.onNotFound(notFound);
  server.begin();
}

void loop() {}