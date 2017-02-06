#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>

#include <ArduinoJson.h>
#define PIN 4
#define HALL 2
const int nbLeds = 69 ;

#define NLED_PER_PALE 23 
// WiFi parameters to be configured


//WiFiServer server(80); //Initialize the server on Port 80
#include <ESP8266WebServer.h>
ESP8266WebServer server(80); //creating the server at port 80
int status = WL_IDLE_STATUS;

unsigned long lastTime = 0;

bool started = false;

unsigned long diff = 1 ;
unsigned long secondTime = 0;

class RGB {
  public:
    byte r;
    byte g;
    byte b;
};

struct Image {
  int bitmap[23][72];
};

Adafruit_NeoPixel strip = Adafruit_NeoPixel(nbLeds, PIN, NEO_GRB + NEO_KHZ800);

Image img;
Image* draw;
void setup() {
  draw = &img;
  Serial.begin(115200); //Start communication between the ESP8266-12E and the monitor window

  const char* ssid = "Freebox-49B5E2";
const char* password = "dejuras-exierint-ramusculi";
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //print a new line, then print WiFi connected and the IP address
  Serial.println("");
  Serial.println("WiFi connected");
  // Print the IP address
  Serial.println(WiFi.localIP());

  server.on("/image", HTTP_POST, treatImage);
  server.begin();

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(75);
  pinMode(HALL, INPUT);
}

#define NPALE 3 
#define POSITION_PER_LED 72
void loop() {
  unsigned long now = millis();
  server.handleClient();
  int tableSize = POSITION_PER_LED;
  if (!magnetNear()) {
    if (started) {
      unsigned long current = now - lastTime;
      for (int i = 0; i < nbLeds / NPALE; i++) {
        turn(img.bitmap[NLED_PER_PALE-1- i][(current * tableSize / diff - 1) % tableSize], i);
        server.handleClient();
      }
      for (int i = nbLeds / 3; i < 2 * nbLeds / NPALE; i++) {
        turn(img.bitmap[(i - nbLeds / NPALE)][((current * tableSize / diff - 1) + tableSize/NPALE ) % tableSize], i);
        server.handleClient();
      }
      for (int i = 2 * nbLeds / 3; i < nbLeds; i++) {
        turn(img.bitmap[(i - 2 * nbLeds / NPALE)][((current * tableSize / diff - 1) + (2*tableSize)/NPALE ) % tableSize], i);
        server.handleClient();
      }
    }
    strip.show();
  } else {
    unsigned long tmpdiff = now - lastTime;
    if (tmpdiff > 50) {
      diff = tmpdiff;
      lastTime = now;
      started = true;

    }

  }
}

bool magnetNear() {
  int hallState = digitalRead(HALL);
  if (hallState == 0) {
    return true;
  } else {
    return false;
  }

}

void applyChanges(String newLed,int led){
  int i = 0;
  char* token;
  while (i<72){
    String s = newLed.substring(7*i + 14,(7*(i+1))-1 + 14 );
    img.bitmap[led][i] = (int) strtol( &s[0u], NULL, 16);
    i++;
  }
}

void turn(RGB rgb, int i) {
  strip.setPixelColor(i, rgb.r, rgb.g, rgb.b);
}

void turn(int number, int i){
  //int number = (int) strtol( hex, NULL, 16);
 
  // Split them up into r, g, b values
  int r = number >> 16;
  int g = number >> 8 & 0xFF;
  int b = number & 0xFF;
  strip.setPixelColor(i, r, g, b);
  
}


void turnOff() {
  for (int i = 0; i < nbLeds; i++) {
    strip.setPixelColor(i, 0);
  }
  strip.show();
}


int treatImage() {

  String answer = server.arg(0);
  char* charAns = &answer[0u];
  int chosenLed= atoi(answer.substring(10,13).c_str());
  
  applyChanges(answer, chosenLed);
  server.send(200, "text/html", "ok ");
  return 200;
}

