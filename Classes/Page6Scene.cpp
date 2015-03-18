//
//  Page6Scene.cpp
//  inthesand
//
//  Created by Alex Gievsky on 17.03.15.
//
//

#include "Page6Scene.h"

#include "GameLayer.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

using std::vector;
using std::string;

#define zBlockMenu 100

#define kBoxImgTag 11

#define kNumfOfLetters 26

Page6Layer::~Page6Layer() {
    
}

Page6Layer::Page6Layer() {
    _back = nullptr;
    
    _menu = nullptr;
    _backBtn = nullptr;
    
    _top = nullptr;
    _bottom = nullptr;
    
    _currentSlide = 0;
    
    _blockMenu = nullptr;
    
    _lettersMenu = nullptr;
    
    _currentLetter = nullptr;
    
    _bBtn = nullptr;
}

Scene* Page6Layer::scene() {
    auto scene = Scene::create();
    auto gameLayer = Page6Layer::create();
    
    // add layer as a child to scene
    scene->addChild(gameLayer);
    
    // return the scene
    return scene;
    
}

bool Page6Layer::init() {
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
        _backBtn = MenuItemImage::create("btn_back.png", "btn_back_on.png", CC_CALLBACK_0(Page6Layer::onBackBtnPressed, this));
        _backBtn->setAnchorPoint({0, 0});
        _backBtn->setPosition({0, 0});
        
        _replayBtn = MenuItemImage::create("btn_replay.png", "btn_replay_on.png", CC_CALLBACK_0(Page6Layer::onReplayBtnPressed, this));
        _replayBtn->setAnchorPoint({1, 0});
        _replayBtn->setPosition({visibleSize.width, 0});
        _replayBtn->setPosition(_replayBtn->getPosition() - Point(0, _replayBtn->getContentSize().height));

        
        _menu = Menu::create(_backBtn, _replayBtn, nullptr);
        this->addChild(_menu);
        
        _menu->setPosition({0, -_backBtn->getContentSize().height});
        
        _menu->runAction(Sequence::create(DelayTime::create(0.15),
                                          EaseBackOut::create(MoveBy::create(0.1, {0, _backBtn->getContentSize().height})),
                                          NULL));
        
    }
    
    
    // words
    _words.push_back({1, 0, 6}); // bag
    _words.push_back({17, 4, 3}); // red
    _words.push_back({1, 8, 13}); // bin
    _words.push_back({18, 0, 13, 3}); // sand
    _words.push_back({12, 0, 19}); // mat
    _words.push_back({12, 8, 19, 19}); // mitt
    _words.push_back({3, 0, 3}); // dad
    _words.push_back({6, 17, 4, 4, 13}); // green
    _words.push_back({24, 4, 11, 11, 14, 22}); // yellow
    _words.push_back({1, 11, 0, 2, 10}); // red
    
    // letters menu
    
    // touches
    {
        auto touchesListener = EventListenerTouchOneByOne::create();
        
        touchesListener->onTouchBegan = CC_CALLBACK_2(Page6Layer::onTouchBegan, this);
        touchesListener->onTouchEnded = CC_CALLBACK_2(Page6Layer::onTouchEnded, this);
        touchesListener->onTouchMoved = CC_CALLBACK_2(Page6Layer::onTouchMoved, this);
        touchesListener->onTouchCancelled = CC_CALLBACK_2(Page6Layer::onTouchCancelled, this);
        
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchesListener, this);
    }
    
    {
        Vector<MenuItem*> buttons;

        for(int i = 0; i < kNumfOfLetters; ++i) {
            MenuItemImage *btn = MenuItemImage::create("crush_letter_mount.png", "crush_letter_mount.png", [](Ref *){});
            btn->setTag(i);
            btn->setEnabled(false);
            
            Sprite *img = Sprite::create(StringUtils::format("ch_small_%i.png", i));
            
            btn->addChild(img);
            
            img->setPosition({btn->getContentSize().width * 0.5f, btn->getContentSize().height * 0.5f});
            
            buttons.pushBack(btn);
            _letters.push_back(btn);
            
            if(i == 1) {
                _bBtn = btn;
            }
        }
        
        _lettersMenu = Menu::createWithArray(buttons);
        _lettersMenu->alignItemsInColumns(6, 6, 6, 6, 2, NULL);
        
        Sprite *sqr = Sprite::create("drag_square.png");
        
        this->addChild(_lettersMenu);
        _lettersMenu->setPosition({visibleSize.width * 0.5f, _backBtn->getContentSize().height + sqr->getContentSize().height +
            (visibleSize.height - _top->getContentSize().height - (_backBtn->getContentSize().height + sqr->getContentSize().height)) / 2.0f});
        
        _lettersMenu->setPosition(_lettersMenu->getPosition() + Point(0, visibleSize.height * 0.5f));

        _lettersMenu->runAction(Sequence::create(DelayTime::create(0.12),
                                                 EaseBackOut::create(MoveBy::create(0.2, {0, -visibleSize.height * 0.5f})),
                                                 CallFunc::create([this]() {
                                                    this->showTutorial();
                                                 }),
                                                 NULL));
    }
    
    return true;
}

void Page6Layer::onBackBtnPressed() {
    SimpleAudioEngine::getInstance()->stopAllEffects();
    
    this->popOut();
}

