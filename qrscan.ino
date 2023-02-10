#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include <GoogleSheets.h>

#define OLED_ADDRESS 0x3C
Adafruit_SSD1306 display(OLED_ADDRESS);

const int rxPin = 2;
const int txPin = 3;
SoftwareSerial serialGM65(rxPin, txPin);

const int button1 = 4;
const int button2 = 5;

GoogleSheets googleSheets;

void setup() {
  // initialize display
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS);
  display.clearDisplay();
  display.display();

  // initialize GM65 serial
  serialGM65.begin(19200);
  delay(1000);

  // initialize buttons
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);

  // initialize Google Sheets
  if (!googleSheets.begin(Serial)) {
    display.println("Error initializing Google Sheets");
    return;
  }
}

void loop() {
  if (digitalRead(button1) == LOW) {
    // send command to GM65 to start scanning QR code
    serialGM65.println("AT+QRCODE=1");
    delay(2000);
    while (serialGM65.available() > 0) {
      String qrCode = serialGM65.readString();
      // display QR code on LCD
      display.clearDisplay();
      display.println(qrCode);
      display.display();
      // put QR code data in Google Sheet
      googleSheets.println(qrCode);
    }
  }
  if (digitalRead(button2) == LOW) {
    // perform other action if button2 is pressed
  }
}
