
#include "GameLayer.h"
#include "SimpleAudioEngine.h"

#define zBack 0

using namespace cocos2d;

GameLayer::~GameLayer() {
    
}

GameLayer::GameLayer(): Layer() {
    _back = nullptr;
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
        
        this->addChild(_back, zBack);
    }
    
    return true;
}