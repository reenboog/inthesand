//
//  Page0Scene.cpp
//  inthesand
//
//  Created by Alex Gievsky on 24.02.15.
//
//

#include "Page0Scene.h"
#include "GameLayer.h"
#include "MainMenu.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

#define kSpeechEnabledTag 1
#define kSpeechDisabledTag 2

Page0Layer::~Page0Layer() {
    
}

Page0Layer::Page0Layer() {
    _back = nullptr;
    _mount = nullptr;
    _top = nullptr;
    _text = nullptr;
    
    _menu = nullptr;
    _backBtn = nullptr;
    _speechBtn = nullptr;
}

Scene* Page0Layer::scene() {
    auto scene = Scene::create();
    auto gameLayer = Page0Layer::create();
    
    // add layer as a child to scene
    scene->addChild(gameLayer);
    
    // return the scene
    return scene;

}

bool Page0Layer::init() {
    if(!Layer::init()) {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
        
    // back
    {
        _back = Sprite::create("back.png");
        _back->setPosition({static_cast<float>(visibleSize.width / 2.0), static_cast<float>(visibleSize.height / 2.0)});
        
        Size backSize = _back->getContentSize();
        
        _back->setScale(visibleSize.width / backSize.width, visibleSize.height / backSize.height);
        _back->setColor({247, 241, 235});
        
        this->addChild(_back);
    }
    
    // top
    _top = LayerColor::create({255, 180, 50, 180});
    this->addChild(_top);
    
    _top->setPosition({0, visibleSize.height * 0.5f});
    _top->setScaleY(0);
    _top->setAnchorPoint({0, 0});
    
    _top->runAction(Sequence::create(DelayTime::create(0.06),
                                     ScaleTo::create(0.2, 1),
                                     NULL));
    
    // menu
    _backBtn = MenuItemImage::create("btn_back.png", "btn_back_on.png", CC_CALLBACK_0(Page0Layer::onBackBtnPressed, this));
    _backBtn->setAnchorPoint({0, 0});
    _backBtn->setPosition({0, 0});
    
    _speechBtn = MenuItemImage::create("btn_speech.png", "btn_speech_on.png", CC_CALLBACK_0(Page0Layer::onSpeechBtnPressed, this));
    _speechBtn->setAnchorPoint({1, 0});
    _speechBtn->setPosition({visibleSize.width, 0});
    _speechBtn->setTag(kSpeechDisabledTag);
    
    _menu = Menu::create(_backBtn, _speechBtn, nullptr);
    this->addChild(_menu);
    _menu->setPosition({0, -_backBtn->getContentSize().height});
    
    _menu->runAction(Sequence::create(DelayTime::create(0.15),
                                      EaseBackOut::create(MoveBy::create(0.1, {0, _backBtn->getContentSize().height})),
                                      NULL));
    
    // mount
    _mount = Sprite::create("page_0_mount.png");
    this->addChild(_mount);
    
    _mount->setPosition({visibleSize.width * 0.5f - _mount->getContentSize().width * 0.5f, visibleSize.height * 0.5f});
    
    _mount->runAction(EaseSineOut::create(MoveBy::create(0.1, {_mount->getContentSize().width * 0.5f, 0})));
    
    // text
    _text = Sprite::create("page_0_text.png");
    this->addChild(_text);
    
    _text->setPosition({visibleSize.width * 0.5f, visibleSize.height * 0.5f});
    
    _text->setOpacity(0);
    _text->runAction(Sequence::create(DelayTime::create(0.22),
                                      FadeIn::create(0.1),
                                      NULL));
    
    
    return true;
}

void Page0Layer::onBackBtnPressed() {
    _menu->setEnabled(false);
    this->popOut();
}

void Page0Layer::onSpeechBtnPressed() {
    if(_speechBtn->getTag() == kSpeechDisabledTag) {
        _speechBtn->setTag(kSpeechEnabledTag);
        SimpleAudioEngine::getInstance()->playBackgroundMusic("story.mp3");
    } else {
        _speechBtn->setTag(kSpeechDisabledTag);
        SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    }
}

void Page0Layer::popOut() {
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    
    _text->runAction(FadeOut::create(0.1f));
    _mount->runAction(ScaleTo::create(0.2, 1.5));
    
    this->runAction(Sequence::create(DelayTime::create(0.12),
                                     CallFunc::create([this]() {
        Director::getInstance()->replaceScene(MainMenu::scene());
    }), NULL));
}