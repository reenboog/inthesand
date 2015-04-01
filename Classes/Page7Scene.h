//
//  Page7Scene.h
//  inthesand
//
//  Created by Alex Gievsky on 18.03.15.
//
//

#ifndef __inthesand__Page7Scene__
#define __inthesand__Page7Scene__

#include <vector>
#include "cocos2d.h"

class Page7Layer: public cocos2d::Layer {
public:
    Page7Layer();
    virtual ~Page7Layer();
    
    static cocos2d::Scene* scene();
    
    virtual bool init();
    
    void onBackBtnPressed();
    void onNextBtnPressed();
    void onMuteBtnPressed();
    void onCharacterBtnPressed(cocos2d::Ref *btn);
    
    void openSlide(int slide);
    
    CREATE_FUNC(Page7Layer);
private:
    void popOut();
    
    void openSlide0();
    void openSlide1();
    void openSlide2();
    void openSlide3();
    void openSlide4();
    void openSlide5();
    void openSlide6();
    void openSlide7();
    
    void block();
    void unblock();
    
    bool isMuted() const;
    void setMuted(bool muted);
private:
    cocos2d::Sprite *_back;
    
    cocos2d::Sprite *_top;
    cocos2d::Sprite *_bottom;
    
    cocos2d::Sprite *_mount;
    
    cocos2d::Sprite *_muted;
    
    cocos2d::Menu *_menu;
    cocos2d::MenuItemImage *_backBtn;
    cocos2d::MenuItemImage *_muteBtn;
    cocos2d::MenuItemImage *_nextBtn;
    
    cocos2d::Label *_string;
    cocos2d::Label *_animatedString;
    
    cocos2d::Node *_slideObjectsNode;
    cocos2d::Node *_slideTextsNode;
    
    // texts
//    std::vector<std::pair<float, cocos2d::Label*> > _texts;
    
    int _currentSlide;
};

#endif /* defined(__inthesand__Page7Scene__) */
