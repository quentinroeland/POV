#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>

#include <ArduinoJson.h>
#define PIN 4
#define HALL 2
const int nbLeds = 69 ;

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
  char* bitmap[23][72];
};

Adafruit_NeoPixel strip = Adafruit_NeoPixel(nbLeds, PIN, NEO_GRB + NEO_KHZ800);

Image img{{{"e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c","e5262c"},
{"e5252c","e5252c","e5252c","e5252c","e5252c","e5252c","fbdfe0","fbdfe0","fbdfe0","fbdfe0","fbdfe0","fbdfe0","fbdfe0","fadfe0","fadfe0","fadfe0","fadfe0","fadfe0","fadfe0","fadfe0","fadfe0","fadfe0","fadfe0","fadfe0","fadfe0","f6c3c6","f6c3c6","f6c3c6","f6c3c6","f6c3c6","f6c3c6","e31720","e31720","e31720","e31720","e31720","e31720","e31720","e31720","e31720","e31720","e31720","e31720","e00009","e00009","e00009","e00009","e00009","e00005","e00005","e00005","e00005","e00005","e00005","e00005","e00005","e00005","e00005","e00005","e00005","e00005","e00005","e00005","e00005","e00005","e00005","e5252c","e5252c","e5252c","e5252c","e5252c","e5252c"},
{"e5252c","e5252c","e5252c","fbdfe0","fbdfe0","fbdfe0","fbdfe0","fbdfe0","fbdfe0","fbdfe0","fdfbfb","fdfbfb","fdfbfb","fdfbfb","fdfbfb","fdfbfb","fdfbfb","fdfbfb","fdfbfb","fdfbfb","fdfbfb","fceeef","fceeef","fceeef","fceeef","fceeef","fceeef","f6c3c6","e10c17","e10c17","e10c17","e10c17","e10c17","e10c17","e10411","e10411","e10411","e10411","e10411","e20512","e20512","e20512","e20512","e20512","e20512","e00009","eb646b","eb646b","eb646b","eb646b","eb646b","eb646b","ef8389","ef8389","ef8389","ef8389","ef8389","f08288","f08288","f08288","f08288","f08288","f08288","e00005","e00005","e00005","e00005","e00005","e00005","e00005","e5252c","e5252c"},
{"e4262d","e4262d","fadfe0","fadfe0","fadfe0","fadfe0","fefcfc","fefbfb","fefbfb","fefbfb","fefbfb","fefbfb","fefcfc","fdfbfb","fdfbfb","fdfbfb","fdfbfb","fefcfc","fefcfc","fefcfc","fbedee","fbedee","fbedee","fbedee","fbedee","e21521","e21521","e21521","e21521","e21521","e00007","e00108","e00108","e00108","e00108","e00309","e00309","e00309","e00309","e00309","e00309","e00309","e00309","e10611","e10611","e10611","e10611","e10611","fbebec","fbebec","fbebec","fbebec","fbebec","fdfcfc","fdfcfc","fdfcfc","fdfbfb","fdfbfb","fdfbfb","fdfbfb","fdfbfb","f08188","f08188","f08188","f08188","f08188","e00007","e00007","e00007","e00007","e00007","e4262d"},
{"e22627","e22627","fadfe0","fadfe0","fadfe0","fefcfb","fefcfc","fefcfc","fefcfc","fefcfc","fefcfc","fefcfc","fefcfc","fefefe","fefefe","fefefe","fefefe","fefefe","fefefe","fefefe","fdf5f5","fdf5f5","fdf5f5","e2141f","e31420","e31420","e00007","e00007","e00007","e00007","e00007","f29397","f29397","f29397","f29397","f29397","f29397","f29397","f29397","f29397","f29397","f29397","e00208","e00208","e00007","e00007","e00007","e2131e","e2131e","e21420","fbebec","fbebec","fbebec","fefcfd","fefcfd","fefcfd","fdfcfd","fdfcfd","fdfcfd","fefdfe","fdfbfb","fdfbfb","fefcfc","fefcfc","fefcfc","f08289","f08289","ef7f84","de0000","de0000","de0000","e22627"},
{"f3a5a8","f3a5a8","fdf5f5","fdf5f5","fefcfc","fefcfc","fefcfc","fefcfc","fefcfc","fefefe","fefefe","fefefe","fefefe","f08488","f08488","ef8588","ef8588","ef8689","ef8689","ef8689","ec666b","ec666b","e10610","e10610","e2141f","df0007","df0007","f19397","f29397","f29397","f29397","fefefe","fefefe","fefefe","fefefe","fefdfd","fefdfd","fefdfd","fefdfd","fefdfd","fefdfd","fefdfd","f29397","f29397","f29397","f29397","e00008","e00008","e00008","e2111d","e2111d","fae4e5","fae4e5","fdf7f8","fdf7f8","fdf7f8","fdf7f7","fdf7f7","fdf7f7","fdf7f7","fefefe","fefefe","fefefe","fefefe","fdfbfb","fdfbfb","ef7f84","f9d9da","f9d9da","ef8689","ef8689","f3a5a8"},
{"fefcfc","fdfdfd","fdfdfd","fefdfd","fefdfd","fefdfd","fefdfd","fefdfd","fefefe","fefefe","ef8589","ef8588","ef8588","ef8588","e1030d","e1030d","e1030e","e1030e","e1030e","e1030e","e1030e","e10310","e10310","e00309","e00309","e00309","f19397","f19397","fefefe","fefefe","fefefe","fefefe","f3a5aa","f3a5aa","f8d7d9","f8d7d9","fdfbfb","fdfbfb","fdfbfb","fdfcfc","fdfcfc","fefdfd","fefdfd","fefdfd","fefdfd","f29397","f19397","e00007","e00007","e00007","e10511","e10511","e74048","e74048","ea545c","ea535b","ea535b","ea535b","ea535b","fdf7f7","fdf7f7","fdf7f7","fcf6f7","fefefe","fefdfd","fefdfd","fefdfd","fefdfd","fefdfd","fefdfd","fdfcfc","fdfcfc"},
{"fefefe","fefefe","fefefe","fefefe","fefefe","fdfefe","fefefe","fefefe","fefefe","f09394","ef8589","e10410","e10410","e1020d","e30d19","e30d19","e30d19","e30d19","e30d19","e30d19","e30d19","e20c19","e20c19","e00208","e00208","f08f93","f19397","fefdfd","fefefe","f6bec0","f6bec0","f29ca2","df0001","df0001","e00a10","e00a10","ee8085","fdf2f2","fdf2f2","fdfbfb","fdfbfb","fefdfd","fefdfd","fefdfd","fefdfd","fefdfd","f19397","f19397","f19397","e00308","e20714","e20714","e10512","e10512","e10310","e10311","e10311","e10311","e10311","ea535b","ea535c","ea535c","fcf6f7","fdf8f8","fefdfd","fefefe","fefdfd","fefdfd","fefefe","fefefe","fefefe","fefefe"},
{"f6b4b7","f6b4b7","f6b4b7","f6b4b7","f6b4b7","fefdfd","fefdfd","fdfcfc","fbe6e7","f6b9ba","e11114","e32126","e20d1a","e20d19","fceced","fceced","fceced","fceced","fceced","fceced","fceced","fbecec","fbeced","e10b13","f4adb0","fefdfd","fefdfd","fdfbfb","fcf3f4","e6363f","df0003","df0001","e10713","e10610","e10410","e10410","e0040d","ea6064","ea6064","fcf2f2","fdfcfc","fefefe","fefefe","fdfdfd","fefdfd","fefcfc","fefdfd","fefdfd","f19094","e0040a","e84c4e","e8494d","e9494d","e9494d","e9494e","e9494e","e9494e","e9494e","e8494d","e10310","e10513","e10f13","eb6166","f9dadb","fdfdfd","fefdfd","fefdfd","fefefe","f6b4b7","f6b4b7","f6b4b7","f6b4b7"},
{"e30d17","e30d17","e30d17","e30d17","f6b4b7","f5b4b7","f6b4b7","fdfcfc","fdfcfc","f6b9ba","f7c9ca","e32126","fbeff0","fbebec","fceced","fefdfd","fefdfd","fefdfd","fefdfd","fefdfd","fefdfd","fefdfd","fbeced","fdfdfe","fdfdfe","fefdfd","fefcfc","fdfbfb","f8cfd1","e6363f","e1040e","e1050f","e10713","f19497","f19497","e21920","e10511","e00406","f29c9e","f29c9e","fdfcfc","fefdfd","fefdfd","fdfdfd","fcf9fa","fefcfc","fdfbfa","fefdfd","fefdfd","f6c7c9","e84c4e","fceeef","fceeef","fceeef","fceeef","fceeef","fceeef","fceeef","e8494d","e8494f","e95b5c","e10f13","f5b6b8","f9dadb","fefbfb","fefdfd","f6b4b7","f6b4b7","f6b4b7","e30d17","e30d17","e20d16"},
{"e1070b","e1070c","e1070c","e1070c","e30d17","e20d17","e30d17","f5b2b5","f5b2b5","fdfafa","fdfdfd","fdfdfd","fefdfd","fefdfd","fefdfd","fefefe","fefefe","fefefe","fefefe","fefefe","fefefe","fefefe","fefdfd","fefefe","fefefe","fefefe","fefefe","fefcfc","e42a2e","e42a2e","e0030b","e8454c","f3aeb2","fdfdfd","fefdfd","f6bfc2","e2131d","e1020f","e94d51","fcf5f5","fdfbfc","fefcfc","f9d1d2","f2a0a2","f2a0a2","fcf5f5","fdfcfc","fdfcfc","fefdfd","fdfdfd","fceff0","fefbfc","fefbfc","fefbfc","fefbfc","fefbfc","fefbfc","fefbfc","fceeef","fceeef","fdf1f1","f9d4d6","f9d4d6","fdfcfc","f5b4b7","f5b4b7","e20d15","e20d15","e20d15","e20613","e1070b","e1070b"},
{"f4b1b3","f5b2b4","f5b2b4","f5b2b4","e1070c","e1070c","e10709","e20c15","f6c3c4","fdfcfc","fdfdfd","fefefe","fefefe","fefefe","fefefe","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","fefefe","fefefe","fefefe","fefefe","fdfdfd","fdfdfd","f9d3d5","e0020d","e10510","e8444a","fdfefe","fdfbfb","fcf9f9","fcebec","e74245","e20713","e63038","fbe7e8","fdfbfb","fbe6e7","fbe6e7","e32629","e53a3d","f8d2d3","fcfcfc","fefdfd","fefefe","fefefe","fefefe","fefefe","fefefe","fefefe","fefefe","fefefe","fefefe","fefefe","fefcfc","fefbfc","fefefe","fdfcfc","fdfbfb","f8ced0","f4b2b6","e20d14","e20714","e20714","e20613","e20a16","ee7c81","f4b0b2"},
{"fdf8f8","fdfafa","fefbfb","fdfbfb","f5b2b4","f5b2b4","e10709","e2141a","fbeeef","fdfcfc","fefefe","fefefe","fefefe","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","fefefe","fefefe","fefdfd","fdfdfd","fcf7f8","df0005","e10510","f2999f","fdfefe","fefbfb","fdfcfc","fdf5f5","ea5d62","e10612","e2131b","f4b6b9","fdf9f9","fdfcfc","e73c44","e10713","ea6266","f8d2d3","fdfdfd","fefefe","fefefe","fefefe","fefefe","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","fefefe","fefefe","fefefe","fefefe","fdfbfb","f8ced0","e95556","e10612","e20916","e10510","e20613","e10613","e31b21","f19598"},
{"eb5c62","fadbdd","fdfbfb","fdfdfd","fefbfb","fefbfb","f6c5c6","f6c5c6","fbeeef","fefdfd","fefefe","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","fefefe","fefefe","fdfefe","f7c8c9","df0000","df0000","ee7b7f","fefefe","fdfcfc","fefdfd","fdfdfd","f3a8aa","e10e11","e10612","e2171f","fefefe","fbe6e7","ee7377","e1000a","e52f32","fcf4f5","fefefe","fefefe","fefefe","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","fefefe","fefefe","fefefe","fcf0f0","ea5b5e","eb666a","e21117","e31f25","e0010b","e10c12","e20713","e00609"},
{"e10610","e53135","f3a3a6","fdfbfb","fefefe","fefefe","fefbfb","fdfdfe","fefefe","fefefe","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","fefefe","fefefe","fefefe","fefcfc","f5acb0","df0000","e10410","e21019","ea5f62","fefdfd","fdfdfc","fcf1f2","ee7c7f","e0070f","e0030b","e6343a","e10510","e10714","e32129","fadddf","fefafa","fefefe","fefefe","fefefe","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","fefefe","fefefe","fefcfc","fdfdfd","faddde","fcf6f6","fdfcfc","f9d7d8","fcf5f5","e31e25","e1040e"},
{"e00309","e10915","ed7377","fdfbfb","fefdfd","fefefe","fefefe","fefefe","fefefe","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","fefefe","fefefe","fefefe","fefcfc","fae3e4","e0080e","df0107","ea5f62","fefefd","fefefe","fdfcfc","fdf9fa","e73c43","e0030b","e0020a","e00309","ec6f73","fbe4e5","fefbfb","fefefe","fefefe","fefefe","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","fefefe","fefefe","fefefe","fdfdfd","fefcfc","fdfdfd","fdfcfc","fcf5f5","fdf7f7","e63136"},
{"ed7073","e21318","e0020a","fdf9f9","fdfcfc","fefefe","fefefe","fffefe","fefefe","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","fefefe","fefefe","fefdfd","fefefe","fefbfc","e52c33","fdf1f2","fdfcfc","fdfcfc","fefdfd","fdfcfc","fcf8f8","e2161d","e52c33","fcf5f6","fadfdf","fdf6f7","fdfafa","fefefe","fefefe","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","fefefe","fefefe","fefefe","fefefe","fefefe","fefdfd","fdfcfc","fdfbfc","fbedee"},
{"fae1e3","f1979b","e20612","e73f44","fdf6f6","fefefe","fefefe","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","fefefe","fefefe","fefefe","fefbfc","fcf5f5","fdfdfd","fefdfd","fefefe","fefdfd","fefefe","fefdfd","fbebed","fdfcfc","fcf5f6","fdfdfd","fefefe","fefefe","fefefe","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","fefefe","fefefe","fefefe","fefefe","fefdfd","fdfdfd","fbe8e9","fbe7e8"},
{"e00813","e20512","e20512","e31b21","ef8185","fdfdfd","fefefe","fefefe","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","fefefe","fefefe","fefefe","fefdfd","fefefe","f5b8bb","f5b8bb","f5b8bb","f5b8bb","f5b7bb","fefefe","fefcfc","fefdfd","fefdfd","fefefe","fefefe","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","fefefe","fefefe","fdfdfd","fbe8e9","fbe8e9","e20a15","e10914"},
{"e1030e","e1030f","e1030f","e20714","df0207","fdfefe","fefefe","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","fefefe","fdfefe","f5b6b8","f5b7bb","e1020e","e1020e","e1020e","e1020e","e1020e","f5b7bb","f5b8bb","fefdfd","fefefe","fefefe","fefefe","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","fefefe","fefefe","fcf2f2","e20a15","e20a14","e1030f","e1030f"},
{"ef7c80","ef7c80","ef7c80","e1030f","e10b15","fefefe","fefefe","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","fefefe","fdfdfd","e2131c","e1010e","e20310","e20310","e20310","e20310","e20310","e1020e","e0020f","f8d6d7","fefefe","fefefe","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","fefefe","fefefe","ea585e","e10410","e1030e","ef7c80","ef7c80"},
{"fefefe","fefefe","fefefe","ef7b7f","f19699","fefdfd","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","fefdfd","e41b26","e10310","f9d5d7","f9d5d7","f9d5d7","f9d5d7","f9d5d7","e10310","e10310","fcf0f0","fefefe","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","fefcfc","fceeef","ee7b7f","ef7c80","fefefe","fefefe"},
{"fefcfc","fefcfc","fefcfc","fefefe","fdfefe","fefefe","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","fefefe","fdfefe","f9d2d5","fefefe","fefefe","fefefe","fefefe","fefefe","f9d5d7","fbe9e9","fdf1f1","fefefe","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","ffffff","fefefe","fdf9f9","fdfefe","fefefe","fefcfc","fefcfc"}}};

