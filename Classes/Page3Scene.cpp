//
//  Page3Scene.cpp
//  inthesand
//
//  Created by Alex Gievsky on 10.03.15.
//
//

#include "Page3Scene.h"

#include "GameLayer.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

using std::vector;
using std::string;

#define kNumOfSlides 4
#define kMaxAttempts 3

#define zBlockMenu 100

Page3Layer::~Page3Layer() {
    
}

Page3Layer::Page3Layer() {
    _back = nullptr;
    
    _menu = nullptr;
    _backBtn = nullptr;
    _nextBtn = nullptr;
    
    _top = nullptr;
    _bottom = nullptr;
    
    _lettersMenu = nullptr;
    
    _mount = nullptr;
    
    _currentSlide = 0;
    _slideNode = nullptr;
    
    _sqr0 = nullptr;
    _sqr0 = nullptr;
    _sqr0 = nullptr;
    
    _currentLetter = nullptr;
    _tutorialWasShown = false;

    _bButton = nullptr;
    _iButton = nullptr;
    _nButton = nullptr;
    
    _blockMenu = nullptr;
}

Scene* Page3Layer::scene() {
    auto scene = Scene::create();
    auto gameLayer = Page3Layer::create();
    
    // add layer as a child to scene
    scene->addChild(gameLayer);
    
    // return the scene
    return scene;
    
}

bool Page3Layer::init() {
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
        _backBtn = MenuItemImage::create("btn_back.png", "btn_back_on.png", CC_CALLBACK_0(Page3Layer::onBackBtnPressed, this));
        _backBtn->setAnchorPoint({0, 0});
        _backBtn->setPosition({0, 0});
        
        _nextBtn = MenuItemImage::create("btn_back.png", "btn_back_on.png", "btn_back_disabled.png", CC_CALLBACK_0(Page3Layer::onNextBtnPressed, this));
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
        
        _mount->setAnchorPoint({0.5, 0});
        _mount->setPosition({-_mount->getContentSize().width * 0.5f, _backBtn->getPosition().y + _backBtn->getContentSize().height});
        
        _mount->runAction(Sequence::create(DelayTime::create(0.1),
                                           MoveTo::create(0.1, {visibleSize.width * 0.5f, _mount->getPositionY()}),
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
                                                       CC_CALLBACK_1(Page3Layer::onCharacterBtnPressed, this));
            
            if(lettersCodes[i] == 1) {
                _bButton = btn;
            } else if(lettersCodes[i] == 8) {
                _iButton = btn;
            } else if(lettersCodes[i] == 13) {
                _nButton = btn;
            }
            
            buttons.pushBack(btn);
            
            Sprite *image = Sprite::create(StringUtils::format("ch_pair_%i.png", lettersCodes[i]));
            btn->addChild(image);
            btn->setTag(lettersCodes[i]);
            btn->setEnabled(false);
            
            image->setPosition(btn->getContentSize().width * 0.5f, btn->getContentSize().height * 0.5f);
            
            _letters.push_back(btn);
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
                                                 CallFunc::create([this]() {
            this->openSlide(0);
        }), NULL));
    }
    
    // slides
    {
        _slideNode = Node::create();
        _mount->addChild(_slideNode);
        
        _slideNode->setPosition({0, 0});
    }
    
    // touches
    {
        auto touchesListener = EventListenerTouchOneByOne::create();
        
        touchesListener->onTouchBegan = CC_CALLBACK_2(Page3Layer::onTouchBegan, this);
        touchesListener->onTouchEnded = CC_CALLBACK_2(Page3Layer::onTouchEnded, this);
        touchesListener->onTouchMoved = CC_CALLBACK_2(Page3Layer::onTouchMoved, this);
        touchesListener->onTouchCancelled = CC_CALLBACK_2(Page3Layer::onTouchCancelled, this);
        
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchesListener, this);
    }
    
    // squares
    {
        _sqr0 = Sprite::create("drag_square.png");
        this->addChild(_sqr0);
        
        _sqr1 = Sprite::create("drag_square.png");
        this->addChild(_sqr1);
        
        _sqr2 = Sprite::create("drag_square.png");
        this->addChild(_sqr2);
        
        _sqr0->setPosition({visibleSize.width * 0.5f - _sqr1->getContentSize().width, _mount->getPositionY() + _mount->getContentSize().height * 0.15f});
        _sqr1->setPosition({visibleSize.width * 0.5f, _mount->getPositionY() + _mount->getContentSize().height * 0.15f});
        _sqr2->setPosition({visibleSize.width * 0.5f + _sqr1->getContentSize().width, _mount->getPositionY() + _mount->getContentSize().height * 0.15f});
        
        _sqr0->setScale(0);
        _sqr1->setScale(0);
        _sqr2->setScale(0);
        
        _sqr0->runAction(Sequence::create(DelayTime::create(0.1),
                                          EaseBackOut::create(ScaleTo::create(0.2, 1)),
                                          NULL));
        
        _sqr1->runAction(Sequence::create(DelayTime::create(0.1),
                                          EaseBackOut::create(ScaleTo::create(0.2, 1)),
                                          NULL));
        
        _sqr2->runAction(Sequence::create(DelayTime::create(0.1),
                                          EaseBackOut::create(ScaleTo::create(0.2, 1)),
                                          NULL));
    }
    
    return true;
}

