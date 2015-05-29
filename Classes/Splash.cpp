//
//  Splash.cpp
//  inthesand
//
//  Created by Alex Gievsky on 01.04.15.
//
//

#include "Splash.h"
//#include "GameLayer.h"
#include "MainMenu.h"

using namespace cocos2d;

Scene* Splash::scene() {
    auto scene = Scene::create();
    auto gameLayer = Splash::create();
    
    // add layer as a child to scene
    scene->addChild(gameLayer);
    
    // return the scene
    return scene;
}

bool Splash::init() {
    if(!Layer::init()) {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    // back
    {
        Sprite *back = Sprite::create("back.png");
        back->setPosition({static_cast<float>(visibleSize.width / 2.0), static_cast<float>(visibleSize.height / 2.0)});
        
        Size backSize = back->getContentSize();
        
        back->setScale(visibleSize.width / backSize.width, visibleSize.height / backSize.height);
        back->setColor({247, 241, 235});
        
        this->addChild(back);
    }
    
    // logo
    {
        Sprite *logo = Sprite::create("logo.png");
        this->addChild(logo);
        
        logo->setPosition({visibleSize.width * 0.5f, visibleSize.height * 0.6f});
    }
    
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create([=]() {
        Director::getInstance()->replaceScene(MainMenu::scene());
    }), NULL));
    
    return true;
}