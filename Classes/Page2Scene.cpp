//
//  Page2Scene.cpp
//  inthesand
//
//  Created by Alex Gievsky on 08.03.15.
//
//

#include "Page2Scene.h"

#include "GameLayer.h"
#include "MainMenu.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

using std::vector;
using std::string;

#define kNumOfSlides 4

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
    _slideNode = nullptr;
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
        _menuBtn = MenuItemImage::create("btn_menu.png", "btn_menu_on.png", CC_CALLBACK_0(Page2Layer::onMenuBtnPressed, this));
        _menuBtn->setAnchorPoint({0, 0});
        _menuBtn->setPosition({0, 0});
        
        _backBtn = MenuItemImage::create("btn_back.png", "btn_back_on.png", "btn_back_disabled.png", CC_CALLBACK_0(Page2Layer::onBackBtnPressed, this));
        _backBtn->setAnchorPoint({0, 0});
        _backBtn->setPosition({_menuBtn->getContentSize().width, 0});
        _backBtn->setEnabled(false);
        
        _nextBtn = MenuItemImage::create("btn_back.png", "btn_back_on.png", "btn_back_disabled.png", CC_CALLBACK_0(Page2Layer::onNextBtnPressed, this));
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
    
    // slides
    {
        _slideNode = Node::create();
        _mount->addChild(_slideNode);
        
        _slideNode->setPosition({0, 0});
    }
    
    this->openSlide(0);
    
    return true;
}

void Page2Layer::onMenuBtnPressed() {
    this->popOut();
}

void Page2Layer::onBackBtnPressed() {
    SimpleAudioEngine::getInstance()->stopAllEffects();

    _currentSlide--;
    
    _nextBtn->setEnabled(true);
    
    if(_currentSlide == 0) {
        _backBtn->setEnabled(false);
    }
    
    this->openSlide(_currentSlide);
}

void Page2Layer::onNextBtnPressed() {
    SimpleAudioEngine::getInstance()->stopAllEffects();
    
    _currentSlide++;

    if(_currentSlide == kNumOfSlides - 1) {
        _nextBtn->setEnabled(false);
    }
    
    this->openSlide(_currentSlide);
}

void Page2Layer::popOut() {
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    //    _text->runAction(FadeOut::create(0.1f));
    //    _mount->runAction(ScaleTo::create(0.2, 1.5));
    
    this->runAction(Sequence::create(DelayTime::create(0.12),
                                     CallFunc::create([this]() {
        Director::getInstance()->replaceScene(MainMenu::scene());
    }), NULL));
}

void Page2Layer::onCharacterBtnPressed(cocos2d::Ref *btn) {
    int tag = (static_cast<Node*>(btn)->getTag());
    
    SimpleAudioEngine::getInstance()->stopAllEffects();
    SimpleAudioEngine::getInstance()->playEffect(StringUtils::format("abc_name_sound_%i.mp3", tag).c_str());

}

void Page2Layer::openSlide(int slide) {
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
    }
}

