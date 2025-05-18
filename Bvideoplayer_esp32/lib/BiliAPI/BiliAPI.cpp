#include "BiliAPI.h"
#include "SecureHTTP.h"
#include "../include/config.h"
#include <ArduinoJson.h>
#define BILI_OAUTH_URL "https://passport.bilibili.com"
#define BILI_API_ROOT "https://api.bilibili.com"
// 确保在config.h中定义以下宏（示例值）
// #define BILI_OAUTH_URL "https://passport.bilibili.com"
// #define BILI_API_ROOT "https://api.bilibili.com"

String BiliAPI::generateQRCodeUrl() {
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient http;
    
    String url = String(BILI_OAUTH_URL) + "/x/passport-login/web/qrcode/generate";
    http.begin(client, url);
    
    if (http.GET() != HTTP_CODE_OK) return "";
    
    DynamicJsonDocument doc(512);
    if (deserializeJson(doc, http.getString()) != DeserializationError::Ok) {
        http.end();
        return "";
    }
    
    if (doc["code"].as<int>() == 0 && doc["data"].containsKey("url")) {
        return doc["data"]["url"].as<String>();
    }
    http.end();
    return "";
}

bool BiliAPI::addFavorite(const String& bvid) {
    String endpoint = "/x/v3/fav/resource/deal";
    String params = "rid=" + bvid + "&type=2";
    
    DynamicJsonDocument doc(256);
    BiliApiError err = _apiGet(endpoint + "?" + params, doc);
    return (err == API_OK);
}

BiliAPI::BiliApiError BiliAPI::_apiGet(const String& endpoint, ArduinoJson::DynamicJsonDocument& result) {
    if (millis() > _tokenExpire) {
        BiliApiError err = refreshToken();
        if (err != API_OK) return err;
    }
    
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient http;
    
    String url = String(BILI_API_ROOT) + endpoint;
    http.begin(client, url);
    http.addHeader("Authorization", "Bearer " + _accessToken);
    
    int code = http.GET();
    if (code != HTTP_CODE_OK) {
        http.end();
        return (code == HTTP_CODE_UNAUTHORIZED) ? API_AUTH_FAILED : API_NETWORK_ERROR;
    }
    
    DeserializationError jsonErr = deserializeJson(result, http.getString());
    http.end();
    
    return (jsonErr) ? API_JSON_ERROR : 
           (result["code"].as<int>() == 0 ? API_OK : API_AUTH_FAILED);
}

BiliAPI::BiliApiError BiliAPI::refreshToken() {
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient http;
    
    String url = String(BILI_OAUTH_URL) + "/x/passport-login/oauth2/refresh_token";
    http.begin(client, url);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    String payload = "refresh_token=" + _refreshToken;
    int code = http.POST(payload);
    if (code != HTTP_CODE_OK) {
        http.end();
        return API_NETWORK_ERROR;
    }
    
    DynamicJsonDocument doc(512);
    DeserializationError jsonErr = deserializeJson(doc, http.getString());
    http.end();
    
    if (jsonErr) return API_JSON_ERROR;
    
    if (doc["code"].as<int>() == 0 && doc.containsKey("data")) {
        _accessToken = doc["data"]["access_token"].as<String>();
        _refreshToken = doc["data"]["refresh_token"].as<String>();
        _tokenExpire = millis() + (doc["data"]["expires_in"].as<unsigned long>() * 1000);
        return API_OK;
    }
    return API_AUTH_FAILED;
}

std::vector<BiliAPI::VideoItem> BiliAPI::getPopularVideos(int page) {
    std::vector<VideoItem> videos;
    String endpoint = "/x/web-interface/popular?pn=" + String(page);
    
    DynamicJsonDocument doc(2048);
    if (_apiGet(endpoint, doc) != API_OK) return videos;
    
    JsonArray list = doc["data"]["list"].as<JsonArray>();
    for (JsonObject item : list) {
        VideoItem video;
        video.bvid = item["bvid"].as<String>();
        video.title = item["title"].as<String>();
        video.cover = item["pic"].as<String>();
        video.duration = item["duration"].as<int>();
        videos.push_back(video);
    }
    return videos;
}
void BiliAPI::setCredential(const String& user, const String& pass) {
    
    // 实现凭证存储逻辑（示例）
    _user = user;
    _pass = pass;
    // 可扩展为持久化存储
    // Preferences prefs;
    // prefs.begin("bili-auth");
    // prefs.putString("user", user);
    // prefs.putString("pass", pass);
    // prefs.end();
}