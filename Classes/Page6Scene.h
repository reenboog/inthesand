//
//  Page6Scene.h
//  inthesand
//
//  Created by Alex Gievsky on 17.03.15.
//
//

#ifndef __inthesand__Page6Scene__
#define __inthesand__Page6Scene__

#include <vector>
#include "cocos2d.h"

class Page6Layer: public cocos2d::Layer {
public:
    Page6Layer();
    virtual ~Page6Layer();
    
    static cocos2d::Scene* scene();
    
    virtual bool init();
    
    void onBackBtnPressed();
    
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event  *event);
    void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event  *event);
    
    void onReplayBtnPressed();
    
    CREATE_FUNC(Page6Layer);
private:
    void popOut();
    
    void popUpANewWord();
    
    void showTutorial();
        
    void block();
    void unblock();
private:
    cocos2d::Sprite *_back;
    
    cocos2d::Sprite *_top;
    cocos2d::Sprite *_bottom;
    
    cocos2d::Menu *_menu;
    cocos2d::MenuItemImage *_backBtn;
    cocos2d::MenuItemImage *_replayBtn;
    
    cocos2d::Menu *_blockMenu;
    
    std::vector<std::vector<int> > _words;
    std::vector<cocos2d::Sprite*> _boxes;
    std::vector<cocos2d::Node*> _letters;
    
    cocos2d::Node *_bBtn;
    
    cocos2d::Sprite *_currentLetter;
    
    cocos2d::Menu *_lettersMenu;

    int _currentSlide;
};

#endif /* defined(__inthesand__Page6Scene__) */
