//
//  Page3Scene.h
//  inthesand
//
//  Created by Alex Gievsky on 10.03.15.
//
//

#ifndef __inthesand__Page3Scene__
#define __inthesand__Page3Scene__

#include <vector>
#include "cocos2d.h"

class Page3Layer: public cocos2d::Layer {
public:
    Page3Layer();
    virtual ~Page3Layer();
    
    static cocos2d::Scene* scene();
    
    virtual bool init();
    
    void onBackBtnPressed();
    void onNextBtnPressed();
    void onCharacterBtnPressed(cocos2d::Ref *btn);
    
    void openSlide(int slide);
    
    // touches
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event  *event);
    void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event  *event);
    
    CREATE_FUNC(Page3Layer);
private:
    void popOut();
    
    void openSlide0();
    void openSlide1();
    void openSlide2();
    void openSlide3();
    
    void block();
    void unblock();
private:
    cocos2d::Sprite *_back;
    
    cocos2d::Sprite *_top;
    cocos2d::Sprite *_bottom;
    
    cocos2d::Sprite *_mount;
    
    cocos2d::Menu *_lettersMenu;
    std::vector<cocos2d::Node*> _letters;
    
    cocos2d::Menu *_menu;
    cocos2d::MenuItemImage *_backBtn;
    cocos2d::MenuItemImage *_nextBtn;
    
    cocos2d::Node *_slideNode;
    
    cocos2d::Sprite *_sqr0;
    cocos2d::Sprite *_sqr1;
    cocos2d::Sprite *_sqr2;
    
    cocos2d::Sprite *_currentLetter;
    cocos2d::MenuItemImage *_bButton;
    cocos2d::MenuItemImage *_iButton;
    cocos2d::MenuItemImage *_nButton;
    bool _tutorialWasShown;
    
    cocos2d::Menu *_blockMenu;
    
    int _currentSlide;
    int _attemptsMade;
};

#endif /* defined(__inthesand__Page3Scene__) */
