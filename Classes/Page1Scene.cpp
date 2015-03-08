//
//  Page1Scene.cpp
//  inthesand
//
//  Created by Alex Gievsky on 07.03.15.
//
//

#include "Page1Scene.h"

#include "GameLayer.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

#define kSpeechEnabledTag 1
#define kSpeechDisabledTag 2

Page1Layer::~Page1Layer() {
    
}

Page1Layer::Page1Layer() {
    _back = nullptr;

    _menu = nullptr;
    _backBtn = nullptr;
    
    _top = nullptr;
    _bottom = nullptr;
}

Scene* Page1Layer::scene() {
    auto scene = Scene::create();
    auto gameLayer = Page1Layer::create();
    
    // add layer as a child to scene
    scene->addChild(gameLayer);
    
    // return the scene
    return scene;
    
}

bool Page1Layer::init() {
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
        _back->setColor({255, 199, 118});
        
        this->addChild(_back);
    }
    
    // top
    {
        _top = Sprite::create("abc_top.png");
        this->addChild(_top);
        
        _top->setAnchorPoint({0.5, 1});
        _top->setPosition({visibleSize.width * 0.5f, visibleSize.height});
    }
    
    // bottom
    {
        _bottom = Sprite::create("abc_bottom.png");
        this->addChild(_bottom);
        
        _bottom->setAnchorPoint({0.5, 0});
        _bottom->setPosition({visibleSize.width * 0.5f, 0});
    }
    
    // back btn
    {
        
        // menu
        _backBtn = MenuItemImage::create("btn_back.png", "btn_back_on.png", CC_CALLBACK_0(Page1Layer::onBackBtnPressed, this));
        _backBtn->setAnchorPoint({0, 0});
        _backBtn->setPosition({0, 0});
        
        _menu = Menu::create(_backBtn, nullptr);
        this->addChild(_menu);
        _menu->setPosition({0, -_backBtn->getContentSize().height});
        
        _menu->runAction(Sequence::create(DelayTime::create(0.15),
                                          EaseBackOut::create(MoveBy::create(0.1, {0, _backBtn->getContentSize().height})),
                                          NULL));

    }
    
    //CC_CALLBACK_1(InAppsMenuNode::onPurchaseBtnPressed, this));
    
    return true;
}

void Page1Layer::onCharacterBtnPressed(Ref *btn) {
    //int tag = (static_cast<Node*>(btn)->getTag());
}

void Page1Layer::onBackBtnPressed() {
    _menu->setEnabled(false);
    this->popOut();
}

void Page1Layer::popOut() {
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    
//    _text->runAction(FadeOut::create(0.1f));
//    _mount->runAction(ScaleTo::create(0.2, 1.5));
    
    this->runAction(Sequence::create(DelayTime::create(0.12),
                                     CallFunc::create([this]() {
        Director::getInstance()->replaceScene(GameLayer::scene());
    }), NULL));
}