void Page2Layer::openSlide0() {
    Sprite *obj = Sprite::create("reading_slide_0.png");
    
    _slideNode->addChild(obj);
    
    obj->setPosition({_mount->getContentSize().width * 0.5f, _mount->getContentSize().height * 0.6f});
    obj->setScale(0);
    
    obj->runAction(Sequence::create(DelayTime::create(0.8),
                                    EaseBackOut::create(ScaleTo::create(0.15, 1)),
                                    CallFunc::create([=]() {
        SimpleAudioEngine::getInstance()->playEffect("reading_slide_0.mp3");
    }), NULL));
    
    
    {
        Sprite *b = Sprite::create("ch_small_1.png");
        _slideNode->addChild(b);
        
        Sprite *i = Sprite::create("ch_small_8.png");
        _slideNode->addChild(i);
        
        Sprite *n = Sprite::create("ch_small_13.png");
        _slideNode->addChild(n);
        
        b->setPosition({_mount->getContentSize().width * 0.5f - b->getContentSize().width * 2.5f,
            _mount->getContentSize().height * 0.15f
        });
        
        i->setPosition({_mount->getContentSize().width * 0.5f, _mount->getContentSize().height * 0.15f});
        
        n->setPosition({_mount->getContentSize().width * 0.5f + b->getContentSize().width * 2.5f,
            _mount->getContentSize().height * 0.15f
        });
        
        b->setScale(0);
        i->setScale(0);
        n->setScale(0);
        
        b->runAction(Sequence::create(DelayTime::create(1.9),
                                      CallFunc::create([=]() {
                                        SimpleAudioEngine::getInstance()->playEffect("abc_sound_1.mp3");
                                      }),
                                      EaseBackOut::create(ScaleTo::create(0.15, 1)),
                                      NULL));
        
        i->runAction(Sequence::create(DelayTime::create(2.5),
                                      CallFunc::create([=]() {
                                        SimpleAudioEngine::getInstance()->playEffect("abc_sound_8.mp3");
                                      }),
                                      EaseBackOut::create(ScaleTo::create(0.15, 1)),
                                      NULL));
        
        n->runAction(Sequence::create(DelayTime::create(3.1),
                                      CallFunc::create([=]() {
                                        SimpleAudioEngine::getInstance()->playEffect("abc_sound_13.mp3");
                                      }),
                                      EaseBackOut::create(ScaleTo::create(0.15, 1)),
                                      DelayTime::create(0.7),
                                      CallFunc::create([=]() {
            SimpleAudioEngine::getInstance()->playEffect("reading_slide_0.mp3");
            
            b->runAction(MoveBy::create(0.2, {b->getContentSize().width * 1.5f, 0}));
            n->runAction(MoveBy::create(0.2, {-b->getContentSize().width * 1.5f, 0}));
        }), CallFunc::create([this]() {
            if(_nextBtn->isVisible() == false) {
                _nextBtn->setVisible(true);
                
                _nextBtn->runAction(MoveBy::create(0.1, {0, _nextBtn->getContentSize().height}));
            }
        }), NULL));
    }
}

void Page2Layer::openSlide1() {
    Sprite *obj = Sprite::create("reading_slide_1.png");
    
    _slideNode->addChild(obj);
    
    obj->setPosition({_mount->getContentSize().width * 0.5f, _mount->getContentSize().height * 0.6f});
    obj->setScale(0);
    
    obj->runAction(Sequence::create(DelayTime::create(0.8),
                                    EaseBackOut::create(ScaleTo::create(0.15, 1)),
                                    CallFunc::create([=]() {
        SimpleAudioEngine::getInstance()->playEffect("reading_slide_1.mp3");
    }), NULL));
    
    
    {
        Sprite *s = Sprite::create("ch_small_18.png");
        _slideNode->addChild(s);
        
        Sprite *a = Sprite::create("ch_small_0.png");
        _slideNode->addChild(a);
        
        Sprite *n = Sprite::create("ch_small_13.png");
        _slideNode->addChild(n);
        
        Sprite *d = Sprite::create("ch_small_3.png");
        _slideNode->addChild(d);
        
        s->setPosition({_mount->getContentSize().width * 0.5f - s->getContentSize().width * 3.5f,
            _mount->getContentSize().height * 0.15f
        });
        
        a->setPosition({_mount->getContentSize().width * 0.5f - s->getContentSize().width * 1.0f,
            _mount->getContentSize().height * 0.15f
        });
        
        n->setPosition({_mount->getContentSize().width * 0.5f + s->getContentSize().width * 1.f,
            _mount->getContentSize().height * 0.15f
        });
        
        d->setPosition({_mount->getContentSize().width * 0.5f + s->getContentSize().width * 3.5f,
            _mount->getContentSize().height * 0.15f
        });
        
        s->setScale(0);
        a->setScale(0);
        n->setScale(0);
        d->setScale(0);
        
        s->runAction(Sequence::create(DelayTime::create(2.2),
                                      CallFunc::create([=]() {
                                        SimpleAudioEngine::getInstance()->playEffect("abc_sound_18.mp3");
                                      }),
                                      EaseBackOut::create(ScaleTo::create(0.15, 1)),
                                      NULL));
        
        a->runAction(Sequence::create(DelayTime::create(2.8),
                                      CallFunc::create([=]() {
                                        SimpleAudioEngine::getInstance()->playEffect("abc_sound_0.mp3");
                                      }),
                                      EaseBackOut::create(ScaleTo::create(0.15, 1)),
                                      NULL));
        
        n->runAction(Sequence::create(DelayTime::create(3.4),
                                      CallFunc::create([=]() {
                                        SimpleAudioEngine::getInstance()->playEffect("abc_sound_13.mp3");
                                      }),
                                      EaseBackOut::create(ScaleTo::create(0.15, 1)),
                                      NULL));
        
        d->runAction(Sequence::create(DelayTime::create(4),
                                      CallFunc::create([=]() {
                                        SimpleAudioEngine::getInstance()->playEffect("abc_sound_3.mp3");
                                      }),
                                      EaseBackOut::create(ScaleTo::create(0.15, 1)),
                                      DelayTime::create(0.7),
                                      CallFunc::create([=]() {
            SimpleAudioEngine::getInstance()->playEffect("reading_slide_1.mp3");
            
            s->runAction(MoveBy::create(0.2, {s->getContentSize().width * 1.5f, 0}));
            a->runAction(MoveBy::create(0.2, {s->getContentSize().width * 0.5f, 0}));
            n->runAction(MoveBy::create(0.2, {-s->getContentSize().width * 0.5f, 0}));
            d->runAction(MoveBy::create(0.2, {-s->getContentSize().width * 1.5f, 0}));
        }), NULL));
    }
}

