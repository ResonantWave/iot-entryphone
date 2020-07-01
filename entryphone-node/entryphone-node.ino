#include <ESP8266WiFi.h>
 
const char* ssid = "MyWifi"; // Your WiFi SSID
const char* password = "MyPassword"; // Your WiFi Password

WiFiServer server(80);
String header;

/* // Uncomment this block if you want a static IP (you probably do)
   // Also don't forget to uncomment the WiFi.config line below.
IPAddress ip(192, 168, 0, 7);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
*/

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(0, OUTPUT);
  digitalWrite(0, LOW);

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.hostname("DoorOpener"); // change to whatever you fancy
  //WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());

  server.begin();
}
 
void loop(){
  WiFiClient client = server.available();

  if(WiFi.status() != WL_CONNECTED) {
    WiFi.config(ip, gateway, subnet);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  }
  
  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            if (header.indexOf("GET /open") >= 0) {
              int delayTime = midString(header, "?delay=", "HTTP").toInt(); // probably the worst way to get the parameter, but this device will not be directly exposed to the Internet.

              if(delayTime == 0)
                delayTime = 3000;
                
              digitalWrite(0, HIGH);
              delay(delayTime);
              client.println("ok");
              digitalWrite(0, LOW);
            } 
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

String midString(String str, String start, String finish) {
  int locStart = str.indexOf(start);
  if (locStart == -1) return "";
  locStart += start.length();
  int locFinish = str.indexOf(finish, locStart);
  if (locFinish == -1) return "";
  return str.substring(locStart, locFinish);
}

 