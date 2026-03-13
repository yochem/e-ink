// API
// https://github.com/todd-herbert/heltec-eink-modules/blob/main/docs/API.md
#include <Arduino.h>
#include <heltec-eink-modules.h>

#include <SFNSMono8pt7b.h>
EInkDisplay_WirelessPaperV1_1 display;

const GFXfont *font = &SFNSMono8pt7b;

#define DRAW_UI(...)                                                           \
  DRAW(display) {                                                              \
    draw_header();                                                             \
    __VA_ARGS__;                                                               \
  }

void draw_header() {
  display.setFont();
  const int pad = 10;
  display.setCursor(10, pad / 2 + 2);
  display.print("2026-03-12 19:47");
  const int h = display.getTextHeight("A");
  display.drawLine(0, pad + h, 250, pad + h, 0);
  display.setFont(font);
  display.setCursor(pad, 2 * pad + h + 15);
}

enum Screen { SCREEN_HOME, SCREEN_A, SCREEN_B };
enum Action {
  PRESS_SINGLE,
  PRESS_DOUBLE,
  PRESS_TRIPLE,
  ENTER_SCREEN,
  EXIT_SCREEN,
};

struct State {
  Screen screen;
  int cursor;
  int menu_count;
};

State state = {SCREEN_HOME, 0, 0};


void switch_screen(Screen next);
void dispatch(Action action);

void screen_home(Action action) {
  switch (action) {
    case PRESS_DOUBLE:
      switch (state.cursor) {
        case 0: switch_screen(SCREEN_A); return;
        case 1: switch_screen(SCREEN_B); return;
      }
    case PRESS_SINGLE:
      state.cursor = (state.cursor + 1) % state.menu_count;
      break;
    case ENTER_SCREEN:
      state.menu_count = 3;
      state.cursor = 0;
      break;
    // ignored:
    case PRESS_TRIPLE:
    case EXIT_SCREEN:
      break;
  }
  DRAW_UI(
    display.printf("(%s) option A\n", state.cursor == 0 ? "x" : " ");
    display.printf("(%s) option B\n", state.cursor == 1 ? "x" : " ");
    display.printf("(%s) option C\n", state.cursor == 2 ? "x" : " ");
  );
}

void screen_a(Action action) {
  switch (action) {
    case PRESS_TRIPLE:
      switch_screen(SCREEN_HOME);
      return;
    // ignored:
    case PRESS_SINGLE:
    case PRESS_DOUBLE:
    case ENTER_SCREEN:
    case EXIT_SCREEN:
      break;
  }
  DRAW_UI(
    display.println("Hi from screen A");
  );
}
void screen_b(Action action) {
  switch (action) {
    case PRESS_TRIPLE:
      switch_screen(SCREEN_HOME);
      return;
    // ignored:
    case PRESS_SINGLE:
    case PRESS_DOUBLE:
    case ENTER_SCREEN:
    case EXIT_SCREEN:
      break;
  }
  DRAW_UI(
    display.println("Hi from screen B");
  );
}


void dispatch(Action action) {
  switch (state.screen) {
    case SCREEN_HOME:  screen_home(action);  break;
    case SCREEN_A:     screen_a(action);     break;
    case SCREEN_B:     screen_b(action);     break;
  }
}

void switch_screen(Screen next) {
  dispatch(EXIT_SCREEN);
  state.screen = next;
  state.cursor = 0;
  state.menu_count = -1;
  dispatch(ENTER_SCREEN);
}


void checkSerialInput() {
  if (!Serial.available()) {
    return;
  }
  String s = Serial.readStringUntil('\n');
  s.trim();
  if (s.length() == 1) {
    Serial.println("single press");
    dispatch(PRESS_SINGLE);
  } else if (s.length() == 2) {
    Serial.println("double press");
    dispatch(PRESS_DOUBLE);
  } else if (s.length() == 3) {
    Serial.println("triple press");
    dispatch(PRESS_TRIPLE);
  }
}

void setup() {
  Serial.begin(115200);
  display.landscape();
  display.clear();
  display.fastmodeOn();
  display.setFont(font);
  dispatch(ENTER_SCREEN);
}

void loop() {
  checkSerialInput();
}