void Page2Layer::openSlide2() {
    Sprite *obj = Sprite::create("reading_slide_2.png");
    
    _slideNode->addChild(obj);
    
    obj->setPosition({_mount->getContentSize().width * 0.5f, _mount->getContentSize().height * 0.6f});
    obj->setScale(0);
    
    obj->runAction(Sequence::create(DelayTime::create(0.8),
                                    EaseBackOut::create(ScaleTo::create(0.15, 1)),
                                    CallFunc::create([=]() {
        SimpleAudioEngine::getInstance()->playEffect("reading_slide_2.mp3");
    }), NULL));
    
    
    {
        Sprite *m = Sprite::create("ch_small_12.png");
        _slideNode->addChild(m);
        
        Sprite *i = Sprite::create("ch_small_8.png");
        _slideNode->addChild(i);
        
        Sprite *t1 = Sprite::create("ch_small_19.png");
        _slideNode->addChild(t1);
        
        Sprite *t2 = Sprite::create("ch_small_19.png");
        _slideNode->addChild(t2);
        
        m->setPosition({_mount->getContentSize().width * 0.5f - m->getContentSize().width * 3.5f,
            _mount->getContentSize().height * 0.15f
        });
        
        i->setPosition({_mount->getContentSize().width * 0.5f - i->getContentSize().width * 1.0f,
            _mount->getContentSize().height * 0.15f
        });
        
        t1->setPosition({_mount->getContentSize().width * 0.5f + t1->getContentSize().width * 1.f,
            _mount->getContentSize().height * 0.15f
        });
        
        t2->setPosition({_mount->getContentSize().width * 0.5f + t2->getContentSize().width * 3.5f,
            _mount->getContentSize().height * 0.15f
        });
        
        m->setScale(0);
        i->setScale(0);
        t1->setScale(0);
        t2->setScale(0);
        
        m->runAction(Sequence::create(DelayTime::create(2.2),
                                      CallFunc::create([=]() {
                                        SimpleAudioEngine::getInstance()->playEffect("abc_sound_12.mp3");
                                      }),
                                      EaseBackOut::create(ScaleTo::create(0.15, 1)),
                                      NULL));
        
        i->runAction(Sequence::create(DelayTime::create(2.8),
                                      CallFunc::create([=]() {
                                        SimpleAudioEngine::getInstance()->playEffect("abc_sound_8.mp3");
                                      }),
                                      EaseBackOut::create(ScaleTo::create(0.15, 1)),
                                      NULL));
        
        t1->runAction(Sequence::create(DelayTime::create(3.4),
                                      CallFunc::create([=]() {
                                        SimpleAudioEngine::getInstance()->playEffect("abc_sound_19.mp3");
                                      }),
                                      EaseBackOut::create(ScaleTo::create(0.15, 1)),
                                      NULL));
        
        t2->runAction(Sequence::create(DelayTime::create(4),
                                      CallFunc::create([=]() {
                                        SimpleAudioEngine::getInstance()->playEffect("abc_sound_19.mp3");
                                      }),
                                      EaseBackOut::create(ScaleTo::create(0.15, 1)),
                                      DelayTime::create(0.7),
                                      CallFunc::create([=]() {
            SimpleAudioEngine::getInstance()->playEffect("reading_slide_2.mp3");
            
            m->runAction(MoveBy::create(0.2, {m->getContentSize().width * 1.5f, 0}));
            i->runAction(MoveBy::create(0.2, {i->getContentSize().width * 0.5f, 0}));
            t1->runAction(MoveBy::create(0.2, {-t1->getContentSize().width * 0.5f, 0}));
            t2->runAction(MoveBy::create(0.2, {-t2->getContentSize().width * 1.5f, 0}));
        }), NULL));
    }
}

