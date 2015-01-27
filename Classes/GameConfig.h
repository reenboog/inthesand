//
//  GameConfig.h
//  solveMe
//
//  Created by Alex Gievsky on 11.11.13.
//
//

#ifndef __solveMe__GameConfig__
#define __solveMe__GameConfig__

#include "cocos2d.h"
#include "Constants.h"

USING_NS_CC;

#define kAppVersion 1

class GameConfig: public GameValues<GameConfig> {
private:
    GameConfig();
    void setDesignLayoutSize(const Size &size);
    void setVisibleOrigin(const Point &point);
public:
    ~GameConfig();
    
    void load();
    void save();
    
    Size getDesignLayoutSize();
    Point getVisibleOrigin();
    
    // notifications
    bool areRemoteNotificationsEnabled();
    void setRemoteNotificationsEnabled(bool enabled);
    
    bool isAppAlreadyRated();
    void setAppRated(bool rated);
    
    bool wasRateBadgeAlreadyShown();
    void setRateBadgeAlreadyShown(bool shown);
    
    bool wasTutorialShown();
    void setTutorialWasShown(bool shown);
    
    bool wasFriendsPermissionAsked() const;
    void setFriendsPermissionAsked(bool b);
    
    bool wasSyncPermissionAsked() const;
    void setSyncPermissionAsked(bool b);
    
    bool wasGetPillsFromFriendRequestSent() const;
    void setPillsFromFriendsRequestSent(bool b);
    
    // audio stuff
    void setMusicVolume(float volume);
    float getMusicVolume();
    
    void setSoundVolume(float volume);
    float getSoundVolume();
    
    // languages
    void setCurrentLanguageType(int type);
    int getCurrentLanguageType();
    
    void loadFilePaths();
    
    static GameConfig* sharedInstance();
    static void purge();
private:
    static GameConfig *__sharedInstance;
    
    Point _visibleOrigin;
    Size _designLayoutSize;
    
    bool _rateBadgeWasAlreadyShown;
    bool _remoteNotificationsEnabled;
    bool _appWasRated;
    bool _tutorialWasShown;
    bool _friendsPermissionWasAsked;
    bool _syncPermissionWasAsked;
    bool _wasGetPillsFromFriendsRequestSent;
    
    float _soundVolume;
    float _musicVolume;
};

#endif /* defined(__solveMe__GameConfig__) */
