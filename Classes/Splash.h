//
//  Splash.h
//  inthesand
//
//  Created by Alex Gievsky on 01.04.15.
//
//

#ifndef __inthesand__Splash__
#define __inthesand__Splash__

#include "cocos2d.h"

class Splash: public cocos2d::Layer {
public:
    Splash() {}
    virtual ~Splash() {}
    
    static cocos2d::Scene* scene();
    
    virtual bool init();
    
    CREATE_FUNC(Splash);
};


#endif /* defined(__inthesand__Splash__) */
