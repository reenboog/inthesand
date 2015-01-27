//
//  Localized.cpp
//  astroGems
//
//  Created by Alex Gievsky on 30.10.13.
//
//

#include "Localized.h"
#include "Shared.h"
#include "json/document.h"
#include "Shared.h"
//#include "Constants.h"

Localized *Localized::__sharedInstance = nullptr;

Localized::~Localized() {
    //
}

Localized::Localized() {
    //
}

void Localized::load() {
   if(__sharedInstance == nullptr) {
        __sharedInstance = new Localized();
    } else {
        return;
    }
    
    unsigned char *t = nullptr;
    ssize_t stringsDataSize = 0;
    t = (FileUtils::getInstance()->getFileData("strings.json", "r", &stringsDataSize));
    
    char *stringsData = new char[stringsDataSize + 1];
    memcpy(stringsData, t, stringsDataSize);
    stringsData[stringsDataSize] = NULL;
    
    delete[] t;
    t = NULL;
    
    rapidjson::Document stringsDoc;
    stringsDoc.Parse<0>(stringsData);
    
    const auto &stringsMap = stringsDoc["strings"];
    if(stringsMap.IsObject()) {
        for(auto it = stringsMap.MemberonBegin(); it != stringsMap.MemberonEnd(); ++it) {
            string key = it->name.GetString();
            string value = it->value.GetString();
            
            __sharedInstance->strings.insert(make_pair(key, value));
        }
    }
    
    delete[] stringsData;
}

void Localized::purge() {
    delete __sharedInstance;
    __sharedInstance = nullptr;
}

string Localized::getString(const string &key) {
    if(__sharedInstance == nullptr) {
        Localized::load();
    }
    
    auto stringIt = __sharedInstance->strings.find(key);
    
    if(stringIt != __sharedInstance->strings.end()) {
        return stringIt->second;
    } else {
        CCLOG("String not found for key: %s", key.c_str());
    }
    
    return "notFound";
}

string Localized::languageShortNameForType(cocos2d::LanguageType type) {
    string languagePrefix = "unknown";
    switch(type) {
        case LanguageType::ENGLISH:
            languagePrefix = "en"; break;
        case LanguageType::CHINESE:
            languagePrefix = "zh"; break;
        case LanguageType::FRENCH:
            languagePrefix = "fr"; break;
        case LanguageType::ITALIAN:
            languagePrefix = "it"; break;
        case LanguageType::GERMAN:
            languagePrefix = "de"; break;
        case LanguageType::SPANISH:
            languagePrefix = "es"; break;
        case LanguageType::RUSSIAN:
            languagePrefix = "ru"; break;
        case LanguageType::KOREAN:
            languagePrefix = "ko"; break;
        case LanguageType::JAPANESE:
            languagePrefix = "ja"; break;
        case LanguageType::HUNGARIAN:
            languagePrefix = "hu"; break;
        case LanguageType::PORTUGUESE:
            languagePrefix = "pt"; break;
        case LanguageType::ARABIC:
            languagePrefix = "ar"; break;
        case LanguageType::NORWEGIAN:
            languagePrefix = "nb"; break;
        case LanguageType::POLISH:
            languagePrefix = "pl"; break;
        default:
            break;
    }
    
    return languagePrefix;
}