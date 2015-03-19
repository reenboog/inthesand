//
//  Page7Scene.cpp
//  inthesand
//
//  Created by Alex Gievsky on 18.03.15.
//
//

#include "Page7Scene.h"

#include "GameLayer.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

using std::vector;
using std::string;
using std::pair;

#define kNumOfSlides 8

Page7Layer::~Page7Layer() {
    
}

Page7Layer::Page7Layer() {
    _back = nullptr;
    
    _menu = nullptr;

    _backBtn = nullptr;
    _nextBtn = nullptr;
    _muteBtn = nullptr;
    
    _top = nullptr;
    _bottom = nullptr;
    
    _muted = nullptr;
    
    _mount = nullptr;
    
    _string = nullptr;
    _animatedString = nullptr;
    
    _currentSlide = 0;

    _slideObjectsNode = nullptr;
    _slideTextsNode = nullptr;
}

Scene* Page7Layer::scene() {
    auto scene = Scene::create();
    auto gameLayer = Page7Layer::create();
    
    // add layer as a child to scene
    scene->addChild(gameLayer);
    
    // return the scene
    return scene;
    
}

bool Page7Layer::init() {
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
        _backBtn = MenuItemImage::create("btn_back.png", "btn_back_on.png", CC_CALLBACK_0(Page7Layer::onBackBtnPressed, this));
        _backBtn->setAnchorPoint({0, 0});
        _backBtn->setPosition({0, 0});
        
        _nextBtn = MenuItemImage::create("btn_back.png", "btn_back_on.png", "btn_back_disabled.png", CC_CALLBACK_0(Page7Layer::onNextBtnPressed, this));
        _nextBtn->setAnchorPoint({0, 0});
        _nextBtn->setPosition({visibleSize.width, 0});
        _nextBtn->setScaleX(-1.0);
        
        _muteBtn = MenuItemImage::create("btn_mute.png", "btn_mute_on.png", CC_CALLBACK_0(Page7Layer::onMuteBtnPressed, this));
        _muteBtn->setAnchorPoint({1, 0});
        _muteBtn->setPosition({visibleSize.width - _nextBtn->getContentSize().width, 0});
        
        _muted = Sprite::create("muted.png");
        _muteBtn->addChild(_muted);
        
        _muted->setPosition({_muteBtn->getContentSize().width * 0.5f, _muteBtn->getContentSize().height * 0.5f});
        _muted->setVisible(false);
        
        _menu = Menu::create(_backBtn, _muteBtn, _nextBtn, nullptr);
        this->addChild(_menu);
        
        _menu->setPosition({0, -_backBtn->getContentSize().height});
        
        _menu->runAction(Sequence::create(DelayTime::create(0.15),
                                          EaseBackOut::create(MoveBy::create(0.1, {0, _backBtn->getContentSize().height})),
                                          NULL));
        
    }
    
    // mount
    {
        _mount = Sprite::create("story_obj_mount.png");
        this->addChild(_mount);
        
        _mount->setAnchorPoint({0.5, 0});
        _mount->setPosition({-_mount->getContentSize().width * 0.5f, _backBtn->getPosition().y + _backBtn->getContentSize().height});
        
        _mount->runAction(Sequence::create(DelayTime::create(0.1),
                                           MoveTo::create(0.1, {visibleSize.width * 0.5f, _mount->getPositionY()}),
                                           CallFunc::create([=]() {
            this->openSlide(0);
        }), NULL));
    }
    
    // slides objects
    {
        _slideObjectsNode = Node::create();
        _mount->addChild(_slideObjectsNode);
        
        _slideObjectsNode->setPosition({0, 0});
    }
    
    // slides
    {
        _slideObjectsNode = Node::create();
        _mount->addChild(_slideObjectsNode);
        
        _slideObjectsNode->setPosition({0, 0});
    }
    
    // slides
    {
        _slideTextsNode = Node::create();
        _mount->addChild(_slideTextsNode);
        
        _slideTextsNode->setPosition({0, 0});
    }
    
    /*
    
     Pam and Dan sit in the sand.
     Sam and Dad sit in the sand.
     They dig.
     Look! Pam's blue bag is in the sand.
     Sam's red mitt is in the sand.
     A yellow mat is in the sand.
     A green bin is in the sand.
     Look! A black ant is in the sand too!
     
    */
    
    return true;
}

