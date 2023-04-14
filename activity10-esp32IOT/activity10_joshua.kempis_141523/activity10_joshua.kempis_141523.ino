/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-oled
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED width,  in pixels
#define SCREEN_HEIGHT 64 // OLED height, in pixels
#define LIGHT_SENSOR_PIN 26
#define POT_PIN 2

// create an OLED display object connected to I2C
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(9600);
  //set the resolution to 12 bits (0-4096)
  analogReadResolution(7);

  // initialize OLED display with I2C address 0x3C
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("failed to start SSD1306 OLED"));
    while (1);
  }

  delay(2000);        
}

void loop() {
  int analogValue = analogRead(POT_PIN);
  int lightValue = analogRead(LIGHT_SENSOR_PIN);
  //Serial.printf("ADC analog value = %d\n",analogValue);
  oled.clearDisplay(); // clear display
  oled.setTextSize(1);         // set text size
  oled.setTextColor(WHITE);    // set text color
  oled.setCursor(2, 5);       // set position to display
  oled.print("Turn:  "); // set text
  oled.println(analogValue); // set text
  oled.setCursor(2, 15);       // set position to display
  oled.print("Light: "); // set text
  oled.println(lightValue); // set text
  oled.setCursor(2, 25);       // set position to display
  oled.print("Temp:  "); // set text
  oled.print(analogValue); // set text
  oled.write(247); // set text
  oled.print("C | "); // set text
  oled.print((analogValue*(9/5))+32); // set text
  oled.write(247); // set text
  oled.println("F"); // set text
  oled.display();              // display on OLED
  
}
