//
//  EAVIGUI.borderEffect.h
//  FishPolice
//
//  Created by Chris on 01/07/2013.
//
//

#ifndef __FishPolice__EAVIGUI_borderEffect__
#define __FishPolice__EAVIGUI_borderEffect__

#include <iostream>
#include "EAVIGUI.baseEffect.h"

namespace EAVIGUI {
    
    class borderEffect : public baseEffect {
    public:
        borderEffect(ofColor newcol);
        void draw();
        ofColor col;
    };
    
};
#endif /* defined(__FishPolice__EAVIGUI_borderEffect__) */
