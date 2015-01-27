//
//  Shared.h
//  1pGigTickets
//
//  Created by Alex Gievsky on 18.09.13.
//
//

#ifndef ___pGigTickets__Shared__
#define ___pGigTickets__Shared__

#include "cocos2d.h"

class Shared {
private:
    Shared() {};
    virtual ~Shared() {};
public:
    static cocos2d::Animation* loadAnimation(const std::string &fileName, const std::string &name);
    
    static std::string formatValue(double value, bool skipSuffix = false);
};

#endif /* defined(___pGigTickets__Shared__) */
