#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>

#include <ArduinoJson.h>
#define PIN 4 //pin for led data
#define HALL 2 //pin for hall sensor data
const int nbLeds = 69 ; //set total amount of leds

#define NLED_PER_PALE 23 //set number of leds per pale


// WiFi parameters to be configured
//WiFiServer server(80); //Initialize the server on Port 80
#include <ESP8266WebServer.h>
ESP8266WebServer server(80); //creating the server at port 80
int status = WL_IDLE_STATUS;  //temporary status assigned when WiFi.begin()

//date of last magnet inpulse
unsigned long lastTime = 0;

//know is sequence is started (led are ON)
bool started = false;

unsigned long diff = 1 ;
unsigned long secondTime = 0;

class RGB {
  public:
    byte r;
    byte g;
    byte b;
};

//define struct to use data table (received on post)
struct Image {
  //23 : number of leds per pale
  //72 : number of states of led on a complete rotation
  int bitmap[23][72];
};

//Define the strip using Adafruit librairy
Adafruit_NeoPixel strip = Adafruit_NeoPixel(nbLeds, PIN, NEO_GRB + NEO_KHZ800);

Image img; //data table (see struct definition)
Image* draw; //pointer on the struct to gain performances

void setup() {
  draw = &img; //set pointer
  Serial.begin(115200); //Start communication between the ESP8266-12E and the monitor window

  //Wifi Accesspoint parameters
  const char* ssid = "hotspot";
  const char* password = "password";
  
  //start connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println(""); //print a new line
  Serial.println("WiFi connected"); //then print Wifi is conneted
  Serial.println(WiFi.localIP()); //print IP address

  server.on("/image", HTTP_POST, treatImage); //set function "treatImage" as callback on post request for the endpoint /image
  server.begin(); //start server HTTP

  strip.begin(); //Set up strip
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(75); //set led brightness. DO NOT use a value greater than 80.
  pinMode(HALL, INPUT); //Set GPIO HALL as input for read data
}

#define NPALE 3 //number of pale
#define POSITION_PER_LED 72 //number of position (state) of a led per rotation in the data colors array
void loop() {
  unsigned long now = millis(); //get time 
  server.handleClient(); //server is listening
  int tableSize = POSITION_PER_LED; //set the size of data table
  
  //Synchronization of sequence with Hall sensor
  if (!magnetNear()) {
    if (started) {
      unsigned long current = now - lastTime;
	  
	  //set first pale
      for (int i = 0; i < nbLeds / NPALE; i++) {
		//for each led of pale
		//turn(img.bitmap[ledIndex][ledStateIndexWantedAtThisTime], ledIndex);
		//ledStateIndexWantedAtThisTime = calculate index with the time gap between two iterations
		//allows to be always synchronized independently of time/frequence of calcul
		//NOTICE first pale led index are inverted in data table (img.bitmap)
        turn(img.bitmap[NLED_PER_PALE-1- i][(current * tableSize / diff - 1) % tableSize], i);
        server.handleClient(); //server is still listening
      }
	  
	  //set second pale
      for (int i = nbLeds / 3; i < 2 * nbLeds / NPALE; i++) {
        turn(img.bitmap[(i - nbLeds / NPALE)][((current * tableSize / diff - 1) + tableSize/NPALE ) % tableSize], i);
        server.handleClient(); //server is still listening
      }
	  
	  //set third pale
      for (int i = 2 * nbLeds / 3; i < nbLeds; i++) {
        turn(img.bitmap[(i - 2 * nbLeds / NPALE)][((current * tableSize / diff - 1) + (2*tableSize)/NPALE ) % tableSize], i);
        server.handleClient(); //server is still listening
      }
    }
	
    strip.show(); //apply
	
  } else {
	//case where magnet is not near
	//update time data
    unsigned long tmpdiff = now - lastTime;
	
	//value of 50ms is the delta tolerated between two rotation
    if (tmpdiff > 50) {
      diff = tmpdiff;
      lastTime = now;
      started = true;
    }

  }
}

//Known if Hall sensor detect magnet
bool magnetNear() {
  int hallState = digitalRead(HALL); //read input
  if (hallState == 0) { 
    return true; //magnet is detected
  } else {
    return false;
  }

}
//newled (string) concatenation of each state color
//led (int) index of led in strip
void applyChanges(String newLed,int led){
  int i = 0;
  char* token;
  while (i<72){
    String s = newLed.substring(7*i + 14,(7*(i+1))-1 + 14 ); //parse each code color of concatenate string
    img.bitmap[led][i] = (int) strtol( &s[0u], NULL, 16); //set led color (converted from string to int) at state i
    i++;
  }
}

//set led of index i in strip from the color in RGB format
void turn(RGB rgb, int i) {
  strip.setPixelColor(i, rgb.r, rgb.g, rgb.b);
}

//set led of index i in strip from the color in HEX format
void turn(int number, int i){
  //int number = (int) strtol( hex, NULL, 16); //deprecated
  // Split them up into r, g, b values
  int r = number >> 16;
  int g = number >> 8 & 0xFF;
  int b = number & 0xFF;
  strip.setPixelColor(i, r, g, b);
}

//Turn off all leds
void turnOff() {
  for (int i = 0; i < nbLeds; i++) {
    strip.setPixelColor(i, 0);
  }
  strip.show(); //apply
}

// POST callback (/image)
int treatImage() {
  String answer = server.arg(0); //get json object on post request
  char* charAns = &answer[0u]; //0u unsigned int(0)
  int chosenLed= atoi(answer.substring(10,13).c_str()); //convert from string to int the led index
  applyChanges(answer, chosenLed); 
  server.send(200, "text/html", "ok "); //server response
  return 200;
}

