
#include "GameLayer.h"
#include "SimpleAudioEngine.h"
#include "Localized.h"

#include "Page0Scene.h"
#include "Page1Scene.h"
#include "Page2Scene.h"
#include "Page3Scene.h"
#include "Page4Scene.h"

#define zBack 0

#define kSwipeMinimalLength 50
#define kNumOfPages 7

using namespace cocos2d;

using std::vector;
using std::string;

GameLayer::~GameLayer() {
    
}

GameLayer::GameLayer(): Layer() {
    _back = nullptr;
    
    _cursorIndex = 0;
    
    _currentPage = nullptr;
    _pageBtn = nullptr;
    _pageBtnTitle = nullptr;
}

Scene* GameLayer::scene() {
    auto scene = Scene::create();
    auto gameLayer = GameLayer::create();

    // add layer as a child to scene
    scene->addChild(gameLayer);

    // return the scene
    return scene;
}

bool GameLayer::init() {
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
        
        this->addChild(_back, zBack);
    }
    
    // set up touches
    {
        auto touchesListener = EventListenerTouchOneByOne::create();
        
        touchesListener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
        touchesListener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded, this);
        touchesListener->onTouchMoved = CC_CALLBACK_2(GameLayer::onTouchMoved, this);
        touchesListener->onTouchCancelled = CC_CALLBACK_2(GameLayer::onTouchCancelled, this);
        
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchesListener, this);
    }
    
    // cursors
    {
        for(int i = 0; i < kNumOfPages; ++i) {
            Sprite *cursor = Sprite::create("main_menu_page_cursor.png");
            this->addChild(cursor);
            
            cursor->setAnchorPoint({0.0f, 0.5f});
            cursor->setPosition({visibleSize.width * 0.5f - (cursor->getContentSize().width * kNumOfPages) * 0.5f + cursor->getContentSize().width * i,
                visibleSize.height * 0.095f});
            
            _cursorsSprites.push_back(cursor);
        }
    }
    
    // menus {
    _pageBtn = MenuItemImage::create("btn_main_menu.png", "btn_main_menu_on.png", CC_CALLBACK_0(GameLayer::onPageBtnPressed, this));

    Menu *menu = Menu::create(_pageBtn, nullptr);
    this->addChild(menu);
    
    menu->setPosition({visibleSize.width * 0.5f, visibleSize.height * 0.2583f});
    
    _pageBtnTitle = Label::createWithBMFont("arial_rounded_mt_bold_72.fnt", "");
    _pageBtn->addChild(_pageBtnTitle);
    
    _pageBtnTitle->setPosition({_pageBtn->getContentSize().width * 0.5f, _pageBtn->getContentSize().height * 0.5f});
    
    this->setCursor(0);
    
    return true;
}

#pragma mark - Touches

bool GameLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) {
    this->_touchPrevPos = touch->getLocation();
    
    return true;
}

void GameLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) {
    Point pos = touch->getLocation();
    
    float delta = pos.x - _touchPrevPos.x;
    
    if(fabs(delta) > kSwipeMinimalLength) {
        if(delta < 0) {
            _cursorIndex++;
            
            this->setCursor(_cursorIndex);
        } else {
            _cursorIndex--;
            
            if(_cursorIndex < 0) {
                _cursorIndex = kNumOfPages - 1;
            }
            
            this->setCursor(_cursorIndex);
        }
    }
}

void GameLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event  *event) {
}

void GameLayer::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event  *event) {
    
}

void GameLayer::setCursor(int cursor) {
    _cursorIndex = cursor % (kNumOfPages);
    
    for(int i = 0; i < kNumOfPages; ++i) {
        if(i == _cursorIndex) {
            _cursorsSprites[i]->setColor({255, 180, 50});
        } else {
            _cursorsSprites[i]->setColor({3, 214, 138});
        }
    }
    
    _pageBtnTitle->setString(Localized::getString(StringUtils::format("page_title_%i", _cursorIndex)));
    
    if(_currentPage != nullptr) {
        _currentPage->removeFromParent();
    }
    
    _currentPage = Sprite::create(StringUtils::format("page_%i.png", _cursorIndex));
    this->addChild(_currentPage);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    _currentPage->setPosition({visibleSize.width * 0.5f, visibleSize.height * 0.5f});
    _currentPage->setScale(0.7);
    
    _currentPage->runAction(Sequence::create(EaseBackOut::create(ScaleTo::create(0.2, 1.0f)),
                                             NULL));
    
    _pageBtn->stopAllActions();
    _pageBtn->setScale(0.8);
    
    _pageBtn->runAction(Sequence::create(DelayTime::create(0.1),
                                         EaseBackOut::create(ScaleTo::create(0.2, 1)),
                                         NULL));
}

void GameLayer::onPageBtnPressed() {
    _pageBtn->setEnabled(false);

    this->popOut();
}

void GameLayer::popOut() {
    _pageBtn->runAction(EaseBackIn::create(ScaleTo::create(0.35f, 0)));
    _currentPage->runAction(Sequence::create(DelayTime::create(0.1),
                                             EaseBackIn::create(ScaleTo::create(0.3, 0)),
                                             NULL));
    
    this->runAction(Sequence::create(DelayTime::create(0.3),
                                     CallFunc::create([this]() {
        this->openPageScene(_cursorIndex);
    }), NULL));
}

void GameLayer::openPageScene(int idx) {
    Scene *scene = nullptr;
    switch(idx) {
        case 0:
            scene = Page0Layer::scene();
            break;
        case 1:
            scene = Page1Layer::scene();
            break;
        case 2:
            scene = Page2Layer::scene();
            break;
        case 3:
            scene = Page3Layer::scene();
            break;
        case 5:
            scene = Page4Layer::scene();
            break;

        default:
            scene = Page0Layer::scene();
            break;
    }
    
    Director::getInstance()->replaceScene(scene);
}