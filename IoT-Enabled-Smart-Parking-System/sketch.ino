#include <ESP8266WiFi.h> 
#include <ESP8266WebServer.h> 

// WiFi credentials 
const char* ssid = "WIFI-SSID"; 
const char* password = "WIFI-PASSWORD"; 

// IR sensor pins 
const int sensorPin1 = 5;  // GPIO 5 
const int sensorPin2 = 4;  // GPIO 4 
const int sensorPin3 = 0;  // GPIO 0 

// Initialize the web server 
ESP8266WebServer server(80); 

void handleRoot() {  
  String html = R"( 
    <!DOCTYPE html> 
    <html> 
    <head> 
      <title>IR Sensor Status</title> 
      <style> 
        body { 
          font-family: Arial, sans-serif; 
          display: flex; 
          flex-direction: column;
          align-items: center; 
          justify-content: center; 
          height: 100vh; 
          margin: 0; 
          background-color: #f4f4f9;
        } 
        .container { 
          text-align: center; 
          display: flex; 
          flex-direction: row; 
          gap: 15px; 
        } 
        .sensorBox { 
          width: 150px; 
          height: 100px; 
          color: white; 
          display: flex; 
          align-items: center; 
          justify-content: center; 
          font-size: 20px; 
          border-radius: 5px; 
          transition: background 0.3s; 
        } 
      </style> 
    </head> 
    <body> 
      <h1>IR Sensor Status</h1> 
      <div class="container"> 
        <div id="sensor1" class="sensorBox">Loading...</div> 
        <div id="sensor2" class="sensorBox">Loading...</div> 
        <div id="sensor3" class="sensorBox">Loading...</div> 
      </div> 
 
      <script> 
        // Function to fetch sensor data and update div colors 
        function updateSensorStatus() { 
          fetch('/sensors') 
            .then(response => response.json()) 
            .then(data => { 
              // Update div colors based on sensor data 
              document.getElementById('sensor1').style.backgroundColor = data.sensor1 == 0 ? 'red' : 'green'; 
              document.getElementById('sensor1').textContent = data.sensor1 == 0 ? 'Occupied' : 'Unoccupied'; 
 
              document.getElementById('sensor2').style.backgroundColor = data.sensor2 == 0 ? 'red' : 'green'; 
              document.getElementById('sensor2').textContent = data.sensor2 == 0 ? 'Occupied' : 'Unoccupied'; 
 
              document.getElementById('sensor3').style.backgroundColor = data.sensor3 == 0 ? 'red' : 'green'; 
              document.getElementById('sensor3').textContent = data.sensor3 == 0 ? 'Occupied' : 'Unoccupied'; 
            }) 
            .catch(error => console.error('Error:', error)); 
        } 
 
        // Update sensor status every second 
        setInterval(updateSensorStatus, 1000); 
      </script> 
    </body> 
    </html> 
  )"; 
  server.send(200, "text/html", html); 
} 
 
void handleSensors() { 
  int sensorValue1 = digitalRead(sensorPin1); 
  int sensorValue2 = digitalRead(sensorPin2); 
  int sensorValue3 = digitalRead(sensorPin3); 
     
  String json = "{\"sensor1\": " + String(sensorValue1) + 
                ", \"sensor2\": " + String(sensorValue2) + 
                ", \"sensor3\": " + String(sensorValue3) + "}"; 
  server.send(200, "application/json", json); 
} 
 
void setup() { 
  Serial.begin(9600); 
  pinMode(sensorPin1, INPUT);  // Set each sensor pin as input 
  pinMode(sensorPin2, INPUT); 
  pinMode(sensorPin3, INPUT); 
 
  // Connect to Wi-Fi 
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) { 
    delay(1000); 
    Serial.println("Connecting to WiFi..."); 
  } 
  Serial.println("Connected to WiFi"); 
  Serial.println("URL: http://" + WiFi.localIP().toString() + ":80"); 
 
  // Define routes 
  server.on("/", handleRoot); 
  server.on("/sensors", handleSensors);  // Route for all sensor data 
 
  // Start the server 
  server.begin(); 
  Serial.println("Server started"); 
} 

void loop() { 
  server.handleClient(); 
}