void Page3Layer::onBackBtnPressed() {
    SimpleAudioEngine::getInstance()->stopAllEffects();
    
    _currentSlide--;
    _attemptsMade = kMaxAttempts;
    
    _nextBtn->setEnabled(true);
    
    if(_currentSlide < 0) {
        _menu->setEnabled(false);
        this->popOut();
    } else {
        this->openSlide(_currentSlide);
    }
}

void Page3Layer::onNextBtnPressed() {
    SimpleAudioEngine::getInstance()->stopAllEffects();
    
    _currentSlide++;
    
    if(_currentSlide == kNumOfSlides - 1) {
        _nextBtn->setEnabled(false);
    }
    
    this->openSlide(_currentSlide);
}

void Page3Layer::popOut() {
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    //    _text->runAction(FadeOut::create(0.1f));
    //    _mount->runAction(ScaleTo::create(0.2, 1.5));
    
    this->runAction(Sequence::create(DelayTime::create(0.12),
                                     CallFunc::create([this]() {
        Director::getInstance()->replaceScene(GameLayer::scene());
    }), NULL));
}

void Page3Layer::onCharacterBtnPressed(cocos2d::Ref *btn) {
    int tag = (static_cast<Node*>(btn)->getTag());
    
    SimpleAudioEngine::getInstance()->stopAllEffects();
    SimpleAudioEngine::getInstance()->playEffect(StringUtils::format("abc_sound_%i.mp3", tag).c_str());
    
}

void Page3Layer::openSlide(int slide) {
    SimpleAudioEngine::getInstance()->stopAllEffects();
    
    _sqr0->removeAllChildren();
    _sqr1->removeAllChildren();
    _sqr2->removeAllChildren();
    
    _slideNode->removeAllChildren();
    
    _attemptsMade = kMaxAttempts;
    
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
    }
}

