#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#include <map>

#define kPTMRatio (Director::getInstance()->getVisibleSize().width / 10.0f)

#define b2d_2_coco(a)(kPTMRatio * (a))
#define coco_2_b2d(a)((a) / kPTMRatio)

#define NC_ADD(name, handler) cocos2d::Director::getInstance()->getEventDispatcher()->\
                                        addCustomEventListener(name, handler)

#define NC_POST(name, data) cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(name, data)

#define kSNLoggedIn "snLoggedInNotification"
#define kSNLoggedOut "snLoggedOut"
#define kSNFailedToLogIn "snFailedToLogIn"

#define kSNUserDetailsFetched "snUserDetailsFetched"
#define kSNFailedToFetchUserDetails "snFailedToFetchUserDetails"
#define kSNUserAvatarFetched "snUserAvatarFetched"

#define kSNFailedToSyncProgress "snFailedToSyncProgress"
#define kSNProgressSynced "snProgressSynced"

#define kSNFriendsDetailsFetched "snFriendsDetailsFetched"
#define kSNFailedToFetchFriendsDetails "snFailedToFetchFriendsDetails"
#define kSNFriendAvatarFetched "snFriendAvatarFetched"

#define kSNIncomingRequestsRead "snIncomingRequestsRead"

#define kFakeAvatarPrefix "fake"
#define kNumOfFakeAvatars 10

static const char *kFakeCompetitorsNames[kNumOfFakeAvatars] = {
    "John",
    "Mathew",
    "Puma",
    "Tom",
    "Olga",
    "Helen",
    "Mike",
    "Alex",
    "Andrew",
    "Poal"
};

#define AFakeName(index)(kFakeCompetitorsNames[index])


template<typename T>
class GameValues {
public:
    GameValues() {
    }
    virtual ~GameValues() {
    }
public:
//    static int getPills() {
//        return _values._pills;
//    }
//    
//    static void setPills(int pills) {
//        _values._pills = pills;
//    }
    
    template<typename> friend class GameValues;

    template<typename SrcT>
    static void copyFrom() {
        GameValues<T>::_values = *(reinterpret_cast<GameValues<T>::Values*>(&(GameValues<SrcT>::_values)));
    }
private:
    struct Values {
        friend class GameValues<T>;
    public:
        Values() {
//            _pills = -1;
        }
    private:
//        int _pills;
    };
    
    static Values _values;
};

template<typename T>
typename GameValues<T>::Values GameValues<T>::_values;

#endif // __CONSTANTS_H__