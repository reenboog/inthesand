//
//  Page4Scene.cpp
//  inthesand
//
//  Created by Alex Gievsky on 11.03.15.
//
//

#include "Page4Scene.h"

#include "GameLayer.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

using std::vector;
using std::string;

#define kNumOfSlides 5

#define kBlackBtnTag 0
#define kRedBtnTag 4
#define kGreenBtnTag 1
#define kBlueBtnTag 2
#define kYellowBtnTag 3

#define kMaxAttempts 3

#define zBlockMenu 100

Page4Layer::~Page4Layer() {
    
}

Page4Layer::Page4Layer() {
    _back = nullptr;
    
    _menu = nullptr;
    _menuBtn = nullptr;
    _backBtn = nullptr;
    _nextBtn = nullptr;
    
    _top = nullptr;
    _bottom = nullptr;
    
    _mount = nullptr;
    
    _currentSlide = 0;
    _slideNode = nullptr;
    
    _colorsMenu = nullptr;
    
    _blockMenu = nullptr;
    _tutorialWasShown = false;
    
    _blackBtn = nullptr;
    
    _obj = nullptr;
}

Scene* Page4Layer::scene() {
    auto scene = Scene::create();
    auto gameLayer = Page4Layer::create();
    
    // add layer as a child to scene
    scene->addChild(gameLayer);
    
    // return the scene
    return scene;
    
}

bool Page4Layer::init() {
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
    
    // prev/next btn
    {
        
        // menu
        _menuBtn = MenuItemImage::create("btn_menu.png", "btn_menu_on.png", CC_CALLBACK_0(Page4Layer::onMenuBtnPressed, this));
        _menuBtn->setAnchorPoint({0, 0});
        _menuBtn->setPosition({0, 0});
        
        _backBtn = MenuItemImage::create("btn_back.png", "btn_back_on.png", "btn_back_disabled.png", CC_CALLBACK_0(Page4Layer::onBackBtnPressed, this));
        _backBtn->setAnchorPoint({0, 0});
        _backBtn->setPosition({_menuBtn->getContentSize().width, 0});
        _backBtn->setEnabled(false);

        _nextBtn = MenuItemImage::create("btn_back.png", "btn_back_on.png", "btn_back_disabled.png", CC_CALLBACK_0(Page4Layer::onNextBtnPressed, this));
        _nextBtn->setAnchorPoint({0, 0});
        _nextBtn->setPosition({visibleSize.width, 0});
        _nextBtn->setScaleX(-1.0);
        _nextBtn->setVisible(false);
        _nextBtn->setPosition(_nextBtn->getPosition() - Point(0, _nextBtn->getContentSize().height));
        
        _menu = Menu::create(_menuBtn, _backBtn, _nextBtn, nullptr);
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
        
        _mount->setAnchorPoint({0.5, 0});
        _mount->setPosition({-_mount->getContentSize().width * 0.5f, _backBtn->getPosition().y + _backBtn->getContentSize().height});
        
        _mount->runAction(Sequence::create(DelayTime::create(0.1),
                                           MoveTo::create(0.1, {visibleSize.width * 0.5f, _mount->getPositionY()}),
                                           NULL));
    }
    
    // slides
    {
        _slideNode = Node::create();
        _mount->addChild(_slideNode);
        
        _slideNode->setPosition({0, 0});
    }
    
    // colors buttons
    {
//        _colors.insert({kBlackBtnTag, {}});
//        _colors.insert({kRedBtnTag, {}});
//        _colors.insert({kGreenBtnTag, {}});
//        _colors.insert({kBlueBtnTag, {}});
//        _colors.insert({kYellowBtnTag, {}});
        
        _blackBtn = MenuItemImage::create("btn_black.png", "btn_black_on.png", CC_CALLBACK_1(Page4Layer::onColorBtnPressed, this));
        _blackBtn->setTag(kBlackBtnTag);
        
        MenuItemImage *red = MenuItemImage::create("btn_red.png", "btn_red_on.png", CC_CALLBACK_1(Page4Layer::onColorBtnPressed, this));
        red->setTag(kRedBtnTag);
        
        MenuItemImage *green = MenuItemImage::create("btn_green.png", "btn_green_on.png", CC_CALLBACK_1(Page4Layer::onColorBtnPressed, this));
        green->setTag(kGreenBtnTag);
        
        MenuItemImage *blue = MenuItemImage::create("btn_blue.png", "btn_blue_on.png", CC_CALLBACK_1(Page4Layer::onColorBtnPressed, this));
        blue->setTag(kBlueBtnTag);
        
        MenuItemImage *yellow = MenuItemImage::create("btn_yellow.png", "btn_yellow_on.png", CC_CALLBACK_1(Page4Layer::onColorBtnPressed, this));
        yellow->setTag(kYellowBtnTag);
        
        _colorsMenu = Menu::create(_blackBtn, red, green, blue, yellow, nullptr);
        this->addChild(_colorsMenu);
        
        _colorsMenu->alignItemsInColumns(3, 2, NULL);
        
        _colorsMenu->setPosition({visibleSize.width * 0.5f,
            _backBtn->getContentSize().height + _mount->getContentSize().height +
            (visibleSize.height - _top->getContentSize().height - (_backBtn->getContentSize().height + _mount->getContentSize().height)) / 2.0f});
        
        _colorsMenu->setPosition(_colorsMenu->getPosition() + Point(0, visibleSize.height * 0.5f));
        
        _colorsMenu->runAction(Sequence::create(DelayTime::create(0.12),
                                                EaseBackOut::create(MoveBy::create(0.2, {0, -visibleSize.height * 0.5f})),
                                                CallFunc::create([this]() {
            this->openSlide(0);
        }), NULL));

    }
    
    return true;
}

