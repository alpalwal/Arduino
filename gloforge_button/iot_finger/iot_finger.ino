#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>   // Include the WebServer library
#include <Servo.h>

ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'
ESP8266WebServer server(5646);    // Create a webserver object that listens for HTTP request on port 80

// Servo stuff
int basePosition=130; // ~180 degrees off of 0. If it's at 0, when it sweeps it'll go past the ground/button instead of sweeping through the air. 
Servo myservo;  // create servo object to control a servo

void handleRoot();              // function prototypes for HTTP handlers
void handleNotFound();

void setup(void){
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');

  wifiMulti.addAP("Yamaha", "UnifiSaintJames");   // add Wi-Fi networks you want to connect to
  wifiMulti.addAP("HG", "SerenaFamily!");
  
  Serial.println("Connecting ...");
  
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(250);
    Serial.print('.');
  }

  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // Tell us what network we're connected to
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer

  if (MDNS.begin("esp8266")) {              // Start the mDNS responder for esp8266.local
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }

  server.on("/", handleRoot);               // Call the 'handleRoot' function when a client requests URI "/"
  server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");

  pinMode(2, OUTPUT);
  myservo.attach(2);
  myservo.write(basePosition); // move to base position
}

void loop(void){
  server.handleClient();                    // Listen for HTTP requests from clients
}

void handleRoot() {
  server.send(200, "text/plain", "Hello world!");   // Send HTTP status 200 (Ok) and send some text to the browser/client
  moveServo(true);
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

// Move the Servo forwards and backwards
void moveServo(bool state){
  Serial.print("Servo state: ");
  if (state) {
    Serial.println("ON");
    int newVal = basePosition+20;
    myservo.write(newVal);
    delay(1000);
    myservo.write(basePosition); // Reset the servo position
    Serial.println("Finished button press");  
  } else {
    Serial.println("OFF");
    myservo.write(basePosition);
  }
}