void Page7Layer::onBackBtnPressed() {
    SimpleAudioEngine::getInstance()->stopAllEffects();
    
    _currentSlide--;
    
    _nextBtn->setEnabled(true);
    
    if(_currentSlide < 0) {
        _menu->setEnabled(false);
        this->popOut();
    } else {
        this->openSlide(_currentSlide);
    }
}

void Page7Layer::onNextBtnPressed() {
    SimpleAudioEngine::getInstance()->stopAllEffects();
    
    _currentSlide++;
    
    if(_currentSlide == kNumOfSlides - 1) {
        _nextBtn->setEnabled(false);
    }
    
    this->openSlide(_currentSlide);
}

void Page7Layer::popOut() {
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    //    _text->runAction(FadeOut::create(0.1f));
    //    _mount->runAction(ScaleTo::create(0.2, 1.5));
    
    this->runAction(Sequence::create(DelayTime::create(0.12),
                                     CallFunc::create([this]() {
        Director::getInstance()->replaceScene(GameLayer::scene());
    }), NULL));
}

void Page7Layer::onCharacterBtnPressed(cocos2d::Ref *btn) {
    int tag = (static_cast<Node*>(btn)->getTag());
    
    SimpleAudioEngine::getInstance()->stopAllEffects();
    SimpleAudioEngine::getInstance()->playEffect(StringUtils::format("abc_sound_%i.mp3", tag).c_str());
    
}

void Page7Layer::openSlide(int slide) {
    SimpleAudioEngine::getInstance()->stopAllEffects();
    
    _slideObjectsNode->removeAllChildren();
    _slideTextsNode->removeAllChildren();

    //_texts.clear();
    
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
        case 5:
            this->openSlide5();
            break;
        case 6:
            this->openSlide6();
            break;
        case 7:
            this->openSlide7();
            break;
    }
}

void Page7Layer::openSlide0() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    // str
    
    string str = "Pam and Dan sit in the sand.";

    _string = Label::createWithBMFont("arial_rounded_mt_bold_55.fnt", str);
    _slideTextsNode->addChild(_string);
    
    _string->setMaxLineWidth(visibleSize.width * 0.75);
    _string->setHorizontalAlignment(TextHAlignment::CENTER);
    _string->setPosition({_mount->getContentSize().width * 0.5f, _mount->getContentSize().height * 0.28f});
    //_string->setScale(0);
    
    _string->setVisible(this->isMuted());
    
    _animatedString = Label::createWithBMFont("arial_rounded_mt_bold_55.fnt", str);
    _slideTextsNode->addChild(_animatedString);
    
    _animatedString->setMaxLineWidth(visibleSize.width * 0.75);
    _animatedString->setHorizontalAlignment(TextHAlignment::CENTER);
    _animatedString->setPosition(_string->getPosition());
    //_animatedString->setScale(0);
    
    _animatedString->setVisible(!this->isMuted());
    
