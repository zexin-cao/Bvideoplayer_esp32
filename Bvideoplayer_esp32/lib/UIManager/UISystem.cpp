#include "UISystem.h"
#include "../include/config.h"
#include <TJpg_Decoder.h>

// 初始化静态成员
TFT_eSPI* UISystem::_current_tft = nullptr;

UISystem::UISystem() {
    _current_tft = &tft;  // 将静态指针指向当前实例的tft
}

void UISystem::init(uint8_t cs, uint8_t dc, int8_t rst) {
    tft.init();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);

    // 配置JPEG解码回调
    TJpgDec.setJpgScale(1);
    TJpgDec.setCallback([](int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* data) {
        if (_current_tft) {
            _current_tft->pushImage(x, y, w, h, data);
        }
        return true;
    });
}

void UISystem::showQRLogin(const String& url) {
    currentViewState = VIEW_LOGIN;
    tft.fillScreen(TFT_BLACK);
    tft.drawString("Scan QR Code:", 10, 10);
    tft.drawString(url.substring(0, 20), 10, 40);
}

void UISystem::showMainMenu() {
    currentViewState = VIEW_MAIN_MENU;
    tft.fillScreen(TFT_BLUE);
    tft.drawString("Main Menu", 10, 10);
}

void UISystem::showVideoDetail(const BiliAPI::VideoItem& video) {
    currentViewState = VIEW_VIDEO_DETAIL;
    currentVideoData = video;
    tft.fillScreen(TFT_BLACK);
    tft.drawString(video.title.substring(0, 20), 10, 10);
    tft.drawString("Duration: " + String(video.duration) + "s", 10, 40);
}

UISystem::AppView UISystem::currentView() const {
    return currentViewState;
}

void UISystem::navigateNext() {
    switch (currentViewState) {
        case VIEW_LOGIN:
            showMainMenu();
            break;
        case VIEW_MAIN_MENU:
            showVideoDetail(currentVideoData);
            break;
        default: break;
    }
}

void UISystem::navigateBack() {
    switch (currentViewState) {
        case VIEW_VIDEO_DETAIL:
            showMainMenu();
            break;
        case VIEW_MAIN_MENU:
            showQRLogin("");
            break;
        default: break;
    }
}