//
//  MainMenu.cpp
//  inthesand
//
//  Created by Alex Gievsky on 14.04.15.
//
//

#include "MainMenu.h"

#include "SimpleAudioEngine.h"
#include "Localized.h"

#include "Page0Scene.h"
#include "Page1Scene.h"
#include "Page2Scene.h"
#include "Page3Scene.h"
#include "Page4Scene.h"
#include "Page6Scene.h"
#include "Page7Scene.h"

using namespace cocos2d;
using namespace CocosDenshion;

#include <vector>

using namespace std;

Scene* MainMenu::scene() {
    auto scene = Scene::create();
    auto gameLayer = MainMenu::create();
    
    // add layer as a child to scene
    scene->addChild(gameLayer);
    
    // return the scene
    return scene;
}

bool MainMenu::init() {
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
    
    // menu
    vector<int> buttonsIndices;
    
    buttonsIndices.push_back(1);
    buttonsIndices.push_back(2);
    buttonsIndices.push_back(3);
    buttonsIndices.push_back(0);
    buttonsIndices.push_back(4);
    buttonsIndices.push_back(5);
    
    Vector<MenuItem*> buttons;
    
    for(int i = 0; i < buttonsIndices.size(); ++i) {
        MenuItemImage *btn = MenuItemImage::create("btn_main_menu_item.png", "btn_main_menu_item_on.png", CC_CALLBACK_1(MainMenu::onBtnPressed, this));
        btn->setTag(i);
        
        buttons.pushBack(btn);
        
        Sprite *icon = Sprite::create(StringUtils::format("page_%i.png", buttonsIndices[i]));
        btn->addChild(icon);
        
        icon->setScale(0.2f);
        
        icon->setPosition({btn->getContentSize().width * 0.15f, btn->getContentSize().height * 0.5f});
        
        Label *label = Label::createWithBMFont("arial_rounded_mt_bold_72.fnt", Localized::getString(StringUtils::format("page_title_%i", buttonsIndices[i])));
        btn->addChild(label);
        
        label->setPosition(btn->getContentSize().width * 0.5f, btn->getContentSize().height * 0.5f);
        
        if(i == buttonsIndices.size() - 1) {
            label->setScale(0.95);
            
            label->setPosition(btn->getContentSize().width * 0.55f, btn->getContentSize().height * 0.5f);
        }
    }
    
    Menu *menu = Menu::createWithArray(buttons);
    this->addChild(menu);
    
    menu->setPosition({visibleSize.width * 0.5f, visibleSize.height * 0.5f});
    menu->alignItemsVerticallyWithPadding(-20);
    
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.2);
    
    SimpleAudioEngine::getInstance()->playBackgroundMusic("main_menu.mp3");
    
    return true;
}

void MainMenu::onBtnPressed(cocos2d::Ref *btn) {
    int tag = (static_cast<Node*>(btn)->getTag());
    
    Scene *scene = nullptr;
    switch(tag) {
        case 0:
            scene = Page1Layer::scene();
            break;
        case 1:
            scene = Page2Layer::scene();
            break;
        case 2:
            scene = Page3Layer::scene();
            break;
        case 3:
            scene = Page7Layer::scene();
            break;
        case 4:
            scene = Page4Layer::scene();
            break;
        case 5:
            scene = Page6Layer::scene();
            break;
            
        default:
            scene = Page0Layer::scene();
            break;
    }
    
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    
    Director::getInstance()->replaceScene(scene);
}