void Page3Layer::openSlide0() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    _sqr0->setTag(1);
    _sqr1->setTag(8);
    _sqr2->setTag(13);
    
    Sprite *obj = Sprite::create("drag_slide_0.png");
    
    _slideNode->addChild(obj);
    
    obj->setPosition({_mount->getContentSize().width * 0.5f, _mount->getContentSize().height * 0.6f});
    obj->setScale(0);
    
    obj->runAction(Sequence::create(DelayTime::create(0.8),
                                    EaseBackOut::create(ScaleTo::create(0.15, 1)),
                                    CallFunc::create([=]() {
        SimpleAudioEngine::getInstance()->playEffect("drag_slide_0.mp3");
    }), NULL));
    
    if(_tutorialWasShown == false) {
        this->block();
        
        Sprite *hand = Sprite::create("hand.png");
        this->addChild(hand);
        
        hand->setAnchorPoint({0, 1});
        hand->setPosition({visibleSize.width * 0.7f, visibleSize.height * 0.3f});
        hand->setScale(0);
        
        Point bPos = _lettersMenu->getPosition() + _bButton->getPosition();
        
        hand->runAction(Sequence::create(DelayTime::create(1.6),
                                         EaseBackOut::create(ScaleTo::create(0.2, 1)),
                                         DelayTime::create(0.5),
                                         MoveTo::create(1, bPos),
                                         DelayTime::create(0.2),
                                         CallFunc::create([=]() {
                                            Sprite *b = Sprite::create("ch_small_1.png");
                                            hand->addChild(b, -1);
                                            b->setPosition({0, hand->getContentSize().height});
            
                                            hand->runAction(Sequence::create(MoveTo::create(1, _sqr0->getPosition()),
                                                                             CallFunc::create([=]() {
                                                b->setVisible(false);
                                                
                                                Sprite *b1 = Sprite::create("ch_small_1.png");
                                                _sqr0->addChild(b1);
                                                
                                                b1->setPosition(_sqr0->getContentSize().width * 0.5f, _sqr0->getContentSize().height * 0.5f);
                                                
                                                hand->runAction(Sequence::create(DelayTime::create(0.3),
                                                                                 EaseBackIn::create(ScaleTo::create(0.2, 0)),
                                                                                 DelayTime::create(0.5),
                                                                                 CallFunc::create([=]() {
                                                    
                                                    _tutorialWasShown = true;
                                                    
                                                    _bButton->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.2, 1.1),
                                                                                                               DelayTime::create(0.1),
                                                                                                               ScaleTo::create(0.1, 1),
                                                                                                               DelayTime::create(1.7),
                                                                                                               NULL)));
                                                    
                                                    _iButton->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.3),
                                                                                                               ScaleTo::create(0.2, 1.1),
                                                                                                               DelayTime::create(0.1),
                                                                                                               ScaleTo::create(0.1, 1),
                                                                                                               DelayTime::create(1.4),
                                                                                                               NULL)));
                                                    
                                                    _nButton->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.5),
                                                                                                               ScaleTo::create(0.2, 1.1),
                                                                                                               DelayTime::create(0.1),
                                                                                                               ScaleTo::create(0.1, 1),
                                                                                                               DelayTime::create(1.2),
                                                                                                               NULL)));
                                                    this->unblock();
                                                    
                                                    b1->removeFromParent();
                                                    hand->removeFromParent();
                                                }), nullptr));
                                                
                                            }), NULL));
                                         }),
                                         NULL));
        
    }
}

void Page3Layer::openSlide1() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    _sqr0->setTag(18);
    _sqr1->setTag(8);
    _sqr2->setTag(19);
    
    Sprite *obj = Sprite::create("drag_slide_1.png");
    
    _slideNode->addChild(obj);
    
    obj->setPosition({_mount->getContentSize().width * 0.5f, _mount->getContentSize().height * 0.6f});
    obj->setScale(0);
    
    obj->runAction(Sequence::create(DelayTime::create(0.8),
                                    EaseBackOut::create(ScaleTo::create(0.15, 1)),
                                    CallFunc::create([=]() {
        SimpleAudioEngine::getInstance()->playEffect("drag_slide_1.mp3");
    }), NULL));
}

void Page3Layer::openSlide2() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    _sqr0->setTag(3);
    _sqr1->setTag(0);
    _sqr2->setTag(3);
    
    Sprite *obj = Sprite::create("drag_slide_2.png");
    
    _slideNode->addChild(obj);
    
    obj->setPosition({_mount->getContentSize().width * 0.5f, _mount->getContentSize().height * 0.6f});
    obj->setScale(0);
    
    obj->runAction(Sequence::create(DelayTime::create(0.8),
                                    EaseBackOut::create(ScaleTo::create(0.15, 1)),
                                    CallFunc::create([=]() {
        SimpleAudioEngine::getInstance()->playEffect("drag_slide_2.mp3");
    }), NULL));
}

