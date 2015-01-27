//
//  GameConfig.cpp
//  solveMe
//
//  Created by Alex Gievsky on 11.11.13.
//
//

#include "GameConfig.h"
#include "Constants.h"
#include "Shared.h"
#include "Localized.h"

#include "json/document.h"

#include <string>

using namespace std;

#define kSoundVolumeKey "soundVolumeKey"
#define kMusicVolumeKey "musicVolumeKey"
#define kRemoteNotificationsEnabledKey "remoteNotificationsEnabledKey"
#define kAppWasRatedKey "appWasRatedKey"
#define kRateBadgeWasShownKey "rateBadgeWasShownKey"
#define kTutorialWasShownKey "tutorialWasShownKey"

#define kRateUsNotification "rateUsNotificationKey"

GameConfig * GameConfig::__sharedInstance = nullptr;


GameConfig::~GameConfig() {
    
}

GameConfig::GameConfig(): GameValues<GameConfig>() {
    _rateBadgeWasAlreadyShown = false;
    _remoteNotificationsEnabled = false;
    _appWasRated = false;
    
    _tutorialWasShown = false;

    _soundVolume = 0;
    _musicVolume = 0;
    
    _friendsPermissionWasAsked = false;
    _syncPermissionWasAsked = false;
    _wasGetPillsFromFriendsRequestSent = false;
}

GameConfig * GameConfig::sharedInstance() {
    if(__sharedInstance == nullptr) {
        __sharedInstance = new GameConfig();
    }
    
    return __sharedInstance;
}

void GameConfig::purge() {
    delete __sharedInstance;
    __sharedInstance = nullptr;
    
    FileUtils::getInstance()->destroyInstance();
}

void GameConfig::load() {
    _soundVolume = UserDefault::getInstance()->getFloatForKey(kSoundVolumeKey, 0.8f);
    _musicVolume = UserDefault::getInstance()->getFloatForKey(kMusicVolumeKey, 0.7f);
    
    _remoteNotificationsEnabled = UserDefault::getInstance()->getBoolForKey(kRemoteNotificationsEnabledKey, false);
    _appWasRated = UserDefault::getInstance()->getBoolForKey(kAppWasRatedKey, false);
    _rateBadgeWasAlreadyShown = UserDefault::getInstance()->getBoolForKey(kRateUsNotification, false);

    _tutorialWasShown = UserDefault::getInstance()->getBoolForKey(kTutorialWasShownKey, false);
    
    //GameValues<GameConfig>::setCurrentLength(UserDefault::getInstance()->getDoubleForKey(kCurrentLengthKey, kUndefinedLength));
    
    // ok, we haven't played the game yet => our level is 0
    if(_tutorialWasShown == false) {
        //GameValues<GameConfig>::setCurrentLength(this->aLengthForLevelUp(0));
    }
}

void GameConfig::save() {
    // use user defaults here
    UserDefault::getInstance()->setFloatForKey(kMusicVolumeKey, _musicVolume);
    UserDefault::getInstance()->setFloatForKey(kSoundVolumeKey, _soundVolume);
    UserDefault::getInstance()->setBoolForKey(kRemoteNotificationsEnabledKey, _remoteNotificationsEnabled);
    UserDefault::getInstance()->setBoolForKey(kAppWasRatedKey, _appWasRated);
    UserDefault::getInstance()->setBoolForKey(kRateUsNotification, _rateBadgeWasAlreadyShown);
    
    UserDefault::getInstance()->setBoolForKey(kTutorialWasShownKey, _tutorialWasShown);
    
    // save active accessories as well
    //UserDefault::getInstance()->setIntegerForKey(kCharacterCurrentGogglesPrefixKey, CharacterAccessories<GameConfig>::getCurrentGoggles());
}

