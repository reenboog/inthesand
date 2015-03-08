//
//  Page2Scene.cpp
//  inthesand
//
//  Created by Alex Gievsky on 08.03.15.
//
//

#include "Page2Scene.h"

#include "GameLayer.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

using std::vector;
using std::string;

Page2Layer::~Page2Layer() {
    
}

Page2Layer::Page2Layer() {
    _back = nullptr;
    
    _menu = nullptr;
    _backBtn = nullptr;
    _nextBtn = nullptr;
    
    _top = nullptr;
    _bottom = nullptr;
    
    _lettersMenu = nullptr;
    
    _mount = nullptr;
    
    _currentSlide = 0;
}

Scene* Page2Layer::scene() {
    auto scene = Scene::create();
    auto gameLayer = Page2Layer::create();
    
    // add layer as a child to scene
    scene->addChild(gameLayer);
    
    // return the scene
    return scene;
    
}

bool Page2Layer::init() {
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
    
    // prev/next btn
    {
        
        // menu
        _backBtn = MenuItemImage::create("btn_back.png", "btn_back_on.png", CC_CALLBACK_0(Page2Layer::onBackBtnPressed, this));
        _backBtn->setAnchorPoint({0, 0});
        _backBtn->setPosition({0, 0});
        
        _nextBtn = MenuItemImage::create("btn_back.png", "btn_back_on.png", "btn_back_disabled.png", CC_CALLBACK_0(Page2Layer::onNextBtnPressed, this));
        _nextBtn->setAnchorPoint({0, 0});
        _nextBtn->setPosition({visibleSize.width, 0});
        _nextBtn->setScaleX(-1.0);
        _nextBtn->setVisible(false);
        _nextBtn->setPosition(_nextBtn->getPosition() - Point(0, _nextBtn->getContentSize().height));
        
        _menu = Menu::create(_backBtn, _nextBtn, nullptr);
        this->addChild(_menu);

        _menu->setPosition({0, -_backBtn->getContentSize().height});
        
        _menu->runAction(Sequence::create(DelayTime::create(0.15),
                                          EaseBackOut::create(MoveBy::create(0.1, {0, _backBtn->getContentSize().height})),
                                          NULL));
        
    }
    
    // mount
    {
        _mount = Sprite::create("reading_obj_mount.png");
        this->addChild(_mount);
        
        _mount->setAnchorPoint({1, 0});
        _mount->setPosition({0, _backBtn->getPosition().y + _backBtn->getContentSize().height});
        
        _mount->runAction(Sequence::create(DelayTime::create(0.1),
                                           MoveBy::create(0.1, {visibleSize.width, 0}),
                                           NULL));
    }
    
    {
        vector<int> lettersCodes;

        lettersCodes.push_back(0);
        lettersCodes.push_back(8);
        lettersCodes.push_back(1);
        lettersCodes.push_back(3);
        lettersCodes.push_back(6);
        lettersCodes.push_back(12);
        lettersCodes.push_back(13);
        lettersCodes.push_back(18);
        lettersCodes.push_back(19);
        
        string btnName = "btn_reading_small";
        
        Vector<MenuItem *> buttons;
        
        for(int i = 0; i < lettersCodes.size(); ++i) {
            if(i < 2) {
                btnName = "btn_reading_big";
            }
            
            MenuItemImage *btn = MenuItemImage::create(StringUtils::format("%s.png", btnName.c_str()),
                                                       StringUtils::format("%s_on.png", btnName.c_str()),
                                                       CC_CALLBACK_1(Page2Layer::onCharacterBtnPressed, this));
            
            buttons.pushBack(btn);
            
            Sprite *image = Sprite::create(StringUtils::format("ch_pair_%i.png", lettersCodes[i]));
            btn->addChild(image);
            btn->setTag(lettersCodes[i]);
            
            image->setPosition(btn->getContentSize().width * 0.5f, btn->getContentSize().height * 0.5f);
        }
        
        _lettersMenu = Menu::createWithArray(buttons);
        this->addChild(_lettersMenu);
        
        _lettersMenu->alignItemsInColumns(2, 4, 3, NULL);
        
        _lettersMenu->setPosition({visibleSize.width * 0.5f,
            _backBtn->getContentSize().height + _mount->getContentSize().height +
            (visibleSize.height - _top->getContentSize().height - (_backBtn->getContentSize().height + _mount->getContentSize().height)) / 2.0f});
        
        _lettersMenu->setPosition(_lettersMenu->getPosition() + Point(0, visibleSize.height * 0.5f));
        
        _lettersMenu->runAction(Sequence::create(DelayTime::create(0.12),
                                                 EaseBackOut::create(MoveBy::create(0.2, {0, -visibleSize.height * 0.5f})),
                                                 NULL));
    }
    
    return true;
}

void Page2Layer::onBackBtnPressed() {
    _currentSlide--;
    
    if(_currentSlide < 0) {
        _menu->setEnabled(false);
        this->popOut();
    } else {
        // show slide i
    }
    
}

void Page2Layer::onNextBtnPressed() {
    //
}

void Page2Layer::popOut() {
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    //    _text->runAction(FadeOut::create(0.1f));
    //    _mount->runAction(ScaleTo::create(0.2, 1.5));
    
    this->runAction(Sequence::create(DelayTime::create(0.12),
                                     CallFunc::create([this]() {
        Director::getInstance()->replaceScene(GameLayer::scene());
    }), NULL));
}

void Page2Layer::onCharacterBtnPressed(cocos2d::Ref *btn) {
    
}