void Page3Layer::openSlide3() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    _sqr0->setTag(12);
    _sqr1->setTag(0);
    _sqr2->setTag(19);
    
    Sprite *obj = Sprite::create("drag_slide_3.png");
    
    _slideNode->addChild(obj);
    
    obj->setPosition({_mount->getContentSize().width * 0.5f, _mount->getContentSize().height * 0.6f});
    obj->setScale(0);
    
    obj->runAction(Sequence::create(DelayTime::create(0.8),
                                    EaseBackOut::create(ScaleTo::create(0.15, 1)),
                                    CallFunc::create([=]() {
        SimpleAudioEngine::getInstance()->playEffect("drag_slide_3.mp3");
    }), NULL));
}

#pragma mark - Touches

bool Page3Layer::onTouchBegan(Touch *touch, Event *event) {
    _bButton->stopAllActions();
    _iButton->stopAllActions();
    _nButton->stopAllActions();
    
    _bButton->setScale(1);
    _iButton->setScale(1);
    _nButton->setScale(1);
    
    Point pos = touch->getLocation();
    
    
    
    return true;
}

void Page3Layer::onTouchMoved(Touch *touch, Event  *event) {
    Point pos = touch->getLocation();
    
    for(int i = 0; i < _letters.size(); ++i) {
        Point localPos = _lettersMenu->convertToNodeSpace(pos);
        if(_letters[i]->getBoundingBox().containsPoint(localPos) && _currentLetter == nullptr) {
            _currentLetter = Sprite::create(StringUtils::format("ch_small_%i.png", _letters[i]->getTag()));
            this->addChild(_currentLetter);
            
            _currentLetter->setPosition(pos);
            _currentLetter->setTag(_letters[i]->getTag());
            
            this->onCharacterBtnPressed(_letters[i]);
            break;
        }
    }
    
    if(_currentLetter != nullptr) {
        _currentLetter->setPosition(pos);
    }
}

