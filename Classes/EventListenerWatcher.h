//
//  Header.h
//  mr_bate
//
//  Created by reenboog on 27.11.14.
//
//

#ifndef mr_bate_EventListenerWatcher_h
#define mr_bate_EventListenerWatcher_h

#include "cocos2d.h"
#include "Constants.h"

class EventListenerWatcher {
public:
    virtual ~EventListenerWatcher() {
        for(cocos2d::EventListener *l : _customListeners) {
            cocos2d::Director::getInstance()->getEventDispatcher()->removeEventListener(l);
        }
        
        _customListeners.clear();
    }
    
    void subscribeForEvent(const std::string &name, const std::function<void(cocos2d::EventCustom*)> &callback) {
        cocos2d::EventListener *l = NC_ADD(name, callback);

        _customListeners.push_back(l);
    }
private:
    std::vector<cocos2d::EventListener*> _customListeners;
};

#endif