//    _string->runAction(Sequence::create(DelayTime::create(0.3),
//                                        EaseBackOut::create(ScaleTo::create(0.15, 1)),
//                                        NULL));
//    
//    _animatedString->runAction(Sequence::create(DelayTime::create(0.3),
//                                        EaseBackOut::create(ScaleTo::create(0.15, 1)),
//                                        NULL));
    
    //
    
    
    Sprite *obj = Sprite::create("story_0.png");
    
    _slideObjectsNode->addChild(obj);
    
    obj->setPosition({_mount->getContentSize().width * 0.5f, _mount->getContentSize().height * 0.65f});
    obj->setScale(0);
    
    obj->runAction(Sequence::create(DelayTime::create(0.2),
                                    EaseBackOut::create(ScaleTo::create(0.15, 1)),
                                    DelayTime::create(0.1),
                                    CallFunc::create([=]() {
        SimpleAudioEngine::getInstance()->playEffect("story_0.mp3");
        
        // animated string
        vector<pair<float, pair<int, int> > > text;
        
        text.push_back({0.478, {0, 2}});
        text.push_back({1.06, {4, 6}});
        text.push_back({1.4, {8, 10}});
        text.push_back({2.257, {12, 14}});
        text.push_back({2.953, {16, 17}});
        text.push_back({3.1, {19, 21}});
        text.push_back({3.372, {23, 27}});
        
        for(int i = 0; i < text.size(); ++i) {
            float delay = text[i].first;
            
            int chStart = text[i].second.first;
            int chEnd = text[i].second.second;
            
            for(int j = chStart; j <= chEnd; ++j) {
                Sprite *character = _animatedString->getLetter(j);
                
                Color3B charPrevColor = character->getColor();
                
                character->runAction(Sequence::create(DelayTime::create(delay),
                                                      TintTo::create(0.05, 50, 50, 50),
                                                      DelayTime::create(0.3),
                                                      TintTo::create(0.05, charPrevColor.r, charPrevColor.g, charPrevColor.b),
                                                      NULL));
            }
        }
    }), NULL));
}

void Page7Layer::openSlide1() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    // str
    
    string str = "Sam and Dad sit in the sand.";
    
    _string = Label::createWithBMFont("arial_rounded_mt_bold_55.fnt", str);
    _slideTextsNode->addChild(_string);
    
    _string->setMaxLineWidth(visibleSize.width * 0.75);
    _string->setHorizontalAlignment(TextHAlignment::CENTER);
    _string->setPosition({_mount->getContentSize().width * 0.5f, _mount->getContentSize().height * 0.28f});
    //_string->setScale(0);
    
    _string->setVisible(this->isMuted());
    
    _animatedString = Label::createWithBMFont("arial_rounded_mt_bold_55.fnt", str);
    _slideTextsNode->addChild(_animatedString);
    
    _animatedString->setMaxLineWidth(visibleSize.width * 0.75);
    _animatedString->setHorizontalAlignment(TextHAlignment::CENTER);
    _animatedString->setPosition(_string->getPosition());
    //_animatedString->setScale(0);
    
    _animatedString->setVisible(!this->isMuted());
    
    //    _string->runAction(Sequence::create(DelayTime::create(0.3),
    //                                        EaseBackOut::create(ScaleTo::create(0.15, 1)),
    //                                        NULL));
    //
    //    _animatedString->runAction(Sequence::create(DelayTime::create(0.3),
    //                                        EaseBackOut::create(ScaleTo::create(0.15, 1)),
    //                                        NULL));
    
    //
    
    
    Sprite *obj = Sprite::create("story_1.png");
    
    _slideObjectsNode->addChild(obj);
    
    obj->setPosition({_mount->getContentSize().width * 0.5f, _mount->getContentSize().height * 0.65f});
    obj->setScale(0);
    
    obj->runAction(Sequence::create(DelayTime::create(0.2),
                                    EaseBackOut::create(ScaleTo::create(0.15, 1)),
                                    DelayTime::create(0.1),
                                    CallFunc::create([=]() {
        SimpleAudioEngine::getInstance()->playEffect("story_1.mp3");
        
        // animated string
        vector<pair<float, pair<int, int> > > text;
        
        text.push_back({0.478, {0, 2}});
        text.push_back({1.06, {4, 6}});
        text.push_back({1.4, {8, 10}});
        text.push_back({2.157, {12, 14}});
        text.push_back({2.553, {16, 17}});
        text.push_back({2.8, {19, 21}});
        text.push_back({2.9, {23, 27}});
        
        for(int i = 0; i < text.size(); ++i) {
            float delay = text[i].first;
            
            int chStart = text[i].second.first;
            int chEnd = text[i].second.second;
            
            for(int j = chStart; j <= chEnd; ++j) {
                Sprite *character = _animatedString->getLetter(j);
                
                Color3B charPrevColor = character->getColor();
                
                character->runAction(Sequence::create(DelayTime::create(delay),
                                                      TintTo::create(0.05, 50, 50, 50),
                                                      DelayTime::create(0.3),
                                                      TintTo::create(0.05, charPrevColor.r, charPrevColor.g, charPrevColor.b),
                                                      NULL));
            }
        }
    }), NULL));
}

