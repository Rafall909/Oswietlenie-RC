#include <SoftwareSerial.h> 
#include <Adafruit_NeoPixel.h>

#define LED_PIN A5    // pin sterowania LED
#define LED_COUNT 12  // liczba ledów w szeregu

#define FLLO 0  // Front Left Lamp Outside
#define FLLI 1  // Front Left Lamp Inside
#define FRLI 4  // Front Right Lamp Inside
#define FRLO 5  // Front Right Lamp Outside
#define RRLO 7  // Rear Right Lamp Outside
#define RR 8    // Right Reverse
#define LR 9    // Lamp Reverse
#define RLLO 10 // Rear Left Lamp Outside

#define FLB 2   // Front Left Blinker
#define FRB 3   // Front Right Blinker
#define RRB 6   // Rear Right Blinker
#define RLB 11  // Rear Left Blinker

#define turnPin A3  // pin do wykrycia skrętu
#define stopPin A2  // pin do wykrycia hamowania

SoftwareSerial BTserial(3, 2); // RX | TX

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800); // deklaracja LED

int y = 0;
int lights_on = 0;
int i = 0;
int clear_0 = 0;
int jasnosc = 255;
int tryb = 0;
int left_range = 1250;
int right_range = 1750; 
int stop_range = 1320;
int blinker_start = 0;
unsigned long blinker_starttime = 0;
unsigned long blinker_time = 0;
int blinker_delay = 600;
int turn=0;
int stop=0;
int policja_delay = 180;
void setup() {

  Serial.begin(115200);
  BTserial.begin(115200); 
  pinMode(A0, INPUT);
  pinMode(A5, OUTPUT);
  strip.begin();            // inicjalizacja LED
  strip.show(); 
  strip.setBrightness(jasnosc);

  pinMode(stopPin, INPUT);
  pinMode(turnPin, INPUT);

}


