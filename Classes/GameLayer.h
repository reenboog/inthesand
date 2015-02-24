#ifndef __GAME_LAYER_H__
#define __GAME_LAYER_H__

#include "cocos2d.h"

#include <vector>
// represents the main menu

class GameLayer: public cocos2d::Layer {
public:
    GameLayer();
    ~GameLayer();
    
    static cocos2d::Scene* scene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // touches
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event  *event);
    void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event  *event);
    
    void setCursor(int cursor);
    void onPageBtnPressed();
    
    void popOut();
    
    CREATE_FUNC(GameLayer);
private:
    void openPageScene(int idx);
private:
    cocos2d::Sprite *_back;
    
    // touches
    cocos2d::Point _touchPrevPos;
    int _cursorIndex;

    cocos2d::Sprite *_currentPage;
    cocos2d::MenuItemImage *_pageBtn;
    
    std::vector<cocos2d::Sprite*> _cursorsSprites;
    cocos2d::Label *_pageBtnTitle;
    
};

#endif // __HELLOWORLD_SCENE_H__
