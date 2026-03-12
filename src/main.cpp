#include <Arduino.h>
#include <heltec-eink-modules.h>
#include <Fonts/FreeMono9pt7b.h>

EInkDisplay_WirelessPaperV1_1 display;

void setup() {
Serial.begin(115200);
  display.setFont(&FreeMono9pt7b);
	display.landscape();


  // delay(2000);

  // const int x = display.centerX();
  // const int y = display.centerY();
  // display.drawCircle(x, y, 10, 0);
  // display.drawCircle(x, y, 20, 0);
  // display.drawCircle(x, y, 30, 0);
  // display.drawCircle(x, y, 40, 0);
  // display.drawCircle(x, y, 50, 0);

  // display.update();

  // delay(2000);
  // display.clear();
}

void loop() {
  if (Serial.available() > 0) {
    const int h = display.height();
    const int w = display.width();
    String input = Serial.readStringUntil('\n');
    Serial.print("Received: ");
    Serial.println(input);
    display.drawRect(w / 4, h / 4, w / 2, h / 2, 0);
    display.printCenter(input);
    display.update();
    display.clearMemory();
  }
}
