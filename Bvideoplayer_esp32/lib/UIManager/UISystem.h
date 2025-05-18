#pragma once
#include <TFT_eSPI.h>
#include <vector>
#include "BiliAPI.h"

class UISystem {
public:
    UISystem();  // 显式声明构造函数
    
    enum AppView {
        VIEW_LOGIN,
        VIEW_MAIN_MENU,
        VIEW_VIDEO_DETAIL
    };

    void init(uint8_t cs = 0, uint8_t dc = 0, int8_t rst = -1);
    void showQRLogin(const String& url);
    void showMainMenu();
    void showVideoDetail(const BiliAPI::VideoItem& video);
    AppView currentView() const;
    void navigateNext();
    void navigateBack();

private:
    TFT_eSPI tft;
    AppView currentViewState = VIEW_LOGIN;
    BiliAPI::VideoItem currentVideoData;

    static TFT_eSPI* _current_tft;  // 静态指针用于JPEG回调
};