void Page4Layer::onMenuBtnPressed() {
    this->popOut();
}

void Page4Layer::onBackBtnPressed() {
//    SimpleAudioEngine::getInstance()->stopAllEffects();
//    
//    _currentSlide--;
//    
//    if(_currentSlide < 0) {
//        _menu->setEnabled(false);
//        this->popOut();
//    } else {
//        this->openSlide(_currentSlide);
//    }
    
    SimpleAudioEngine::getInstance()->stopAllEffects();
    
    _currentSlide--;
    
    _nextBtn->setEnabled(true);
    
    if(_currentSlide == 0) {
        _backBtn->setEnabled(false);
    }
    
    this->openSlide(_currentSlide);
}

void Page4Layer::onNextBtnPressed() {
    SimpleAudioEngine::getInstance()->stopAllEffects();
    
    _currentSlide++;
    
    if(_currentSlide == kNumOfSlides - 1) {
        _nextBtn->setEnabled(false);
    }
    
    this->openSlide(_currentSlide);
}

void Page4Layer::popOut() {
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    //    _text->runAction(FadeOut::create(0.1f));
    //    _mount->runAction(ScaleTo::create(0.2, 1.5));
    
    this->runAction(Sequence::create(DelayTime::create(0.12),
                                     CallFunc::create([this]() {
        Director::getInstance()->replaceScene(GameLayer::scene());
    }), NULL));
}

void Page4Layer::onColorBtnPressed(cocos2d::Ref *btn) {
    int tag = (static_cast<Node*>(btn)->getTag());
    
    SimpleAudioEngine::getInstance()->stopAllEffects();
    
    SimpleAudioEngine::getInstance()->playEffect(StringUtils::format("color_name_%i.mp3", tag).c_str());
    
    _obj->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=]() {
        if(tag == _obj->getTag()) {
            SimpleAudioEngine::getInstance()->playEffect("great.mp3");
            
            if(_nextBtn->isVisible() == false) {
                _nextBtn->setVisible(true);
                _nextBtn->runAction(MoveBy::create(0.1, {0, _nextBtn->getContentSize().height}));
            }
            
            if(_obj->getChildrenCount() == 0) {
                Sprite *colored = Sprite::create(StringUtils::format("color_slide_%i_colored.png", _currentSlide));
                _obj->addChild(colored);
                
                colored->setOpacity(0);
                colored->setPosition({_obj->getContentSize().width * 0.5f, _obj->getContentSize().height * 0.5f});
                
                colored->runAction(FadeIn::create(0.2));
            }
            
            // fill in th eobject
        } else {
            SimpleAudioEngine::getInstance()->playEffect("try.mp3");
        }
    }), NULL));
}

void Page4Layer::openSlide(int slide) {
    SimpleAudioEngine::getInstance()->stopAllEffects();
    
    _slideNode->removeAllChildren();
    
    if(slide == 0) {
        _backBtn->setEnabled(false);
    } else {
        _backBtn->setEnabled(true);
    }
    
    switch(slide) {
        case 0:
            this->openSlide0();
            break;
        case 1:
            this->openSlide1();
            break;
        case 2:
            this->openSlide2();
            break;
        case 3:
            this->openSlide3();
            break;
        case 4:
            this->openSlide4();
            break;
    }
}