void Page3Layer::onTouchEnded(Touch *touch, Event *event) {
    
    Point pos = touch->getLocation();
    
    // check if a buttons was pressed
//    bool btnWasPressed = false;

//    for(int i = 0; i < _letters.size(); ++i) {
//        Point localPos = _lettersMenu->convertToNodeSpace(pos);
//        if(_letters[i]->getBoundingBox().containsPoint(localPos)) {
//            this->onCharacterBtnPressed(_letters[i]);
//            btnWasPressed = true;
//            break;
//        }
//    }
    
    if(_currentLetter != nullptr) {
        vector<Node*> squares;
        squares.push_back(_sqr0);
        squares.push_back(_sqr1);
        squares.push_back(_sqr2);
        
        int numOfFilledSquares = 0;
        const int numOfSquaresToComplete = 3;
        
        for(int i = 0; i < squares.size(); ++i) {
            if(squares[i]->getBoundingBox().containsPoint(pos)) {
                squares[i]->removeAllChildren();
                
                Sprite *s = Sprite::create(StringUtils::format("ch_small_%i.png", _currentLetter->getTag()));
                squares[i]->addChild(s);
                
                s->setPosition({squares[i]->getContentSize().width * 0.5f, squares[i]->getContentSize().height * 0.5f});
                s->setTag(_currentLetter->getTag());
            }
            
            if(squares[i]->getChildrenCount() != 0) {
                numOfFilledSquares++;
            }
        }
        
        if(numOfFilledSquares == numOfSquaresToComplete) {
            // we're ready, aren't we?
            bool slideComplete = true;
            for(int i = 0; i < squares.size(); ++i) {
                if(squares[i]->getChildByTag(squares[i]->getTag()) == nullptr) {
                    slideComplete = false;
                    break;
                }
            }
            
            if(slideComplete) {
                SimpleAudioEngine::getInstance()->playEffect("great.mp3");
                
                for(int i = 0; i < squares.size(); ++i) {
                    squares[i]->getChildByTag(squares[i]->getTag())->runAction(Sequence::create(DelayTime::create(0.1 * i),
                                                                                                ScaleTo::create(0.2, 1.2),
                                                                                                DelayTime::create(0.05),
                                                                                                ScaleTo::create(0.1, 1),
                                                                                                NULL));
                }

                if(_nextBtn->isVisible() == false) {
                    _nextBtn->setVisible(true);
                    
                    _nextBtn->runAction(MoveBy::create(0.1, {0, _nextBtn->getContentSize().height}));
                }
                
            } else {
                _attemptsMade--;

                this->block();
                
                string soundFileName = "try.mp3";
                
                if(_attemptsMade == 0) {
                    soundFileName = StringUtils::format("drag_slide_%i.mp3", _currentSlide);
                } else {
                }
                
                for(int i = 0; i < squares.size(); ++i) {
                    Sprite *mark = nullptr;
                    if(squares[i]->getChildByTag(squares[i]->getTag()) == nullptr) {
                        mark = Sprite::create("cross.png");
                        
//                        squares[i]->runAction(Sequence::create(DelayTime::create(1.6),
//                                                               CallFunc::create([=]() {
//                            squares[i]->removeAllChildren();
//                        }), NULL));
                    } else {
                        mark = Sprite::create("mark.png");
                    }
                    
                    squares[i]->addChild(mark);
                    
                    mark->setAnchorPoint({1, 0});
                    mark->setPosition({squares[i]->getContentSize().width, 0});
                    mark->setScale(0);
                    
                    mark->runAction(Sequence::create(DelayTime::create(0.1 * i),
                                                     EaseBackOut::create(ScaleTo::create(0.2, 1)),
                                                     DelayTime::create(1),
                                                     ScaleTo::create(0.1, 0),
                                                     CallFunc::create([=]() {

                        if(squares[i]->getChildByTag(squares[i]->getTag()) == nullptr) {
                            squares[i]->removeAllChildren();
                        }
                        
                        if(i == numOfSquaresToComplete - 1) {
                            this->unblock();
                            
                            SimpleAudioEngine::getInstance()->playEffect(soundFileName.c_str());
                            
                            if(_attemptsMade == 0) {
                                this->block();

                                for(int j = 0; j < squares.size(); ++j) {
                                    squares[j]->removeAllChildren();
                                    Sprite *ch = Sprite::create(StringUtils::format("ch_small_%i.png", squares[j]->getTag()));
                                    squares[j]->addChild(ch);
                                    
                                    ch->setPosition({squares[j]->getContentSize().width * 0.5f, squares[j]->getContentSize().height * 0.5f});
                                    
                                    ch->runAction(Sequence::create(DelayTime::create(1),
                                                                   CallFunc::create([=]() {
                                        
                                        if(j == numOfSquaresToComplete - 1) {
                                            SimpleAudioEngine::getInstance()->playEffect("try.mp3");
                                            this->unblock();
                                            
                                            _attemptsMade = kMaxAttempts;
                                        }
                                        
                                        ch->removeFromParent();
                                    }), NULL));
                                }
                            }
                        }
                        
                        mark->removeFromParent();
                    }), NULL));
                }

            }
        }
        
        _currentLetter->removeFromParent();
        _currentLetter = nullptr;
    }
}

void Page3Layer::onTouchCancelled(Touch *touch, Event  *event) {
    this->onTouchEnded(touch, event);
    //_touchesEnabled = true;
}

void Page3Layer::block() {
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

void Page3Layer::unblock() {
    if(_blockMenu != nullptr) {
        _blockMenu->removeFromParent();
        _blockMenu = nullptr;
    }
}