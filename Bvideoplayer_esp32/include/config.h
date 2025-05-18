#ifndef CONFIG_H
#define CONFIG_H
#pragma once

// B站API配置
#define BILI_OAUTH_URL "https://passport.bilibili.com"
#define BILI_API_ROOT "https://api.bilibili.com"
#define BILI_POPULAR_API "https://api.bilibili.com/x/web-interface/popular"

#include <Arduino.h>

// 硬件引脚配置（与User_Setup.h一致）
const uint8_t BUTTON_PIN = 0;
const uint8_t TFT_CS_PIN = 5;    // 对应TFT_CS
const uint8_t TFT_DC_PIN = 2;    // 对应TFT_DC
const int8_t TFT_RST_PIN = -1;   // 未使用硬件复位

// WiFi配置
const char* WIFI_SSID = "TP-LINK_A2D5";
const char* WIFI_PASS = "yqhy34553593";

#endif