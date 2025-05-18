#pragma once
#ifndef SECURE_HTTP_H
#define SECURE_HTTP_H

#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <Preferences.h> // 添加Preferences头文件

class SecureHTTP {
public:
    static bool begin();
    static bool checkAuth(); // 声明checkAuth方法
    static String sendRequest(const String& endpoint, const String& params = ""); // 声明sendRequest方法
    static String get(const String& url);

    static WiFiClientSecure client;
    static Preferences prefs; // 声明Preferences静态成员

private:
    static const char* AUTH_NAMESPACE; // 认证命名空间
    static const char* root_ca; // SSL证书
};

#endif