void Page7Layer::openSlide2() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    // str
    
    string str = "They dig.";
    
    _string = Label::createWithBMFont("arial_rounded_mt_bold_55.fnt", str);
    _slideTextsNode->addChild(_string);
    
    _string->setMaxLineWidth(visibleSize.width * 0.75);
    _string->setHorizontalAlignment(TextHAlignment::CENTER);
    _string->setPosition({_mount->getContentSize().width * 0.5f, _mount->getContentSize().height * 0.28f});
    //_string->setScale(0);
    
    _string->setVisible(this->isMuted());
    
    _animatedString = Label::createWithBMFont("arial_rounded_mt_bold_55.fnt", str);
    _slideTextsNode->addChild(_animatedString);
    
    _animatedString->setMaxLineWidth(visibleSize.width * 0.75);
    _animatedString->setHorizontalAlignment(TextHAlignment::CENTER);
    _animatedString->setPosition(_string->getPosition());
    //_animatedString->setScale(0);
    
    _animatedString->setVisible(!this->isMuted());
    
    //    _string->runAction(Sequence::create(DelayTime::create(0.3),
    //                                        EaseBackOut::create(ScaleTo::create(0.15, 1)),
    //                                        NULL));
    //
    //    _animatedString->runAction(Sequence::create(DelayTime::create(0.3),
    //                                        EaseBackOut::create(ScaleTo::create(0.15, 1)),
    //                                        NULL));
    
    //
    
    
    Sprite *obj = Sprite::create("story_2.png");
    
    _slideObjectsNode->addChild(obj);
    
    obj->setPosition({_mount->getContentSize().width * 0.5f, _mount->getContentSize().height * 0.65f});
    obj->setScale(0);
    
    obj->runAction(Sequence::create(DelayTime::create(0.2),
                                    EaseBackOut::create(ScaleTo::create(0.15, 1)),
                                    DelayTime::create(0.1),
                                    CallFunc::create([=]() {
        SimpleAudioEngine::getInstance()->playEffect("story_2.mp3");
        
        // animated string
        vector<pair<float, pair<int, int> > > text;
        
        text.push_back({0.3, {0, 3}});
        text.push_back({0.69, {5, 8}});
        
        for(int i = 0; i < text.size(); ++i) {
            float delay = text[i].first;
            
            int chStart = text[i].second.first;
            int chEnd = text[i].second.second;
            
            for(int j = chStart; j <= chEnd; ++j) {
                Sprite *character = _animatedString->getLetter(j);
                
                Color3B charPrevColor = character->getColor();
                
                character->runAction(Sequence::create(DelayTime::create(delay),
                                                      TintTo::create(0.05, 50, 50, 50),
                                                      DelayTime::create(0.3),
                                                      TintTo::create(0.05, charPrevColor.r, charPrevColor.g, charPrevColor.b),
                                                      NULL));
            }
        }
    }), NULL));
}

