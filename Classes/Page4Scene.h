//
//  Page4Scene.h
//  inthesand
//
//  Created by Alex Gievsky on 11.03.15.
//
//

#ifndef __inthesand__Page4Scene__
#define __inthesand__Page4Scene__

//#include <map>

class Page4Layer: public cocos2d::Layer {
public:
    Page4Layer();
    virtual ~Page4Layer();
    
    static cocos2d::Scene* scene();
    
    virtual bool init();
    
    void onBackBtnPressed();
    void onNextBtnPressed();
    void onColorBtnPressed(cocos2d::Ref *btn);
    
    void openSlide(int slide);
    
    CREATE_FUNC(Page4Layer);
private:
    void popOut();
    
    void openSlide0();
    void openSlide1();
    void openSlide2();
    void openSlide3();
    void openSlide4();
    
    void block();
    void unblock();
private:
    cocos2d::Sprite *_back;
    
    cocos2d::Sprite *_top;
    cocos2d::Sprite *_bottom;
    
    cocos2d::Sprite *_mount;
    
    cocos2d::Menu *_colorsMenu;
    
    cocos2d::MenuItemImage *_blackBtn;
    
    cocos2d::Menu *_menu;
    cocos2d::MenuItemImage *_backBtn;
    cocos2d::MenuItemImage *_nextBtn;
    
    cocos2d::Node *_slideNode;
    cocos2d::Sprite *_obj;
    
    cocos2d::Menu *_blockMenu;
    
//    std::map<int, cocos2d::Color3B> _colors;
    
    bool _tutorialWasShown;
    int _currentSlide;
};

#endif /* defined(__inthesand__Page4Scene__) */
