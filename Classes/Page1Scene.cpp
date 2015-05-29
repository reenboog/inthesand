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

#include "MainMenu.h"

using namespace cocos2d;
using namespace CocosDenshion;

using std::vector;

#define kNumOfLetters 26

Page1Layer::~Page1Layer() {
    
}

Page1Layer::Page1Layer() {
    _back = nullptr;

    _menu = nullptr;
    //_backBtn = nullptr;
    _menuBtn = nullptr;
    
    _top = nullptr;
    _bottom = nullptr;
    
    _abcMenu = nullptr;
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
        //_back->setColor({255, 199, 118});

        _back->setColor({247, 241, 235});
        
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
//        _backBtn = MenuItemImage::create("btn_back.png", "btn_back_on.png", CC_CALLBACK_0(Page1Layer::onBackBtnPressed, this));
//        _backBtn->setAnchorPoint({0, 0});
//        _backBtn->setPosition({0, 0});
        _menuBtn = MenuItemImage::create("btn_menu.png", "btn_menu_on.png", CC_CALLBACK_0(Page1Layer::onMenuBtnPressed, this));
        _menuBtn->setAnchorPoint({0, 0});
        _menuBtn->setPosition({0, 0});
        
        _menu = Menu::create(_menuBtn, nullptr);
        this->addChild(_menu);
        _menu->setPosition({0, -_menuBtn->getContentSize().height});
        
        _menu->runAction(Sequence::create(DelayTime::create(0.15),
                                          EaseBackOut::create(MoveBy::create(0.1, {0, _menuBtn->getContentSize().height})),
                                          NULL));

    }
    
    // buttons
    Vector<MenuItem*> characters;
    
    // align items
    int numofRows = 9;
    int numOfColumns = 3;
    
    Sprite *btn = Sprite::create("btn_abc.png");
    
    Point initialPos = {0, numofRows * btn->getContentSize().height};
    
    for(int i = 0; i < kNumOfLetters; ++i) {
        MenuItemImage *btn = MenuItemImage::create("btn_abc.png", "btn_abc_on.png", CC_CALLBACK_1(Page1Layer::onCharacterBtnPressed, this));
        btn->setTag(i);
        btn->setAnchorPoint({0, 1});

        characters.pushBack(btn);
        
        Sprite *image = Sprite::create(StringUtils::format("abc_%i.png", i));
        btn->addChild(image);
        
        image->setPosition(btn->getContentSize().width * 0.5f, btn->getContentSize().height * 0.5f);
        
        btn->setPosition({(i % numOfColumns) * btn->getContentSize().width, initialPos.y - btn->getContentSize().height * (i / numOfColumns)});
    }
    
    _abcMenu = Menu::createWithArray(characters);
    this->addChild(_abcMenu);
    
    //_abcMenu->alignItemsInColumns(3, 3, 3, 3, 3, 3, 3, 3, 2, NULL);

    _abcMenu->setPosition({visibleSize.width * 0.5f - (btn->getContentSize().width * numOfColumns) / 2.0f,
        visibleSize.height * 0.55f - (btn->getContentSize().height * numofRows) / 2.0f + visibleSize.height});
    
    _abcMenu->runAction(Sequence::create(EaseBackOut::create(MoveBy::create(0.15, {0, -visibleSize.height})),
                                         NULL));
    
    return true;
}

void Page1Layer::onCharacterBtnPressed(Ref *btn) {
    int tag = (static_cast<Node*>(btn)->getTag());
    
    SimpleAudioEngine::getInstance()->stopAllEffects();
    SimpleAudioEngine::getInstance()->playEffect(StringUtils::format("abc_name_sound_word_%i.mp3", tag).c_str());
}

//void Page1Layer::onBackBtnPressed() {
//    _menu->setEnabled(false);
//    this->popOut();
//}

void Page1Layer::onMenuBtnPressed() {
    _menu->setEnabled(false);
    this->popOut();
}

void Page1Layer::popOut() {
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
//    _text->runAction(FadeOut::create(0.1f));
//    _mount->runAction(ScaleTo::create(0.2, 1.5));
    
    _abcMenu->runAction(Sequence::create(EaseBackOut::create(MoveBy::create(0.15, {0, visibleSize.height})),
                                         NULL));
    
    this->runAction(Sequence::create(DelayTime::create(0.12),
                                     CallFunc::create([this]() {
        Director::getInstance()->replaceScene(MainMenu::scene());
    }), NULL));
}