void Page4Layer::openSlide0() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    this->block();
    
    _obj = Sprite::create("color_slide_0.png");
    
    _slideNode->addChild(_obj);

    _obj->setPosition({_mount->getContentSize().width * 0.5f, _mount->getContentSize().height * 0.5f});
    _obj->setScale(0);
    _obj->setTag(kBlackBtnTag);
    
    _obj->runAction(Sequence::create(EaseBackOut::create(ScaleTo::create(0.15, 1)),
                                    CallFunc::create([=]() {
        SimpleAudioEngine::getInstance()->playEffect("color_0.mp3");
        
        if(_tutorialWasShown == false) {
            this->block();
            
            Sprite *hand = Sprite::create("hand.png");
            this->addChild(hand);
            
            hand->setAnchorPoint({0, 1});
            hand->setPosition({visibleSize.width * 0.7f, visibleSize.height * 0.3f});
            hand->setScale(0);
            
            Point bPos = _colorsMenu->getPosition() + _blackBtn->getPosition();
            
            hand->runAction(Sequence::create(DelayTime::create(1.6),
                                             EaseBackOut::create(ScaleTo::create(0.2, 1)),
                                             DelayTime::create(0.5),
                                             MoveTo::create(1, bPos),
                                             DelayTime::create(0.2),
                                             RotateBy::create(0.1, -30),
                                             ScaleTo::create(0.1, 0),
                                             CallFunc::create([=]() {
                _tutorialWasShown = true;
                this->unblock();
                hand->removeFromParent();
            }), NULL));
        } else {
            this->unblock();
        }
    }), NULL));
}

void Page4Layer::openSlide1() {
    this->block();
    
    _obj = Sprite::create("color_slide_1.png");
    
    _slideNode->addChild(_obj);
    
    _obj->setPosition({_mount->getContentSize().width * 0.5f, _mount->getContentSize().height * 0.5f});
    _obj->setScale(0);
    _obj->setTag(kGreenBtnTag);
    
    _obj->runAction(Sequence::create(EaseBackOut::create(ScaleTo::create(0.15, 1)),
                                     CallFunc::create([=]() {
        SimpleAudioEngine::getInstance()->playEffect("color_1.mp3");
        
        this->unblock();
    }), NULL));

}

void Page4Layer::openSlide2() {
    this->block();
    
    _obj = Sprite::create("color_slide_2.png");
    
    _slideNode->addChild(_obj);
    
    _obj->setPosition({_mount->getContentSize().width * 0.5f, _mount->getContentSize().height * 0.5f});
    _obj->setScale(0);
    _obj->setTag(kBlueBtnTag);
    
    _obj->runAction(Sequence::create(EaseBackOut::create(ScaleTo::create(0.15, 1)),
                                     CallFunc::create([=]() {
        SimpleAudioEngine::getInstance()->playEffect("color_2.mp3");
        
        this->unblock();
    }), NULL));

}

void Page4Layer::openSlide3() {
    this->block();
    
    _obj = Sprite::create("color_slide_3.png");
    
    _slideNode->addChild(_obj);
    
    _obj->setPosition({_mount->getContentSize().width * 0.5f, _mount->getContentSize().height * 0.5f});
    _obj->setScale(0);
    _obj->setTag(kYellowBtnTag);
    
    _obj->runAction(Sequence::create(EaseBackOut::create(ScaleTo::create(0.15, 1)),
                                     CallFunc::create([=]() {
        SimpleAudioEngine::getInstance()->playEffect("color_3.mp3");
        
        this->unblock();
    }), NULL));

}

void Page4Layer::openSlide4() {
    this->block();
    
    _obj = Sprite::create("color_slide_4.png");
    
    _slideNode->addChild(_obj);
    
    _obj->setPosition({_mount->getContentSize().width * 0.5f, _mount->getContentSize().height * 0.5f});
    _obj->setScale(0);
    _obj->setTag(kRedBtnTag);
    
    _obj->runAction(Sequence::create(EaseBackOut::create(ScaleTo::create(0.15, 1)),
                                     CallFunc::create([=]() {
        SimpleAudioEngine::getInstance()->playEffect("color_4.mp3");
        
        this->unblock();
    }), NULL));
}

void Page4Layer::block() {
    if(_blockMenu != nullptr) {
        return;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    MenuItemImage *btn = MenuItemImage::create("block.png", "block.png", [](Ref *){});
    btn->setScale(visibleSize.width / btn->getContentSize().width, visibleSize.height / btn->getContentSize().height);
    btn->setOpacity(0);
    
    _blockMenu = Menu::create(btn, nullptr);
    _blockMenu->setPosition({visibleSize.width * 0.5f, visibleSize.height * 0.5f});
    
    this->addChild(_blockMenu, zBlockMenu);
}

void Page4Layer::unblock() {
    if(_blockMenu != nullptr) {
        _blockMenu->removeFromParent();
        _blockMenu = nullptr;
    }
}