void GameConfig::loadFilePaths() {
    // load file paths
    ssize_t configDataSize = 0;
    unsigned char *t = (FileUtils::getInstance()->getFileData("Res/config.json", "r", &configDataSize));
    
    char *configData = new char[configDataSize + 1];
    memcpy(configData, t, configDataSize);
    configData[configDataSize] = NULL;
    
    delete[] t;
    t = NULL;
    
    
    LanguageType currentLanguage = Application::getInstance()->getCurrentLanguage();
    
    string languagePrefix = Localized::languageShortNameForType(currentLanguage);
    
    auto addResourcePath = [=](const string &path) {
        FileUtils::getInstance()->addSearchPath((path + languagePrefix).c_str());
        FileUtils::getInstance()->addSearchPath(path.c_str());
    };
    
    Size frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
    
    rapidjson::Document configDoc;
    configDoc.Parse<0>(configData);
    
    string layoutBaseDimensionType = configDoc["layoutBaseDimension"].GetString();
    
    float frameBaseDimension = frameSize.height;
    
    if(layoutBaseDimensionType.compare("width") == 0) {
        frameBaseDimension = frameSize.width;
    }

    const auto &designEntriesArray = configDoc["designResolutionEntries"];
    if(designEntriesArray.IsArray()) {
        for(int i = designEntriesArray.Size() - 1; i >= 0; --i) {
            const auto &currentDeviceEntry = designEntriesArray[i];
            
            if(currentDeviceEntry.IsObject()) {
                double currentResolutionBaseDimension = currentDeviceEntry[layoutBaseDimensionType.c_str()].GetDouble();
                
                if(frameBaseDimension > currentResolutionBaseDimension) {
                    int requiredEntryIndex = MIN(i + 1, designEntriesArray.Size() - 1);
                    const auto &requiredEntry = designEntriesArray[requiredEntryIndex];
                    double requiredBaseDimension = requiredEntry[layoutBaseDimensionType.c_str()].GetDouble();
                    
                    //
                    double layoutWidth = requiredEntry["designWidth"].GetDouble();
                    double layoutHeight = requiredEntry["designHeight"].GetDouble();
                    
                    this->setDesignLayoutSize({static_cast<float>(layoutWidth), static_cast<float>(layoutHeight)});
                    
                    float designLayoutBaseDimension = layoutHeight;
                    ResolutionPolicy policy = ResolutionPolicy::FIXED_HEIGHT;
                    
                    if(layoutBaseDimensionType.compare("width") == 0) {
                        designLayoutBaseDimension = layoutWidth;
                        //frameBaseDimension = frameSize.width;
                        policy = ResolutionPolicy::FIXED_WIDTH;
                    }
                    
                    Director::getInstance()->getOpenGLView()->setDesignResolutionSize(layoutWidth, layoutHeight, policy);
                    Director::getInstance()->setContentScaleFactor(requiredBaseDimension / designLayoutBaseDimension);
                    
                    Size visibleSize = Director::getInstance()->getVisibleSize();
                    this->setVisibleOrigin({static_cast<float>((visibleSize.width - layoutWidth) / 2.0), static_cast<float>((visibleSize.height - layoutHeight) / 2.0)});
                    //

                    // load required entry and all the entries above
                    for(; requiredEntryIndex >= 0; --requiredEntryIndex) {
                        const auto &entryToLoad = designEntriesArray[requiredEntryIndex];
                        
                        if(entryToLoad.IsObject()) {
                            for(auto it = entryToLoad["paths"].onBegin(); it != entryToLoad["paths"].onEnd(); ++it) {
                                string path = it->GetString();
                                addResourcePath(path);
                            }
                        }
                    }
                    
                    // stop iterating all the rest entries
                    break;
                }
            }
        }
    }

    // load shared paths
    
    const auto &sharedPathsArray = configDoc["sharedPaths"];
    if(sharedPathsArray.IsArray()) {
        for(auto it = sharedPathsArray.onBegin(); it != sharedPathsArray.onEnd(); ++it) {
            string path = it->GetString();
            addResourcePath(path);
        }
    }
    
    delete[] configData;
}

Size GameConfig::getDesignLayoutSize() {
    return this->_designLayoutSize;
}

Point GameConfig::getVisibleOrigin() {
    return this->_visibleOrigin;
}

void GameConfig::setDesignLayoutSize(const Size &size) {
    this->_designLayoutSize = size;
}

void GameConfig::setVisibleOrigin(const Point &point) {
    this->_visibleOrigin = point;
}

bool GameConfig::areRemoteNotificationsEnabled() {
    return _remoteNotificationsEnabled;
}

void GameConfig::setRemoteNotificationsEnabled(bool enabled) {
    this->_remoteNotificationsEnabled = enabled;
}

bool GameConfig::isAppAlreadyRated() {
    return this->_appWasRated;
}

void GameConfig::setAppRated(bool rated) {
    this->_appWasRated = rated;
}

bool GameConfig::wasRateBadgeAlreadyShown() {
    return this->_rateBadgeWasAlreadyShown;
}

void GameConfig::setRateBadgeAlreadyShown(bool shown) {
    this->_rateBadgeWasAlreadyShown = true;
}

bool GameConfig::wasTutorialShown() {
    return this->_tutorialWasShown;
}

void GameConfig::setTutorialWasShown(bool shown) {
    this->_tutorialWasShown = shown;
}

void GameConfig::setMusicVolume(float volume) {
    this->_musicVolume = volume;
}

float GameConfig::getMusicVolume() {
    return this->_musicVolume;
}

void GameConfig::setSoundVolume(float volume) {
    this->_soundVolume = volume;
}

float GameConfig::getSoundVolume() {
    return this->_soundVolume;
}

bool GameConfig::wasFriendsPermissionAsked() const {
    return this->_friendsPermissionWasAsked;
}

void GameConfig::setFriendsPermissionAsked(bool b) {
    this->_friendsPermissionWasAsked = b;
}

bool GameConfig::wasSyncPermissionAsked() const {
    return this->_syncPermissionWasAsked;
}

void GameConfig::setSyncPermissionAsked(bool b) {
    this->_syncPermissionWasAsked = b;
}

bool GameConfig::wasGetPillsFromFriendRequestSent() const {
    return this->_wasGetPillsFromFriendsRequestSent;
}

void GameConfig::setPillsFromFriendsRequestSent(bool b) {
    this->_wasGetPillsFromFriendsRequestSent = b;
}