void Page7Layer::openSlide3() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    // str
    
    string str = "Look! Pam's blue bag is in the sand.";
    
    _string = Label::createWithBMFont("arial_rounded_mt_bold_55.fnt", str);
    _slideTextsNode->addChild(_string);
    
    _string->setMaxLineWidth(visibleSize.width * 0.75);
    _string->setHorizontalAlignment(TextHAlignment::CENTER);
    _string->setPosition({_mount->getContentSize().width * 0.5f, _mount->getContentSize().height * 0.28f});
    //_string->setScale(0);
    
    _string->setVisible(this->isMuted());
    
    _animatedString = Label::createWithBMFont("arial_rounded_mt_bold_55.fnt", str);
    _slideTextsNode->addChild(_animatedString);
    
    _animatedString->setMaxLineWidth(visibleSize.width * 0.75);
    _animatedString->setHorizontalAlignment(TextHAlignment::CENTER);
    _animatedString->setPosition(_string->getPosition());
    //_animatedString->setScale(0);
    
    _animatedString->setVisible(!this->isMuted());
    
    //    _string->runAction(Sequence::create(DelayTime::create(0.3),
    //                                        EaseBackOut::create(ScaleTo::create(0.15, 1)),
    //                                        NULL));
    //
    //    _animatedString->runAction(Sequence::create(DelayTime::create(0.3),
    //                                        EaseBackOut::create(ScaleTo::create(0.15, 1)),
    //                                        NULL));
    
    //
    
    
    Sprite *obj = Sprite::create("story_3.png");
    
    _slideObjectsNode->addChild(obj);
    
    obj->setPosition({_mount->getContentSize().width * 0.5f, _mount->getContentSize().height * 0.65f});
    obj->setScale(0);
    
    obj->runAction(Sequence::create(DelayTime::create(0.2),
                                    EaseBackOut::create(ScaleTo::create(0.15, 1)),
                                    DelayTime::create(0.1),
                                    CallFunc::create([=]() {
        SimpleAudioEngine::getInstance()->playEffect("story_3.mp3");
        
        // animated string
        vector<pair<float, pair<int, int> > > text;
        
        text.push_back({0.28, {0, 4}});
        text.push_back({1.5, {6, 10}});
        text.push_back({2.11, {12, 15}});
        text.push_back({2.52, {17, 19}});
        text.push_back({3.4, {21, 22}});
        text.push_back({3.717, {24, 25}});
        text.push_back({4.11, {27, 29}});
        text.push_back({4.25, {31, 35}});
        
        for(int i = 0; i < text.size(); ++i) {
            float delay = text[i].first;
            
            int chStart = text[i].second.first;
            int chEnd = text[i].second.second;
            
            for(int j = chStart; j <= chEnd; ++j) {
                Sprite *character = _animatedString->getLetter(j);
                
                Color3B charPrevColor = character->getColor();
                
                character->runAction(Sequence::create(DelayTime::create(delay),
                                                      TintTo::create(0.05, 50, 50, 50),
                                                      DelayTime::create(0.3),
                                                      TintTo::create(0.05, charPrevColor.r, charPrevColor.g, charPrevColor.b),
                                                      NULL));
            }
        }
    }), NULL));
}

