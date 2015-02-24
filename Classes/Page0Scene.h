//
//  Page0Scene.h
//  inthesand
//
//  Created by Alex Gievsky on 24.02.15.
//
//

#ifndef __inthesand__Page0Scene__
#define __inthesand__Page0Scene__

#include "cocos2d.h"

#include <vector>

class Page0Layer: public cocos2d::Layer {
public:
    Page0Layer();
    virtual ~Page0Layer();
    
    static cocos2d::Scene* scene();
    
    virtual bool init();

    void onBackBtnPressed();
    void onSpeechBtnPressed();
    
    CREATE_FUNC(Page0Layer);
private:
    void popOut();
private:
    cocos2d::Sprite *_back;
    cocos2d::Sprite *_mount;
    cocos2d::LayerColor *_top;
    
    cocos2d::Sprite *_text;
    
    cocos2d::Menu *_menu;
    cocos2d::MenuItemImage *_backBtn;
    cocos2d::MenuItemImage *_speechBtn;
    
};

#endif /* defined(__inthesand__Page0Scene__) */
