//
//  Page1Scene.h
//  inthesand
//
//  Created by Alex Gievsky on 07.03.15.
//
//

#ifndef __inthesand__Page1Scene__
#define __inthesand__Page1Scene__

#include "cocos2d.h"

class Page1Layer: public cocos2d::Layer {
public:
    Page1Layer();
    virtual ~Page1Layer();
    
    static cocos2d::Scene* scene();
    
    virtual bool init();
    
    void onBackBtnPressed();
    void onCharacterBtnPressed(cocos2d::Ref *btn);
    
    CREATE_FUNC(Page1Layer);
private:
    void popOut();
private:
    cocos2d::Sprite *_back;

    cocos2d::Sprite *_top;
    cocos2d::Sprite *_bottom;
    
    cocos2d::Menu *_menu;
    cocos2d::Menu *_abcMenu;
    cocos2d::MenuItemImage *_backBtn;
};

#endif /* defined(__inthesand__Page1Scene__) */
