//
//  Page2Scene.h
//  inthesand
//
//  Created by Alex Gievsky on 08.03.15.
//
//

#ifndef __inthesand__Page2Scene__
#define __inthesand__Page2Scene__

#include "cocos2d.h"

class Page2Layer: public cocos2d::Layer {
public:
    Page2Layer();
    virtual ~Page2Layer();
    
    static cocos2d::Scene* scene();
    
    virtual bool init();
    
    void onBackBtnPressed();
    void onNextBtnPressed();
    void onCharacterBtnPressed(cocos2d::Ref *btn);
    
    void openSlide(int slide);
    
    CREATE_FUNC(Page2Layer);
private:
    void popOut();
    
    void openSlide0();
    void openSlide1();
    void openSlide2();
    void openSlide3();
private:
    cocos2d::Sprite *_back;
    
    cocos2d::Sprite *_top;
    cocos2d::Sprite *_bottom;
    
    cocos2d::Sprite *_mount;
    
    cocos2d::Menu *_lettersMenu;
    
    cocos2d::Menu *_menu;
    cocos2d::MenuItemImage *_backBtn;
    cocos2d::MenuItemImage *_nextBtn;
    
    cocos2d::Node *_slideNode;
    
    int _currentSlide;
};

#endif /* defined(__inthesand__Page2Scene__) */
