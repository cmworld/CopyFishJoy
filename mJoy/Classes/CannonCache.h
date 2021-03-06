//
//  CannonCache.h
//  mJoy
//
//  Created by Jerry Yang on 7/22/13.
//
//

#ifndef __CannonCache__
#define __CannonCache__

#include "cocos2d.h"
#include "GameConfig.h"

class CannonCache : public cocos2d::CCNode
{
public:
    CannonCache(void);
    ~CannonCache(void);

    static void changeLevel(CannonCache *cannon_cache, char op_type);
    static void touchFollow(CannonCache *cannon_cache, cocos2d::CCPoint touch_pos);

    int level;  // 1 - 7,按图片记数来标记
    /** 美术资源中炮有两套,只用其中之一 */
    cocos2d::CCArray *CannonBatch;
    cocos2d::CCSize windowSize;

private:
    void initBatch(void);
};

#endif /* defined(__CannonCache__) */