void loop() {

  // odczyt z aparatury sterującej

stop=pulseIn(stopPin,HIGH);
turn=pulseIn(turnPin,HIGH);

  sprawdz();

  switch (tryb) {

    case 99:  // światła wyłączone, oczekiwanie na włączenie poprzez aplikację
      lights_on=0;
      clear_0=0;
      y=0;
      strip.clear();
      strip.show();
      sprawdz();
    break;

    case 0:  // światła drogowe, kierunkowskazy reagujące na skręt (aktualnie do testów podza modelem skręt pozorowany poprzez potencjometr)
      y=0;
      sprawdz();

      // światła awaryjne, gdy aparatura nie jest podłączona

      if (stop==0){
        lights_on=0;
        clear_0=0;
        i=0;
        strip.setPixelColor(FLLO, i, i, i);
        strip.setPixelColor(FLLI, i, i, i);
        strip.setPixelColor(FRLI, i, i, i);
        strip.setPixelColor(FRLO, i, i, i);
        strip.setPixelColor(RRLO, i, 0, 0);
        strip.setPixelColor(RR, i, 0, 0);
        strip.setPixelColor(LR, i, 0, 0);
        strip.setPixelColor(RLLO, i, 0, 0);

        blinker_time = millis();

        if (blinker_start == 0) {
          blinker_starttime = blinker_time;
          blinker_start = 1;
        }
        if ((blinker_time - blinker_starttime) <= blinker_delay) {

          strip.setPixelColor(FLB, 255, 80, 0);
          strip.setPixelColor(RLB, 255, 80, 0);

          strip.setPixelColor(FRB, 255, 80, 0);
          strip.setPixelColor(RRB, 255, 80, 0);
        } 
        strip.show();

        if ((blinker_time - blinker_starttime) > blinker_delay && (blinker_time - blinker_starttime) <= (2 * blinker_delay)) {

          strip.setPixelColor(FLB, 0, 0, 0);
          strip.setPixelColor(RLB, 0, 0, 0);

          strip.setPixelColor(FRB, 0, 0, 0);
          strip.setPixelColor(RRB, 0, 0, 0);
        }
        strip.show();

        if ((blinker_time - blinker_starttime) > (2 * blinker_delay)) {
          blinker_start = 0;
        }
        break;
      }
      if (clear_0 == 0) {
        strip.clear();
        strip.show();
        clear_0 = 1;
      }
      sprawdz();
      while (lights_on == 0) {
        for (i = 0; i <= 120; i++) {
          //przol lewy
          strip.setPixelColor(FLLO, i, i, i);
          strip.setPixelColor(FLLI, i, i, i);
          //przod prawy
          strip.setPixelColor(FRLI, i, i, i);
          strip.setPixelColor(FRLO, i, i, i);
          //tyl prawy
          strip.setPixelColor(RRLO, i, 0, 0);
          //strip.setPixelColor(RR, i, 0, 0);
          //tyl lewy
          //strip.setPixelColor(LR, i, 0, 0);
          strip.setPixelColor(RLLO, i, 0, 0);

          strip.show();
          delay(3);
          if (i == 120) { lights_on = 1; }
        }
      }
      blinker_time = millis();
      if (blinker_start == 0) {
        blinker_starttime = blinker_time;
        blinker_start = 1;
      }
      if ((blinker_time - blinker_starttime) <= blinker_delay) {

        if (turn <= left_range) {
          //przol lewy
          strip.setPixelColor(FLB, 255, 80, 0);
          //tyl lewy
          strip.setPixelColor(RLB, 255, 80, 0);
        }
        if (turn >= right_range) {
          //przod prawy
          strip.setPixelColor(FRB, 255, 80, 0);
          //tyl prawy
          strip.setPixelColor(RRB, 255, 80, 0);
        } if(turn>left_range && turn < right_range) {
          strip.setPixelColor(FLB, 0, 0, 0);
          strip.setPixelColor(RLB, 0, 0, 0);
          strip.setPixelColor(FRB, 0, 0, 0);
          strip.setPixelColor(RRB, 0, 0, 0);
        }
        strip.show();
      }
      if ((blinker_time - blinker_starttime) > blinker_delay && (blinker_time - blinker_starttime) <= (2 * blinker_delay)) {
        if (turn <= left_range) {
          //przol lewy
          strip.setPixelColor(FLB, 0, 0, 0);
          //tyl lewy
          strip.setPixelColor(RLB, 0, 0, 0);
        }

        if (turn >= right_range) {
          //przod prawy
          strip.setPixelColor(FRB, 0, 0, 0);

          //tyl prawy
          strip.setPixelColor(RRB, 0, 0, 0);
        } if(turn>left_range && turn < right_range) {
          strip.setPixelColor(FLB, 0, 0, 0);
          strip.setPixelColor(RLB, 0, 0, 0);
          strip.setPixelColor(FRB, 0, 0, 0);
          strip.setPixelColor(RRB, 0, 0, 0);
        }
        strip.show();
      }
      if ((blinker_time - blinker_starttime) > (2 * blinker_delay)) {
        blinker_start = 0;
      }
      if(stop<=stop_range){
        strip.setPixelColor(RR, 255, 0, 0);
        strip.setPixelColor(LR, 255, 0, 0);
      }
      else {strip.setPixelColor(RR, 0, 0, 0);
        strip.setPixelColor(LR, 0, 0, 0); }
      break;

 
    case 1:  // światła migające, losowe kolory

      clear_0 = 0;
      lights_on = 0;
      y=0;
      for (int i = 0; i < strip.numPixels(); i++) {               
        strip.setPixelColor(i, random(255), random(255), random(255));
        strip.show();
        delay(5);
        sprawdz();
        if (tryb != 1) { break; }
      }
    break;



    case 2:  // światła policyjne (migające niebiesko-czerwone), wykorzystuje zmienne wykorzystywane do kierunkowskazów
      clear_0 = 0;
      lights_on = 0;
      blinker_starttime = millis();
      if (y == 0) {
        blinker_time = blinker_starttime;
        y = 1;
      }
      if ((blinker_time - blinker_starttime) <= policja_delay) {
          strip.setPixelColor(FLB, 0, 0, 0);
          strip.setPixelColor(RLB, 0, 0, 0);
          strip.setPixelColor(FRB, 0, 0, 0);
          strip.setPixelColor(RRB, 0, 0, 0);
          //przol lewy
          strip.setPixelColor(FLLO, 255, 0, 0);
          strip.setPixelColor(FLLI, 0, 0, 255);
          //przod prawy
          strip.setPixelColor(FRLI, 255, 0, 0);
          strip.setPixelColor(FRLO, 0, 0, 255);
          //tyl prawy
          strip.setPixelColor(RRLO, 255, 0, 0);
          strip.setPixelColor(RR, 0, 0, 255);
          //tyl lewy
          strip.setPixelColor(LR, 255, 0, 0);
          strip.setPixelColor(RLLO, 0, 0, 255);
          strip.show();
          sprawdz();
        }
      if ((blinker_time - blinker_starttime) > policja_delay && (blinker_time - blinker_starttime) <= (2 * policja_delay)) {
          //przol lewy
          strip.setPixelColor(FLLO, 0, 0, 255);
          strip.setPixelColor(FLLI, 255, 0, 0);
          //przod prawy
          strip.setPixelColor(FRLI, 0, 0, 255);
          strip.setPixelColor(FRLO, 255, 0, 0);
          //tyl prawy
          strip.setPixelColor(RRLO, 0, 0, 255);
          strip.setPixelColor(RR, 255, 0, 0);
          //tyl lewy
          strip.setPixelColor(LR, 0, 0, 255);
          strip.setPixelColor(RLLO, 255, 0, 0);
          strip.show();
          sprawdz();
       }
       if ((blinker_time - blinker_starttime) > (2 * policja_delay)) {
          y = 0;
          sprawdz();
      }
    break;
  }
}

// funkcja realizująca zmianę trybu zainicjowaną przez aplikację na telefonie
void sprawdz() {
 if (BTserial.available())
    tryb=BTserial.read();
  }