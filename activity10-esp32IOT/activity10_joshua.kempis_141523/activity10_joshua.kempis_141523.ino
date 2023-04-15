#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>



#define SCREEN_WIDTH 128 // OLED width,  in pixels
#define SCREEN_HEIGHT 64 // OLED height, in pixels
#define LIGHT_SENSOR_PIN 26
#define POT_PIN 2
#define DHTPIN 32
#define DHTTYPE DHT11
#define BUTTON_PIN_BITMASK 0x200000000 // 2^33 in hex

RTC_DATA_ATTR int bootCount = 0;

// create an OLED display object connected to I2C
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
DHT dht(DHTPIN, DHTTYPE);

void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}

void setup() {
  Serial.begin(9600);
  //set the resolution to 12 bits (0-4096)
  analogReadResolution(7);

  // initialize OLED display with I2C address 0x3C
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("failed to start SSD1306 OLED"));
    while (1);
  }

  dht.begin();
  //Increment boot number and print it every reboot
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  //Print the wakeup reason for ESP32
  print_wakeup_reason();
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33,1); //1 = High, 0 = Low

  //If you were to use ext1, you would use it like
  //esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK,ESP_EXT1_WAKEUP_ANY_HIGH);

  delay(2000);
  //Go to sleep now
  Serial.println("Going to sleep now");
  esp_deep_sleep_start();
  Serial.println("This will never be printed");
}

void loop() {
  int analogValue = analogRead(POT_PIN);
  int lightValue = analogRead(LIGHT_SENSOR_PIN);

  float humValue = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float tempValue = dht.readTemperature();
  // Check if any reads failed and exit early (to try again).
  if (isnan(humValue) || isnan(tempValue)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  //Serial.printf("ADC analog value = %d\n",analogValue);
  oled.clearDisplay(); // clear display
  oled.setTextSize(1);         // set text size
  oled.setTextColor(WHITE);    // set text color
  oled.setCursor(2, 5);       // set position to display
  oled.print("Turn:  "); // set text
  oled.println(int(analogValue / 8)); // set text
  oled.setCursor(2, 15);       // set position to display
  oled.print("Light: "); // set text
  oled.println(lightValue); // set text
  oled.setCursor(2, 25);       // set position to display
  oled.print("Temp:  "); // set text
  oled.print(tempValue); // set text
  oled.write(247); // set text
  oled.println("C"); // set text
  oled.setCursor(2, 35);       // set position to display
  oled.print("Humidity: "); // set text
  oled.println(humValue); // set text
  oled.display();              // display on OLED

}