Image* draw;
void setup() {
  draw = &img;
  Serial.begin(115200); //Start communication between the ESP8266-12E and the monitor window
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

void loop() {
  unsigned long now = millis();
  server.handleClient();
  int tableSize = 72;
  if (!magnetNear()) {
    if (started) {
      unsigned long current = now - lastTime;
      for (int i = 0; i < nbLeds / 3; i++) {
        turn(draw->bitmap[i][(current * tableSize / diff - 1) % tableSize], i);
        server.handleClient();
      }
      for (int i = nbLeds / 3; i < 2 * nbLeds / 3; i++) {
        turn(draw->bitmap[22 - (i - nbLeds / 3)][((current * tableSize / diff - 1) + 33 ) % tableSize], i);
        server.handleClient();
      }
      for (int i = 2 * nbLeds / 3; i < nbLeds; i++) {
        turn(draw->bitmap[22 - (i - 2 * nbLeds / 3)][((current * tableSize / diff - 1) + 66 ) % tableSize], i);
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


void turn(RGB rgb, int i) {
  strip.setPixelColor(i, rgb.r, rgb.g, rgb.b);
}

void turn(char* hex, int i){
  int number = (int) strtol( hex, NULL, 16);

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


void treatImage() {

  Serial.println("holla");

  StaticJsonBuffer<10000> jsonBuffer;
  
  //
  // Step 2: Deserialize the JSON string
  //
  Serial.println(server.arg(0));
  //JsonObject& root = jsonBuffer.parseObject(server.arg(0));
  
  /*if (!root.success())
  {
    Serial.println("parseObject() failed");
    return;
  }*/
  
 //int i = root["i"];
 //int j = root["j"];
 //Serial.println(i);
 //Serial.println(j);
 //String image = server.arg("img");
 //Serial.println(image);
 Serial.println("holla");
  return;
}


