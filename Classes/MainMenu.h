//
//  MainMenu.h
//  inthesand
//
//  Created by Alex Gievsky on 14.04.15.
//
//

#ifndef __inthesand__MainMenu__
#define __inthesand__MainMenu__

#include "cocos2d.h"

class MainMenu: public cocos2d::Layer {
public:
    MainMenu() {}
    virtual ~MainMenu() {}
    
    static cocos2d::Scene* scene();
    
    void onBtnPressed(cocos2d::Ref *btn);
    
    virtual bool init();
    
    CREATE_FUNC(MainMenu);
};


#endif /* defined(__inthesand__MainMenu__) */