void Page7Layer::openSlide4() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    // str
    
    string str = "Sam's red mitt is in the sand.";
    
    _string = Label::createWithBMFont("arial_rounded_mt_bold_55.fnt", str);
    _slideTextsNode->addChild(_string);
    
    _string->setMaxLineWidth(visibleSize.width * 0.75);
    _string->setHorizontalAlignment(TextHAlignment::CENTER);
    _string->setPosition({_mount->getContentSize().width * 0.5f, _mount->getContentSize().height * 0.28f});
    //_string->setScale(0);
    
    _string->setVisible(this->isMuted());
    
    _animatedString = Label::createWithBMFont("arial_rounded_mt_bold_55.fnt", str);
    _slideTextsNode->addChild(_animatedString);
    
    _animatedString->setMaxLineWidth(visibleSize.width * 0.75);
    _animatedString->setHorizontalAlignment(TextHAlignment::CENTER);
    _animatedString->setPosition(_string->getPosition());
    //_animatedString->setScale(0);
    
    _animatedString->setVisible(!this->isMuted());
    
    //    _string->runAction(Sequence::create(DelayTime::create(0.3),
    //                                        EaseBackOut::create(ScaleTo::create(0.15, 1)),
    //                                        NULL));
    //
    //    _animatedString->runAction(Sequence::create(DelayTime::create(0.3),
    //                                        EaseBackOut::create(ScaleTo::create(0.15, 1)),
    //                                        NULL));
    
    //
    
    
    Sprite *obj = Sprite::create("story_4.png");
    
    _slideObjectsNode->addChild(obj);
    
    obj->setPosition({_mount->getContentSize().width * 0.5f, _mount->getContentSize().height * 0.65f});
    obj->setScale(0);
    
    obj->runAction(Sequence::create(DelayTime::create(0.2),
                                    EaseBackOut::create(ScaleTo::create(0.15, 1)),
                                    DelayTime::create(0.1),
                                    CallFunc::create([=]() {
        SimpleAudioEngine::getInstance()->playEffect("story_4.mp3");
        
        // animated string
        vector<pair<float, pair<int, int> > > text;
        
        text.push_back({0.4, {0, 4}});
        text.push_back({1.06, {6, 8}});
        text.push_back({1.4, {10, 13}});
        text.push_back({2.157, {15, 16}});
        text.push_back({2.553, {18, 19}});
        text.push_back({2.8, {21, 23}});
        text.push_back({3.0, {25, 29}});
        
        for(int i = 0; i < text.size(); ++i) {
            float delay = text[i].first;
            
            int chStart = text[i].second.first;
            int chEnd = text[i].second.second;
            
            for(int j = chStart; j <= chEnd; ++j) {
                Sprite *character = _animatedString->getLetter(j);
                
                Color3B charPrevColor = character->getColor();
                
                character->runAction(Sequence::create(DelayTime::create(delay),
                                                      TintTo::create(0.05, 50, 50, 50),
                                                      DelayTime::create(0.3),
                                                      TintTo::create(0.05, charPrevColor.r, charPrevColor.g, charPrevColor.b),
                                                      NULL));
            }
        }
    }), NULL));
}

void Page7Layer::openSlide5() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    // str
    
    string str = "A yellow mat is in the sand.";
    
    _string = Label::createWithBMFont("arial_rounded_mt_bold_55.fnt", str);
    _slideTextsNode->addChild(_string);
    
    _string->setMaxLineWidth(visibleSize.width * 0.75);
    _string->setHorizontalAlignment(TextHAlignment::CENTER);
    _string->setPosition({_mount->getContentSize().width * 0.5f, _mount->getContentSize().height * 0.28f});
    //_string->setScale(0);
    
    _string->setVisible(this->isMuted());
    
    _animatedString = Label::createWithBMFont("arial_rounded_mt_bold_55.fnt", str);
    _slideTextsNode->addChild(_animatedString);
    
    _animatedString->setMaxLineWidth(visibleSize.width * 0.75);
    _animatedString->setHorizontalAlignment(TextHAlignment::CENTER);
    _animatedString->setPosition(_string->getPosition());
    //_animatedString->setScale(0);
    
    _animatedString->setVisible(!this->isMuted());
    
    //    _string->runAction(Sequence::create(DelayTime::create(0.3),
    //                                        EaseBackOut::create(ScaleTo::create(0.15, 1)),
    //                                        NULL));
    //
    //    _animatedString->runAction(Sequence::create(DelayTime::create(0.3),
    //                                        EaseBackOut::create(ScaleTo::create(0.15, 1)),
    //                                        NULL));
    
    //
    
    
    Sprite *obj = Sprite::create("story_5.png");
    
    _slideObjectsNode->addChild(obj);
    
    obj->setPosition({_mount->getContentSize().width * 0.5f, _mount->getContentSize().height * 0.65f});
    obj->setScale(0);
    
    obj->runAction(Sequence::create(DelayTime::create(0.2),
                                    EaseBackOut::create(ScaleTo::create(0.15, 1)),
                                    DelayTime::create(0.1),
                                    CallFunc::create([=]() {
        SimpleAudioEngine::getInstance()->playEffect("story_5.mp3");
        
        // animated string
        vector<pair<float, pair<int, int> > > text;
        
        text.push_back({0.28, {0, 0}});
        text.push_back({0.48, {2, 7}});
        text.push_back({1.0, {9, 11}});
        text.push_back({1.83, {13, 14}});
        text.push_back({2.0, {16, 17}});
        text.push_back({2.2, {19, 21}});
        text.push_back({2.41, {23, 27}});
        
        for(int i = 0; i < text.size(); ++i) {
            float delay = text[i].first;
            
            int chStart = text[i].second.first;
            int chEnd = text[i].second.second;
            
            for(int j = chStart; j <= chEnd; ++j) {
                Sprite *character = _animatedString->getLetter(j);
                
                Color3B charPrevColor = character->getColor();
                
                character->runAction(Sequence::create(DelayTime::create(delay),
                                                      TintTo::create(0.05, 50, 50, 50),
                                                      DelayTime::create(0.3),
                                                      TintTo::create(0.05, charPrevColor.r, charPrevColor.g, charPrevColor.b),
                                                      NULL));
            }
        }
    }), NULL));
}

