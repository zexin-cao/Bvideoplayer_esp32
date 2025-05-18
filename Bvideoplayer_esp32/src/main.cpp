#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include "../include/config.h"


// 硬件定义
#define BTN_UP   12
#define BTN_OK   13
#define BTN_DOWN 14

// 全局对象
TFT_eSPI tft;
int currentSelection = 0;
std::vector<String> videoTitles;

// 初始化WiFi
void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  tft.drawString("Connecting WiFi...", 10, 150);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    tft.drawString(".", 200 + (WiFi.status() % 3)*10, 150);
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
  
  for (int i=0; i<videoTitles.size(); i++) {
    if (i == currentSelection) {
      tft.fillRect(0, i*30, 240, 30, TFT_BLUE);
      tft.setTextColor(TFT_WHITE, TFT_BLUE);
    } else {
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
    }
    tft.drawString(videoTitles[i], 5, 5 + i*30);
  }
}

// 按钮检测
void checkButtons() {
  static unsigned long lastPress = 0;
  if (millis() - lastPress < 200) return;  // 防抖
  
  if (digitalRead(BTN_UP) == LOW) {
    currentSelection = (currentSelection - 1 + videoTitles.size()) % videoTitles.size();
    drawVideoList();
    lastPress = millis();
  } else if (digitalRead(BTN_DOWN) == LOW) {
    currentSelection = (currentSelection + 1) % videoTitles.size();
    drawVideoList();
    lastPress = millis();
  } else if (digitalRead(BTN_OK) == LOW) {
    // 可扩展播放功能
    tft.fillScreen(TFT_BLACK);
    tft.drawString("Selected: " + videoTitles[currentSelection], 10, 140);
    lastPress = millis();
  }
}

void setup() {
  // 初始化硬件
  Serial.begin(115200);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  
  // 初始化按钮
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_OK, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);

  // 连接网络
  connectWiFi();
  
  // 首次加载数据
  fetchPopularVideos();
  drawVideoList();
}

void loop() {
  checkButtons();
  delay(10);
}