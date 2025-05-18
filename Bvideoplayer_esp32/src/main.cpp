#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include "../include/config.h"

// 硬件定义
#define BTN_PIN 12  // 按钮引脚
#define DEBOUNCE_TIME 200  // 消抖时间（毫秒）
#define LONG_PRESS_TIME 1000  // 长按时间（毫秒）
#define DOUBLE_CLICK_TIME 300  // 连按间隔时间（毫秒）

// 全局对象
TFT_eSPI tft;
int currentSelection = 0;
std::vector<String> videoTitles;
bool isLoggedIn = false;  // 登录状态
String authToken;  // 认证令牌

// 按钮状态变量
bool buttonState = HIGH;
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long lastPressTime = 0;
unsigned long lastClickTime = 0;
int clickCount = 0;

// 初始化WiFi
void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  tft.drawString("Connecting WiFi...", 10, 150);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    tft.drawString(".", 200 + (WiFi.status() % 3) * 10, 150);
  }
  tft.fillScreen(TFT_BLACK);
}

// 获取热门视频列表
void fetchPopularVideos() {
  HTTPClient http;
  http.begin(BILI_POPULAR_API);
  if (http.GET() == HTTP_CODE_OK) {
    DynamicJsonDocument doc(2048);  // 优化内存占用
    deserializeJson(doc, http.getString());
    JsonArray list = doc["data"]["list"];

    videoTitles.clear();
    for (JsonObject item : list) {
      String title = item["title"].as<String>();
      if (title.length() > 20) title = title.substring(0, 20) + "...";
      videoTitles.push_back(title);
      if (videoTitles.size() >= 8) break;  // 限制条目数
    }
  }
  http.end();
}

// 显示视频列表
void drawVideoList() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  for (int i = 0; i < videoTitles.size(); i++) {
    if (i == currentSelection) {
      tft.fillRect(0, i * 30, 240, 30, TFT_BLUE);
      tft.setTextColor(TFT_WHITE, TFT_BLUE);
    } else {
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
    }
    tft.drawString(videoTitles[i], 5, 5 + i * 30);
  }
}

// 显示登录界面
void showLoginScreen() {
  tft.fillScreen(TFT_BLACK);
  tft.drawString("Press button to login Bilibili", 10, 150);
}

// 登录 B 站（模拟）
void loginBilibili() {
  // 这里只是模拟登录过程，实际需要实现 OAuth 2.0 流程
  // 可以参考 B 站开放平台文档实现登录逻辑
  isLoggedIn = true;
  authToken = "simulated_token";  // 模拟认证令牌
  tft.fillScreen(TFT_BLACK);
  tft.drawString("Logged in successfully", 10, 150);
  delay(2000);
  fetchPopularVideos();
  drawVideoList();
}

// 按钮检测
void checkButtons() {
  bool reading = digitalRead(BTN_PIN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_TIME) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {  // 按钮按下
        lastPressTime = millis();
        clickCount++;
        if (clickCount == 1) {
          lastClickTime = millis();
        }
      } else {  // 按钮释放
        unsigned long pressDuration = millis() - lastPressTime;
        if (pressDuration >= LONG_PRESS_TIME) {  // 长按
          if (!isLoggedIn) {
            showLoginScreen();
          }
        } else {  // 短按
          if (clickCount == 2 && (millis() - lastClickTime) <= DOUBLE_CLICK_TIME) {  // 连按
            if (isLoggedIn) {
              // 可扩展播放功能
              tft.fillScreen(TFT_BLACK);
              tft.drawString("Selected: " + videoTitles[currentSelection], 10, 140);
            }
            clickCount = 0;
          } else {  // 单按
            if (isLoggedIn) {
              currentSelection = (currentSelection + 1) % videoTitles.size();
              drawVideoList();
            }
          }
        }
      }
    }
  }

  lastButtonState = reading;
}

void setup() {
  // 初始化硬件
  Serial.begin(115200);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  // 初始化按钮
  pinMode(BTN_PIN, INPUT_PULLUP);

  // 连接网络
  connectWiFi();

  if (isLoggedIn) {
    // 首次加载数据
    fetchPopularVideos();
    drawVideoList();
  } else {
    showLoginScreen();
  }
}

void loop() {
  checkButtons();
  delay(10);
}