void Page7Layer::openSlide6() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    // str
    
    string str = "A green bin is in the sand.";
    
    _string = Label::createWithBMFont("arial_rounded_mt_bold_55.fnt", str);
    _slideTextsNode->addChild(_string);
    
    _string->setMaxLineWidth(visibleSize.width * 0.75);
    _string->setHorizontalAlignment(TextHAlignment::CENTER);
    _string->setPosition({_mount->getContentSize().width * 0.5f, _mount->getContentSize().height * 0.28f});
    //_string->setScale(0);
    
    _string->setVisible(this->isMuted());
    
    _animatedString = Label::createWithBMFont("arial_rounded_mt_bold_55.fnt", str);
    _slideTextsNode->addChild(_animatedString);
    
    _animatedString->setMaxLineWidth(visibleSize.width * 0.75);
    _animatedString->setHorizontalAlignment(TextHAlignment::CENTER);
    _animatedString->setPosition(_string->getPosition());
    //_animatedString->setScale(0);
    
    _animatedString->setVisible(!this->isMuted());
    
    //    _string->runAction(Sequence::create(DelayTime::create(0.3),
    //                                        EaseBackOut::create(ScaleTo::create(0.15, 1)),
    //                                        NULL));
    //
    //    _animatedString->runAction(Sequence::create(DelayTime::create(0.3),
    //                                        EaseBackOut::create(ScaleTo::create(0.15, 1)),
    //                                        NULL));
    
    //
    
    
    Sprite *obj = Sprite::create("story_6.png");
    
    _slideObjectsNode->addChild(obj);
    
    obj->setPosition({_mount->getContentSize().width * 0.5f, _mount->getContentSize().height * 0.65f});
    obj->setScale(0);
    
    obj->runAction(Sequence::create(DelayTime::create(0.2),
                                    EaseBackOut::create(ScaleTo::create(0.15, 1)),
                                    DelayTime::create(0.1),
                                    CallFunc::create([=]() {
        SimpleAudioEngine::getInstance()->playEffect("story_6.mp3");
        
        // animated string
        vector<pair<float, pair<int, int> > > text;
        
        text.push_back({0.28, {0, 0}});
        text.push_back({0.48, {2, 6}});
        text.push_back({1.0, {8, 10}});
        text.push_back({1.93, {12, 13}});
        text.push_back({2.15, {15, 16}});
        text.push_back({2.3, {18, 20}});
        text.push_back({2.65, {22, 26}});
        
        for(int i = 0; i < text.size(); ++i) {
            float delay = text[i].first;
            
            int chStart = text[i].second.first;
            int chEnd = text[i].second.second;
            
            for(int j = chStart; j <= chEnd; ++j) {
                Sprite *character = _animatedString->getLetter(j);
                
                Color3B charPrevColor = character->getColor();
                
                character->runAction(Sequence::create(DelayTime::create(delay),
                                                      TintTo::create(0.05, 50, 50, 50),
                                                      DelayTime::create(0.3),
                                                      TintTo::create(0.05, charPrevColor.r, charPrevColor.g, charPrevColor.b),
                                                      NULL));
            }
        }
    }), NULL));
}

