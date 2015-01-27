//
//  Shared.cpp
//  1pGigTickets
//
//  Created by Alex Gievsky on 18.09.13.
//
//

#include "Shared.h"
#include "Localized.h"
#include "json/document.h"

USING_NS_CC;

using namespace std;

Animation* Shared::loadAnimation(const string &fileName, const string &name) {
    Animation *animation = AnimationCache::getInstance()->getAnimation(name);
    if(animation != nullptr) {
        return animation;
    } else {
        animation = Animation::create();
    }
    
    unsigned char *t = nullptr;
    ssize_t animationsDataSize = 0;
    t = (FileUtils::getInstance()->getFileData(fileName, "r", &animationsDataSize));
    
    char *animationsData = new char[animationsDataSize + 1];
    memcpy(animationsData, t, animationsDataSize);
    animationsData[animationsDataSize] = NULL;
    
    delete[] t;
    t = NULL;
    
    rapidjson::Document animationsDoc;
    animationsDoc.Parse<0>(animationsData);
    
    const auto &animationMap = animationsDoc[name.c_str()];
    if(animationMap.IsObject()) {
        const auto &framesArray = animationMap["frames"];
        
        const auto &spriteFrameObj = animationMap["baseSpriteFrameName"];
        
        string spriteFrameName = name;
        if(!spriteFrameObj.IsNull()) {
            spriteFrameName = animationMap["baseSpriteFrameName"].GetString();
        }
        
        for(int i = 0; i < framesArray.Size(); i++) {
            int frameIndex = framesArray[i].GetInt();
            
            string frameName = StringUtils::format("%s_%i.png", spriteFrameName.c_str(), frameIndex);
            
            animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
        }
        
        double delay = animationMap["delay"].GetDouble();
        
        animation->setDelayPerUnit(delay);
    }
    
    delete[] animationsData;

    AnimationCache::getInstance()->addAnimation(animation, name);
    
    return animation;
}

string Shared::formatValue(double value, bool skipSuffix) {
    string result = "";
    if(value < 1.0) {
        result = StringUtils::format("%.1f %s", value * 100, Localized::getString("centi").c_str());
    } else {
        double d = 1;
        string pref = "";
        
        if(value > 999999999999999) { // > 999 teras -> wtf?
            result = Localized::getString("wtf");
        } else {
            if(value > 999999999999) {
                d = 1000000000000;
                pref = Localized::getString("tera");
            } else if(value > 999999999) {
                d = 1000000000;
                pref = Localized::getString("giga");
            } else if(value > 999999) {
                d = 1000000;
                pref = Localized::getString("mega");
            } else if(value > 999) {
                d = 1000;
                pref = Localized::getString("kilo");
            }
            
            value /= d;
            int precision = 1 + static_cast<int>(100 * (value - static_cast<int>(value))) % 10 == 0 ? 0 : 1;
            
            if(skipSuffix) {
                pref = "";
            }
            
            result = StringUtils::format("%.*f %s", precision, value, pref.c_str());
        }
    }
    
    return result;
}
