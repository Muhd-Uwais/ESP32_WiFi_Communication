#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

const char* ssid = "WiFi";
const char* password = "Password";

const int ledPin = 2; // Adjust based on your LED pin
const int serverPort = 80; // Port for web server

IPAddress staticIP(192, 168, 1, 75);
IPAddress gateway(192, 168, 1, 1);  
IPAddress subnet(255, 255, 255, 0);

WiFiServer server(serverPort);

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  // Connect to Wi-Fi
  // WiFi.config(staticIP, gateway, subnet);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  digitalWrite(ledPin, HIGH);
  delay(400);
  digitalWrite(ledPin, LOW);
  Serial.println("");
  Serial.print("Connected to WiFi! IP address: ");
  Serial.println(WiFi.localIP());

  // Start web server
  server.begin();
  Serial.println("Web server started!");
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        String command = client.readStringUntil('\r');
        Serial.print("Received command: ");
        Serial.println(command);

        // Execute command based on value
        if (command.indexOf("on") != -1) {
          digitalWrite(ledPin, HIGH);
          client.println("LED turned ON");
        } else if (command.indexOf("off") != -1) {
          digitalWrite(ledPin, LOW);
          client.println("LED turned OFF");
        } else {
          for(int i = 0; i < 3; i++){
            digitalWrite(ledPin, HIGH);
            delay(100);
            digitalWrite(ledPin, LOW);
            delay(100);
          }
          client.println("Unknown command");
        }
      }
    }
    client.stop();
  }
}