void Page2Layer::openSlide3() {
    Sprite *obj = Sprite::create("reading_slide_3.png");
    
    _slideNode->addChild(obj);
    
    obj->setPosition({_mount->getContentSize().width * 0.5f, _mount->getContentSize().height * 0.6f});
    obj->setScale(0);
    
    obj->runAction(Sequence::create(DelayTime::create(0.8),
                                    EaseBackOut::create(ScaleTo::create(0.15, 1)),
                                    CallFunc::create([=]() {
        SimpleAudioEngine::getInstance()->playEffect("reading_slide_3.mp3");
    }), NULL));
    
    
    {
        Sprite *b = Sprite::create("ch_small_1.png");
        _slideNode->addChild(b);
        
        Sprite *a = Sprite::create("ch_small_0.png");
        _slideNode->addChild(a);
        
        Sprite *g = Sprite::create("ch_small_6.png");
        _slideNode->addChild(g);
        
        b->setPosition({_mount->getContentSize().width * 0.5f - b->getContentSize().width * 2.5f,
            _mount->getContentSize().height * 0.15f
        });
        
        a->setPosition({_mount->getContentSize().width * 0.5f, _mount->getContentSize().height * 0.15f});
        
        g->setPosition({_mount->getContentSize().width * 0.5f + b->getContentSize().width * 2.5f,
            _mount->getContentSize().height * 0.15f
        });
        
        b->setScale(0);
        a->setScale(0);
        g->setScale(0);
        
        b->runAction(Sequence::create(DelayTime::create(2.1),
                                      CallFunc::create([=]() {
                                        SimpleAudioEngine::getInstance()->playEffect("abc_sound_1.mp3");
                                      }),
                                      EaseBackOut::create(ScaleTo::create(0.15, 1)),
                                      NULL));
        
        a->runAction(Sequence::create(DelayTime::create(2.7),
                                      CallFunc::create([=]() {
                                        SimpleAudioEngine::getInstance()->playEffect("abc_sound_0.mp3");
                                      }),
                                      EaseBackOut::create(ScaleTo::create(0.15, 1)),
                                      NULL));
        
        g->runAction(Sequence::create(DelayTime::create(3.3),
                                      CallFunc::create([=]() {
                                        SimpleAudioEngine::getInstance()->playEffect("abc_sound_6.mp3");
                                      }),
                                      EaseBackOut::create(ScaleTo::create(0.15, 1)),
                                      DelayTime::create(0.7),
                                      CallFunc::create([=]() {
            SimpleAudioEngine::getInstance()->playEffect("reading_slide_3.mp3");
            
            b->runAction(MoveBy::create(0.2, {b->getContentSize().width * 1.5f, 0}));
            g->runAction(MoveBy::create(0.2, {-b->getContentSize().width * 1.5f, 0}));
        }), NULL));
    }
}