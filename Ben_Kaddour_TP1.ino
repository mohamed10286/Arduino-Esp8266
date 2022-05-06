#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;

unsigned long delayTime;
const uint8_t del = 2 ;
const uint8_t del2 = 15 ;
const uint8_t button = 13 ;
const unsigned long debounceDelay = 50 ;
int ledState = LOW;
unsigned long previousMillis = 0;
const long interval = 1000;
uint8_t buttonread ;
uint8_t delState = HIGH;
uint8_t buttonstate;
uint8_t dernierbuttonstate = LOW;
unsigned long dernierdebounce = 0;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3D
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define NUMFLAKES     10
#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16


static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000
};



void setup() {

  // Les pinMode

  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(del, OUTPUT);
  pinMode(del2, OUTPUT);
  pinMode(button, INPUT_PULLUP);

  // Serial et ecran de demarrage

  Serial.begin(74880);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  display.display();
  delay(2000);
  display.clearDisplay();
  display.drawPixel(10, 10, SSD1306_WHITE);
  display.display();
  delay(2000);

  while (!Serial);   // time to get serial running

  unsigned status;

  // default settings
  status = bme.begin(0x76);
  // You can also pass in a Wire library object like &Wire2
  // status = bme.begin(0x76, &Wire2)
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
    Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(), 16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    while (1) delay(10);
  }

  Serial.println("-- Default Test --");
  delayTime = 1000;

  Serial.println();



}


void loop() {

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Exécution de code à chaque seconde
    printValues();
    // fin de l'exécution de code
  }
  // d'autre code peut être exécuté

  CheckMax();
  led();
}



// affichage des valeurs lus du senseur sur sur l'ecran


void printValues() {
  display.clearDisplay();
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Temp = ");
  display.print(bme.readTemperature());
  display.print(" C");
  display.println();
  display.println();



  display.print("Press = ");



  display.print(bme.readPressure() / 100.0F);
  display.print(" hPa");
  display.println();
  display.println();



  display.print("Altitude = ");
  display.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  display.print(" m");
  display.println();
  display.println();




  display.print("Humidity = ");
  display.print(bme.readHumidity());
  display.print(" %");
  display.println();
  display.println();



  display.println();
  display.display();

}


// code du boutton et debounce lorsque on appuie qui va allumer les led d'une  maniére contraire

void led() {
  buttonread = digitalRead (button);
  if (buttonread != dernierbuttonstate) {

    dernierdebounce = millis();

  }

  if ((millis() - dernierdebounce ) > debounceDelay) {
    if ( buttonread != buttonstate) {
      buttonstate = buttonread;
      if (buttonstate == HIGH ) {
        delState = !delState;
      }

    }
  }

  digitalWrite(del, delState);
  digitalWrite(del2, !delState);
  dernierbuttonstate = buttonread;

}

// code pour fonctionner le buzzer lorsque on depasse 30 c
void CheckMax() {
  if (bme.readTemperature() > 30.0F) {
    digitalWrite(0, HIGH);

  }
  else {
    digitalWrite(0, LOW);
  }
}