void Page7Layer::openSlide7() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    // str
    
    string str = "Look! A black ant is in the sand too!";
    
    _string = Label::createWithBMFont("arial_rounded_mt_bold_55.fnt", str);
    _slideTextsNode->addChild(_string);
    
    _string->setMaxLineWidth(visibleSize.width * 0.75);
    _string->setHorizontalAlignment(TextHAlignment::CENTER);
    _string->setPosition({_mount->getContentSize().width * 0.5f, _mount->getContentSize().height * 0.28f});
    //_string->setScale(0);
    
    _string->setVisible(this->isMuted());
    
    _animatedString = Label::createWithBMFont("arial_rounded_mt_bold_55.fnt", str);
    _slideTextsNode->addChild(_animatedString);
    
    _animatedString->setMaxLineWidth(visibleSize.width * 0.75);
    _animatedString->setHorizontalAlignment(TextHAlignment::CENTER);
    _animatedString->setPosition(_string->getPosition());
    //_animatedString->setScale(0);
    
    _animatedString->setVisible(!this->isMuted());
    
    //    _string->runAction(Sequence::create(DelayTime::create(0.3),
    //                                        EaseBackOut::create(ScaleTo::create(0.15, 1)),
    //                                        NULL));
    //
    //    _animatedString->runAction(Sequence::create(DelayTime::create(0.3),
    //                                        EaseBackOut::create(ScaleTo::create(0.15, 1)),
    //                                        NULL));
    
    //
    
    
    Sprite *obj = Sprite::create("story_7.png");
    
    _slideObjectsNode->addChild(obj);
    
    obj->setPosition({_mount->getContentSize().width * 0.5f, _mount->getContentSize().height * 0.65f});
    obj->setScale(0);
    
    obj->runAction(Sequence::create(DelayTime::create(0.2),
                                    EaseBackOut::create(ScaleTo::create(0.15, 1)),
                                    DelayTime::create(0.1),
                                    CallFunc::create([=]() {
        SimpleAudioEngine::getInstance()->playEffect("story_7.mp3");
        
        // animated string
        vector<pair<float, pair<int, int> > > text;
        
        text.push_back({0.46, {0, 4}});
        text.push_back({1.47, {6, 6}});
        text.push_back({1.84, {8, 12}});
        text.push_back({2.48, {14, 16}});
        text.push_back({3.4, {18, 19}});
        text.push_back({3.73, {21, 22}});
        text.push_back({4.0, {24, 26}});
        text.push_back({4.165, {28, 31}});
        text.push_back({4.923, {33, 36}});
        
        for(int i = 0; i < text.size(); ++i) {
            float delay = text[i].first;
            
            int chStart = text[i].second.first;
            int chEnd = text[i].second.second;
            
            for(int j = chStart; j <= chEnd; ++j) {
                Sprite *character = _animatedString->getLetter(j);
                
                Color3B charPrevColor = character->getColor();
                
                character->runAction(Sequence::create(DelayTime::create(delay),
                                                      TintTo::create(0.05, 50, 50, 50),
                                                      DelayTime::create(0.3),
                                                      TintTo::create(0.05, charPrevColor.r, charPrevColor.g, charPrevColor.b),
                                                      NULL));
            }
        }
    }), NULL));
}


bool Page7Layer::isMuted() const{
    return _muted->isVisible();
}

void Page7Layer::setMuted(bool muted) {
    _muted->setVisible(muted);
    
    float volume = 1;

    if(muted) {
        volume = 0;
    }

    SimpleAudioEngine::getInstance()->setEffectsVolume(volume);
    
    _string->setVisible(muted);
    _animatedString->setVisible(!muted);
}

void Page7Layer::onMuteBtnPressed() {
    this->setMuted(!this->isMuted());
}