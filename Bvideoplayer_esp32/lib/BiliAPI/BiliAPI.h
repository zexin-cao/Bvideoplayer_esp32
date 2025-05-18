#pragma once
#ifndef BILI_API_H
#define BILI_API_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <vector>

class BiliAPI {
public:
    struct VideoItem {
        String bvid;
        String title;
        String cover;
        int duration;
    };

    enum BiliApiError {
        API_OK,
        API_AUTH_FAILED,
        API_NETWORK_ERROR,
        API_JSON_ERROR
    };

    std::vector<VideoItem> getPopularVideos(int page = 1);
    String generateQRCodeUrl();
    bool addFavorite(const String& bvid);
    void setCredential(const String& user, const String& pass);

private:
    String _accessToken;
    String _refreshToken;
    unsigned long _tokenExpire = 0;
    String _user;  // 新增成员变量
    String _pass;  // 新增成员变量
    BiliApiError _apiGet(const String& endpoint, ArduinoJson::DynamicJsonDocument& result);
    BiliApiError refreshToken();
};

#endif