void Page6Layer::popUpANewWord() {
    // remove previous letters first
    for(int i = 0; i < _boxes.size(); ++i) {
        _boxes[i]->removeFromParent();
    }
    
    _boxes.clear();
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    if(_currentSlide == _words.size()) {
        // show 'retry'
        
        _replayBtn->setEnabled(false);
        _replayBtn->runAction(Sequence::create(DelayTime::create(1),
                                               MoveBy::create(0.1, {0, _replayBtn->getContentSize().height}),
                                               CallFunc::create([=]() {
            SimpleAudioEngine::getInstance()->playEffect("try.mp3");
            _replayBtn->setEnabled(true);
        }), NULL));
    } else {
        int numOfLetters = _words[_currentSlide].size();
        
        for(int i = 0; i < numOfLetters; ++i) {
            Sprite *sqr = Sprite::create("drag_square.png");
            this->addChild(sqr);
            
            sqr->setAnchorPoint({0, 0});
            sqr->setPosition({visibleSize.width * 0.5f - numOfLetters * sqr->getContentSize().width * 0.5f + i * sqr->getContentSize().width, _backBtn->getContentSize().height});
            sqr->setTag(_words[_currentSlide][i]);
            
            Sprite *img = Sprite::create(StringUtils::format("ch_small_%i.png", _words[_currentSlide][i]));
            sqr->addChild(img);
            
            img->setPosition({sqr->getContentSize().width * 0.5f, sqr->getContentSize().height * 0.5f});
            img->setTag(kBoxImgTag);
            img->setOpacity(50);
            
            _boxes.push_back(sqr);
            
        }
    }
}

void Page6Layer::popOut() {
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    //    _text->runAction(FadeOut::create(0.1f));
    //    _mount->runAction(ScaleTo::create(0.2, 1.5));
    
    this->runAction(Sequence::create(DelayTime::create(0.12),
                                     CallFunc::create([this]() {
        Director::getInstance()->replaceScene(GameLayer::scene());
    }), NULL));
}

void Page6Layer::block() {
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

void Page6Layer::unblock() {
    if(_blockMenu != nullptr) {
        _blockMenu->removeFromParent();
        _blockMenu = nullptr;
    }
}

#pragma mark - Touches

bool Page6Layer::onTouchBegan(Touch *touch, Event *event) {
    // Point pos = touch->getLocation();

    return true;
}

void Page6Layer::onTouchMoved(Touch *touch, Event  *event) {
    Point pos = touch->getLocation();
    
    for(int i = 0; i < _letters.size(); ++i) {
        Point localPos = _lettersMenu->convertToNodeSpace(pos);
        if(_letters[i]->getBoundingBox().containsPoint(localPos) && _currentLetter == nullptr) {
            _currentLetter = Sprite::create(StringUtils::format("ch_small_%i.png", _letters[i]->getTag()));
            this->addChild(_currentLetter);
            
            _currentLetter->setPosition(pos);
            _currentLetter->setTag(_letters[i]->getTag());

            break;
        }
    }

    if(_currentLetter != nullptr) {
        _currentLetter->setPosition(pos);
    }
}

void Page6Layer::onTouchEnded(Touch *touch, Event *event) {
    Point pos = touch->getLocation();
    
    if(_currentLetter != nullptr) {
        for(int i = 0; i < _boxes.size(); ++i) {
            if(_boxes[i]->getBoundingBox().containsPoint(pos) && _boxes[i]->getTag() == _currentLetter->getTag()) {
                _boxes[i]->getChildByTag(kBoxImgTag)->setOpacity(255);
                
                break;
            }
        }
    }
    
    bool wordComplete = true;
    
    for(int i = 0; i < _boxes.size(); ++i) {
        if(_boxes[i]->getChildByTag(kBoxImgTag)->getOpacity() != 255) {
            wordComplete = false;
            break;
        }
    }
    
    if(wordComplete == true) {
        // play a sound effect
        SimpleAudioEngine::getInstance()->playEffect("great.mp3");
        _currentSlide++;
        this->popUpANewWord();
    }
    
    if(_currentLetter != nullptr) {
        _currentLetter->removeFromParent();
        _currentLetter = nullptr;
    }
}

void Page6Layer::onTouchCancelled(Touch *touch, Event  *event) {
    this->onTouchEnded(touch, event);
    //_touchesEnabled = true;
}

void Page6Layer::onReplayBtnPressed() {
    _currentSlide = 0;
    
    _replayBtn->setEnabled(false);
    _replayBtn->runAction(MoveBy::create(0.1, {0, -_replayBtn->getContentSize().height}));
    
    this->popUpANewWord();
}

void Page6Layer::showTutorial() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    this->block();
    this->popUpANewWord();
    
    Sprite *hand = Sprite::create("hand.png");
    this->addChild(hand);
    
    hand->setAnchorPoint({0, 1});
    hand->setPosition({visibleSize.width * 0.7f, visibleSize.height * 0.3f});
    hand->setScale(0);
    
    Point bPos = _lettersMenu->getPosition() + _bBtn->getPosition();
    Point bEndPos = _boxes[0]->getPosition() + Point(_boxes[0]->getContentSize().width * 0.5f, _boxes[0]->getContentSize().height * 0.5f);
    
    hand->runAction(Sequence::create(DelayTime::create(0.6),
                                     EaseBackOut::create(ScaleTo::create(0.2, 1)),
                                     DelayTime::create(0.5),
                                     MoveTo::create(1, bPos),
                                     DelayTime::create(0.2),
                                     CallFunc::create([=]() {
                                        Sprite *b = Sprite::create("ch_small_1.png");
                                        
                                        hand->addChild(b);
                                        b->setPosition({0, hand->getContentSize().height});
                                     }),
                                     MoveTo::create(1, bEndPos),
                                     DelayTime::create(0.3),
                                     ScaleTo::create(0.1, 0),
                                     CallFunc::create([=]() {
        this->unblock();
        hand->removeFromParent();
    }), NULL));
}