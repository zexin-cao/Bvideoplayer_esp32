
#include "SecureHTTP.h"

WiFiClientSecure SecureHTTP::client;
Preferences SecureHTTP::prefs;

bool SecureHTTP::begin() {
    client.setInsecure(); 
    return true;
}

bool SecureHTTP::checkAuth() {
    prefs.begin("bili-auth", true);
    String token = prefs.getString("refresh_token", "");
    prefs.end();
    return !token.isEmpty();
}

String SecureHTTP::sendRequest(const String& endpoint, const String& params) {
    HTTPClient http;
    String url = "https://api.bilibili.com" + endpoint;
    if(!params.isEmpty()) url += "?" + params;
    
    http.begin(client, url);
    http.GET();
    String response = http.getString();
    http.end();
    
    return response;
}