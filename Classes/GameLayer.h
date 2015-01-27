#ifndef __GAME_LAYER_H__
#define __GAME_LAYER_H__

#include "cocos2d.h"

class GameLayer: public cocos2d::Layer {
public:
    GameLayer();
    ~GameLayer();
    
    static cocos2d::Scene* scene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    CREATE_FUNC(GameLayer);
private:
    cocos2d::Sprite *_back;
};

#endif // __HELLOWORLD_SCENE_H__
