#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include "WORLD_IR_CODES.h" // Sử dụng file thư viện mã IR đi kèm của TV-B-Gone

// --- Khai báo màn hình OLED ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- Khai báo Chân cắm (Pins) ---
#define PIN_UP      12 // D6 - Nút Lên / Chuyển Menu
#define PIN_SELECT  13 // D7 - Nút Chọn / Phát IR / Bắt đầu Deauth
#define PIN_DOWN    0  // D3 - Nút Xuống / Chuyển Menu
#define IR_LED_PIN  14 // D5 - Chân phát hồng ngoại IR[span_4](start_span)[span_4](end_span)

// --- Khai báo IR ---
IRsend irsend(IR_LED_PIN);[span_5](start_span)[span_5](end_span)

// --- Biến trạng thái Menu ---
enum Mode {
  MENU,
  TV_B_GONE,
  DEAUTHER_SCAN,
  DEAUTHER_ATTACK
};

Mode currentMode = MENU;
int menuIndex = 0;
const int TOTAL_MENU_ITEMS = 3;
const char* menuItems[] = {
  "1. TV-B-Gone (IR)",
  "2. WiFi Scan",
  "3. WiFi Attack"
};

// --- Hàm Khởi Tạo (Setup) ---
void setup() {
  Serial.begin(115200);

  // Cấu hình chân nút bấm
  pinMode(PIN_UP, INPUT_PULLUP);
  pinMode(PIN_SELECT, INPUT_PULLUP);
  pinMode(PIN_DOWN, INPUT_PULLUP);

  // Khởi tạo IR[span_6](start_span)[span_6](end_span)
  irsend.begin();[span_7](start_span)[span_7](end_span)

  // Khởi tạo màn hình OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Khong tim thay man hinh OLED SSD1306!"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 20);
  display.println(F("ESP8266 Combo"));
  display.setCursor(10, 35);
  display.println(F("TV-B-Gone + Deauth"));
  display.display();
  delay(1500);

  drawMenu();
}

// --- Vòng Lặp Chính (Loop) ---
void loop() {
  // Đọc trạng thái nút bấm (mức LOW khi nhấn)
  if (digitalRead(PIN_UP) == LOW) {
    delay(150); // Anti-bounce
    if (currentMode == MENU) {
      menuIndex = (menuIndex - 1 + TOTAL_MENU_ITEMS) % TOTAL_MENU_ITEMS;
      drawMenu();
    }
  }

  if (digitalRead(PIN_DOWN) == LOW) {
    delay(150);
    if (currentMode == MENU) {
      menuIndex = (menuIndex + 1) % TOTAL_MENU_ITEMS;
      drawMenu();
    }
  }

  if (digitalRead(PIN_SELECT) == LOW) {
    delay(200);
    if (currentMode == MENU) {
      switch (menuIndex) {
        case 0:
          currentMode = TV_B_GONE;
          runTVBGone();
          break;
        case 1:
          currentMode = DEAUTHER_SCAN;
          runWiFiScan();
          break;
        case 2:
          currentMode = DEAUTHER_ATTACK;
          runWiFiAttack();
          break;
      }
    } else {
      // Quay lại Menu chính nếu nhấn Select lần nữa
      currentMode = MENU;
      drawMenu();
    }
  }

  yield(); // Giữ ESP8266 không bị WDT Reset
}

// --- Hàm Hiển Thị Menu ---
void drawMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(F("=== MENU CHINTH ==="));
  display.println(F("-------------------"));

  for (int i = 0; i < TOTAL_MENU_ITEMS; i++) {
    if (i == menuIndex) {
      display.print(F("> "));
    } else {
      display.print(F("  "));
    }
    display.println(menuItems[i]);
  }

  display.display();
}

// --- Chức Năng 1: TV-B-Gone ---
void runTVBGone() {
  display.clearDisplay();
  display.setCursor(0, 10);
  display.println(F("Dang phat ma IR..."));
  display.println(F("Tat ca cac TV"));
  display.println(F("\nNhan SELECT de dung"));
  display.display();

  // Gọi hàm phát toàn bộ mã IR trong tập tin WORLD_IR_CODES.h[span_8](start_span)[span_8](end_span)
  sendAllCodes();[span_9](start_span)[span_9](end_span)

  // Cho phép thoát trở lại Menu chính
  currentMode = MENU;
  drawMenu();
}

// --- Chức Năng 2: WiFi Scan ---
void runWiFiScan() {
  display.clearDisplay();
  display.setCursor(0, 10);
  display.println(F("Dang quet WiFi..."));
  display.display();

  // Thêm logic quét mạng AP / Station từ mã nguồn ESP8266-Deauther tại đây[span_10](start_span)[span_10](end_span)
  delay(2000); 

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(F("Ket qua quet:"));
  display.println(F("- Found APs..."));
  display.println(F("\nNhan SELECT de ve Menu"));
  display.display();
}

// --- Chức Năng 3: WiFi Attack ---
void runWiFiAttack() {
  display.clearDisplay();
  display.setCursor(0, 10);
  display.println(F("Dang tan cong Deauth.."));[span_11](start_span)[span_11](end_span)
  display.println(F("Pkt/s: Sending..."));[span_12](start_span)[span_12](end_span)
  display.println(F("\nNhan SELECT de dung"));
  display.display();

  // Thêm logic phát gói tin Deauth/Beacon từ mã nguồn ESP8266-Deauther tại đây[span_13](start_span)[span_13](end_span)
}
